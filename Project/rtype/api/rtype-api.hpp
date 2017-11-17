//
// Created by szterg_r on 20/10/2017.
//

#ifndef RTYPE_RTYPE_API_HPP
#define RTYPE_RTYPE_API_HPP

#include <memory>
#include <vector>
#include <utility>
#include <locale>
#include <codecvt>
#include <system_error>
#include <string_view>

#undef always_inline
#include <cpprest/json.h>
#include <cpprest/http_client.h>
#include <cpprest/asyncrt_utils.h>
#include <log/Logger.hpp>
#include <rtype/error/RTypeError.hpp>
#include <rtype/gutils/manager/EventManager.hpp>

namespace rtype
{
    namespace lg = logging;

    class API
    {
    public:
        static void shuttingDownAPI() noexcept;
        static pplx::task<void> requestLogin(std::string const &username, std::string const &password,
                                             bool &freshAccount, std::error_code &ec) noexcept;
        static pplx::task<void> requestCreatePlayer(std::string const &pseudo,
                                                    std::string const &tokens, std::error_code &ec) noexcept;

    private:
        using PostData = std::vector<std::pair<utility::string_t, std::string>>;
    private:
        static web::http::client::http_client __request(const utility::string_t &url, PostData datas) noexcept;
        static void __prepareRequest(web::http::http_request &request, web::json::value &postData) noexcept;
        static void __debugJSON(const web::json::value &value) noexcept;
        static void __login(bool &freshAccount, const pplx::task<web::json::value> &task, std::error_code &ec) noexcept;
        static void __create(const pplx::task<web::json::value> &task, std::error_code &code) noexcept;

    private:
        static lg::Logger log;
        static web::http::http_request request;

    private:
        API() = default;
    };
}

#endif //RTYPE_RTYPE_API_HPP
