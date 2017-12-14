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

    /**
     * @class API
     * @note
     * The class Api, is a static class, which allows to communicate with our API js which is on rtype.slyris.eu, 
     * the class is not instantiable.
     * It uses the cpprestsdk external library for communication that works asynchronously.
     */
    class API
    {
    public:
        /**
         * @brief The static function requestLogin allows you to connect to the game through an account name and password
         * @param username name of the account
         * @param password password of the account
         * @param freshAccount Boolean to find out if the pilot of the spaceship has already been created or not.
         * @param ec An error_code that can contain the error message in case of connection failure
         */
        static pplx::task<void> requestLogin(const std::string &username, const std::string &password,
                                             bool &freshAccount, std::error_code &ec) noexcept;

        /**
         * @brief The static function requestCreatePlayer allow you to create a pilot with his own faction.
         * @param pseudo name of the pilot
         * @param factionId represents the id of the faction, can take for value:
         * - Bheet (Offensive Faction)
         * - Kooy (Faction Support)
         * - Maul (Defensive Faction)
         * @param tokens The token, is the current JWT of the connected player.
         * @param ec An error_code that can contain the error message in case of pilote creation failure.
         */
        static pplx::task<void> requestCreatePlayer(const std::string &pseudo,
                                                    unsigned int factionId,
                                                    const std::string &tokens, std::error_code &ec) noexcept;

        /**
         * @brief The static function requestGetData allow you to get the data information about a connected player.
         * @param tokens The token, is the current JWT of the connected player.
         * @param ec An error_code that can contain the error message in case we couldn't get player data.
         * @details The player data contains several information about his account : level, gold, experience, faction, nickname, etc...
         */
        static pplx::task<void> requestGetData(const std::string &tokens, std::error_code &ec) noexcept;

    private:
        /**
         * @typedef Postdata
         * @details typedef which represents the Data of the POST, for an HTTP request.
         */
        using PostData = std::vector<std::pair<utility::string_t, std::string>>;
    private:
        /**
         * @internal internal function for all request
         * @details clear the http header, and prepare the request.
         * @param url url to the api
         * @param datas post data
         * @return http_client
         */
        static web::http::client::http_client __request(const utility::string_t &url, PostData datas) noexcept;

        /**
         * @internal internal function for preparing a request
         * @brief fill the http header of the request.
         * @param request the current http request
         * @param postData post data
         * @param method type of http request
         */
        static void __prepareRequest(web::http::http_request &request, web::json::value &postData,
                                     const web::http::method &method = web::http::methods::POST) noexcept;

        static void __prepareRequest(web::http::http_request &request, const web::http::method &method) noexcept;
        static void __getData(const web::json::object &response) noexcept;
        static void __getData(const pplx::task<web::json::value> &task, std::error_code &ec) noexcept;
        static void __login(bool &freshAccount, const pplx::task<web::json::value> &task, std::error_code &ec) noexcept;
        static void __create(const pplx::task<web::json::value> &previousTask, std::error_code &ec) noexcept;
        static void __debugJson(const web::json::value &value) noexcept;
        static void __debugRequest(const web::http::http_request &request) noexcept;
    private:
        static lg::Logger _log;
        static web::http::http_request _request;

    private:
        API() = default;
    };
}

#endif //RTYPE_RTYPE_API_HPP
