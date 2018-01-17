//
// Created by doom on 21/12/17.
//

#ifndef RTYPE_PROTO_PROTOCOL_HPP
#define RTYPE_PROTO_PROTOCOL_HPP

#include <cstring>
#include <variant>
#include <functional>
#include <array>
#include <utils/Endian.hpp>
#include <utils/Span.hpp>

template <typename Packets>
class Proto
{
public:
    template <typename T>
    static inline constexpr size_t getID() noexcept
    {
        return meta::list::Position<Packets, T>();
    }

    using Buffer = std::vector<std::byte>;
    using BufferSpan = utils::Span<std::byte>;

    using Packet = meta::list::Convert<meta::list::PushFront<Packets, std::monostate>, std::variant>;

    class Formatter
    {
    private:
        Buffer _bytes;

        template <typename T>
        Buffer __serializeType(const T &t) noexcept
        {
            Buffer result(sizeof(T));

            static_assert(std::is_trivially_copyable_v<T>);
            std::memcpy(result.data(), &t, sizeof(T));
            if constexpr (sizeof(T) > 1) {
                if constexpr (utils::has_big_endian()) {
                    std::reverse(result.begin(), result.end());
                }
            }
            return result;
        }

        template <typename Object>
        void __startObject([[maybe_unused]] size_t depth) noexcept
        {
            __addValue(getID<Object>());
        }

        void __finishObject([[maybe_unused]] size_t depth) noexcept
        {
        }

        void __addKey([[maybe_unused]] const std::string_view key, [[maybe_unused]] size_t depth = 0) noexcept
        {
        }

        template <typename T>
        void __addValue(const T &t) noexcept
        {
            auto bytesToAdd = __serializeType(t);
            std::copy(bytesToAdd.begin(), bytesToAdd.end(), std::back_inserter(_bytes));
        }

        void __addValue(const std::string &str) noexcept
        {
            __addValue(str.size());
            auto oldSize = _bytes.size();
            _bytes.resize(oldSize + str.size());
            std::memcpy(_bytes.data() + oldSize, str.data(), str.size());
        }

    public:
        Buffer extract() noexcept
        {
            Buffer extracted = std::move(_bytes);

            _bytes.clear();
            return extracted;
        }

        void prefixSize() noexcept
        {
            auto toAdd = __serializeType(_bytes.size());
            _bytes.insert(_bytes.begin(), toAdd.begin(), toAdd.end());
        }

        template <typename T, typename std::enable_if_t<serialization::is_serializable_v<T>, bool> = true>
        void serialize(const T &t, [[maybe_unused]] size_t depth = 0) noexcept
        {
            if constexpr (serialization::is_basic_v<T>) {
                __addValue(t);
            } else {
                auto visitor = [&](auto &&k, auto &&valMemberPtr) {
                    __addKey(k, depth);
                    serialize(t.*valMemberPtr, depth + 1);
                };

                __startObject<T>(depth);
                meta::for_each(T::serializableFields(), visitor);
                __finishObject(depth);
            }
        }
    };

    class Unformatter
    {
    public:
        class NotEnoughData : public std::exception
        {
        public:
            const char *what() const noexcept override
            {
                return "Not enough data to unserialize";
            }
        };

    private:
        template <typename T>
        static auto __unserialize(BufferSpan &sp)
        {
            using RetType = std::decay_t<T>;

            if (unlikely(sp.size() < sizeof(RetType))) {
                throw NotEnoughData();
            }
            Buffer raw{sp.begin(), sp.begin() + sizeof(T)};

            if constexpr (sizeof(T) > 1) {
                if constexpr (utils::has_big_endian()) {
                    std::reverse(raw.begin(), raw.end());
                }
            }

            RetType ret;
            static_assert(std::is_trivially_copyable_v<RetType>);
            std::memcpy(&ret, sp.data(), sizeof(RetType));
            sp = sp.subspan(sizeof(RetType), sp.size() - sizeof(RetType));
            return ret;
        }

        static std::string __unserializeString(BufferSpan &sp)
        {
            size_t len = __unserialize<size_t>(sp);

            if (unlikely(sp.size() < len)) {
                throw NotEnoughData();
            }
            std::string ret(len, 0);
            std::memcpy(ret.data(), sp.data(), len);
            sp = sp.subspan(len, sp.size() - len);
            return ret;
        }

    public:
        Packet unserialize(BufferSpan data)
        {
            size_t packetID = __unserialize<size_t>(data);

            if (unlikely(packetID >= meta::list::Length<Packets>())) {
                return {};
            }
            return _unserializers[packetID](data);
        }

    private:
        template <typename T>
        static Packet __unserializePacket(BufferSpan data)
        {
            T ret;

            meta::for_each(T::serializableFields(), [&ret, &data]([[maybe_unused]] auto &&k, auto &&valMemberPtr) {
                using MemberType = decltype(ret.*valMemberPtr);

                if constexpr (serialization::is_compound_v<MemberType>) {
                    //Handling this case probably won't be needed for this project
                } else if constexpr (std::is_same_v<std::decay_t<MemberType>, std::string>) {
                    ret.*valMemberPtr = __unserializeString(data);
                } else {
                    ret.*valMemberPtr = __unserialize<decltype(ret.*valMemberPtr)>(data);
                }
            });
            return {std::move(ret)};
        }

        using PacketUnserializerArray = std::array<std::function<Packet(BufferSpan)>, meta::list::Length<Packets>::value>;

        template <typename ...Types>
        static inline constexpr PacketUnserializerArray makeArray(meta::TypeList<Types...>) noexcept
        {
            return {{__unserializePacket<Types>...}};
        }

        const PacketUnserializerArray _unserializers = makeArray(Packets{});
    };
};

#endif //RTYPE_PROTO_PROTOCOL_HPP
