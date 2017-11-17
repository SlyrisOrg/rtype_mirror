//
// Created by milerius on 06/11/17.
//

#ifndef RTYPE_CONFIGURATIONMACROS_HPP
#define RTYPE_CONFIGURATIONMACROS_HPP

#define RESOURCE_ADD(enumname, ...)                                                                 \
ENUM(enumname, __VA_ARGS__)                                                                         \


/**
 * @details
 * The 3 macros below are not merged into one for the sake of readability,
 * the person who reads the code can therefore observe that the 3 macros do not have the same log messages.
 * I could take as a parameter part of the message of log,
 * but it would be less readable for someone who should read the code.
 * The name of macros determine which type of resources we are loading.
 *
 * @author Roman Sztergbaum
 */

#define LOAD_RESOURCE(enumname, handle, value, cfg)                                                 \
    for (const auto &cur : enumname::values()) {                                                    \
            (handle).load(value, utils::unpackToString(cfg, value.toString(), ".png"));             \
            log(logging::Debug) << "Loading textures " << (value).toString() + ".png" <<            \
            " successfully loaded." << std::endl;                                                   \
        }

#define LOAD_MUSIC(enumname, handle, value, type, cfg)                                              \
    for (const auto &cur : enumname::values()) {                                                    \
            (handle).load(value, utils::unpackToString(cfg, (value).toString(), ".ogg"));           \
            log(logging::Debug) << "Loading " << type << " " << (value).toString() + ".ogg" <<      \
            " successfully loaded." << std::endl;                                                   \
        }

#define LOAD_VIDEO(enumname, handle, value)                                                            \
    for (const auto &cur : enumname::values()) {                                                       \
            (handle).load(value, utils::unpackToString(cfg::videoPath, (value).toString(), ".avi"));   \
            log(logging::Debug) << "Loading video "  << (value).toString() + ".avi" <<                 \
            " successfully loaded." << std::endl;                                                      \
        }

#endif //RTYPE_CONFIGURATIONMACROS_HPP
