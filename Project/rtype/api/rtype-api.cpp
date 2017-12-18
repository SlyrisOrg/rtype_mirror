//
// Created by szterg_r on 22/10/2017.
//

#include <rtype/config/ProfilConfig.hpp>
#include <rtype/api/rtype-api.hpp>

namespace rtype
{
    lg::Logger API::_log = lg::Logger{"RType-API", lg::Debug};
    web::http::http_request API::_request = web::http::http_request();

    pplx::task<void> API::requestLogin(const std::string &username, const std::string &password,
                                       bool &freshAccount, std::error_code &ec) noexcept
    {
        const auto url = utility::conversions::to_string_t(cfg::game::apiURL + "user/signin");
        auto &&client = __request(url, {std::make_pair(U("name"), username), std::make_pair(U("password"), password)});
        __debugRequest(_request);
        return client.request(_request).then([&freshAccount, &ec](const web::http::http_response &response) {
            if (response.status_code() == web::http::status_codes::OK) {
                return response.extract_json();
            }
            return pplx::task_from_result(web::json::value());
        }).then([&freshAccount, &ec](const pplx::task<web::json::value> &previousTask) {
            __login(freshAccount, previousTask, ec);
        });
    }

    pplx::task<void> API::requestCreatePlayer(const std::string &pseudo,
                                              unsigned int factionId,
                                              const std::string &tokens,
                                              std::error_code &ec) noexcept
    {
        const auto url = utility::conversions::to_string_t(cfg::game::apiURL + "user");
        web::json::value postData;
        postData[U("nickname")] = web::json::value::string(utility::conversions::to_string_t(pseudo));
        postData[U("faction")] = web::json::value::number(factionId);
        web::http::client::http_client client(url);
        _request.headers().clear();
        __prepareRequest(_request, postData);
        _request.headers().add(U("Authorization"), utility::conversions::to_string_t(tokens));
        __debugRequest(_request);
        return client.request(_request).then([&ec](const web::http::http_response &response) {
            if (response.status_code() == web::http::status_codes::OK) {
                return response.extract_json();
            }
            return pplx::task_from_result(web::json::value());
        }).then([&ec](const pplx::task<web::json::value> &previousTask) {
            __create(previousTask, ec);
        });
    }

    pplx::task<void> API::requestGetData(const std::string &tokens, std::error_code &ec) noexcept
    {
        const auto url = utility::conversions::to_string_t(cfg::game::apiURL + "user");
        web::http::client::http_client client(url);
        _request.headers().clear();
        __prepareRequest(_request, web::http::methods::GET);
        _request.headers().add(U("Authorization"), utility::conversions::to_string_t(tokens));
        __debugRequest(_request);
        return client.request(_request).then([&ec](const web::http::http_response &response) {
            if (response.status_code() == web::http::status_codes::OK) {
                return response.extract_json();
            }
            return pplx::task_from_result(web::json::value());
        }).then([&ec](const pplx::task<web::json::value> &previousTask) {
            __getData(previousTask, ec);
        });
    }

    void API::__prepareRequest(web::http::http_request &request, const web::http::method &method) noexcept
    {
        request.set_method(method);
        request.headers().add(U("Content-Type"), U("application/json"));
        request.headers().add(U("X-Hub-Signature"), U("eAVZepqfXsrSW6LVjTuqb3W3CHsf9mAUa5776cGZ2hLGzztK4PAT5gkJE52h"));
    }

    void API::__prepareRequest(web::http::http_request &request, web::json::value &postData,
                               const web::http::method &method) noexcept
    {
        __prepareRequest(request, method);
        request.set_body(postData.serialize());
    }

    void API::__login(bool &freshAccount, const pplx::task<web::json::value> &task, std::error_code &ec) noexcept
    {
        try {
            auto response = task.get();
            web::json::object obj = response.as_object();
            __debugJson(response);
            ec = static_cast<RTypeErrorCode>(obj.at(U("payload")).as_integer());

            if (!ec) {
                _log(lg::Info) << ec.message() << std::endl;
#if defined(USING_WINDOWS)
                const utility::string_t &wtoken = obj.at(U("content")).as_object().at(U("token")).as_string();
                std::string token = utility::conversions::utf16_to_utf8(wtoken);
#else
                const utility::string_t &token = obj.at(U("content")).as_object().at(U("token")).as_string();
#endif
                _log(lg::Info) << "Token Session -> " << token << std::endl;
                freshAccount = obj.at(U("content")).as_object().at(U("new")).as_bool();
                cfg::game::tokenSession = "JWT ";
                cfg::game::tokenSession.append(token);
            } else {
                _log(lg::Warning) << ec.message() << std::endl;
            }
        }
        catch (const web::http::http_exception &e) {
            ec = RTypeErrorCode::InternalError;
            _log(lg::Error) << "Http exception occured : " << e.what() << std::endl;
        }
        catch (const std::exception &e) {
            ec = RTypeErrorCode::InternalError;
            _log(lg::Error) << "Unexpected exception occurred : " << e.what() << std::endl;
        }
    }

    void API::__create(const pplx::task<web::json::value> &previousTask, std::error_code &ec) noexcept
    {
        try {
            auto response = previousTask.get();
            web::json::object obj = response.as_object();
            __debugJson(response);
            ec = static_cast<RTypeErrorCode>(obj.at(U("payload")).as_integer());
            if (!ec) {
                _log(lg::Info) << ec.message() << std::endl;
            } else {
                _log(lg::Warning) << ec.message() << std::endl;
            }
        }
        catch (const web::http::http_exception &e) {
            ec = RTypeErrorCode::InternalError;
            _log(lg::Error) << "Http exception occured : " << e.what() << std::endl;
        }
        catch (const std::exception &e) {
            ec = RTypeErrorCode::InternalError;
            _log(lg::Error) << "Unexpected exception occurred : " << e.what() << std::endl;
        }
    }

    void API::__getData(const web::json::object &response) noexcept
    {
#if defined(USING_WINDOWS)
        const utility::string_t &wnickname = response.at(U("content")).as_object().at(U("nickname")).as_string();
        std::string nickname = utility::conversions::utf16_to_utf8(wnickname);
#else
        const utility::string_t &nickname = response.at(U("content")).as_object().at(U("nickname")).as_string();
#endif
        const auto &profile = response.at(U("content")).as_object().at(
            U("profile")).as_object();
        cfg::profil::xp = static_cast<float>(profile.at(U("experience")).as_double());
        cfg::profil::gold = std::to_string(profile.at(U("gold")).as_integer());
        cfg::profil::faction = static_cast<cfg::profil::FactionT>(profile.at(U("faction")).as_integer());
        cfg::profil::nickname = nickname;
    }

    void API::__getData(const pplx::task<web::json::value> &task, std::error_code &ec) noexcept
    {
        try {
            auto response = task.get();
            web::json::object obj = response.as_object();
            __debugJson(response);
            ec = static_cast<RTypeErrorCode>(obj.at(U("payload")).as_integer());
            if (!ec) {
                _log(lg::Info) << ec.message() << std::endl;
                __getData(obj);
            } else {
                _log(lg::Warning) << ec.message() << std::endl;
            }
        }
        catch (const web::http::http_exception &e) {
            ec = RTypeErrorCode::InternalError;
            _log(lg::Error) << "Http exception occured : " << e.what() << std::endl;
        }
        catch (const std::exception &e) {
            ec = RTypeErrorCode::InternalError;
            _log(lg::Error) << "Unexpected exception occurred : " << e.what() << std::endl;
        }
    }

    web::http::client::http_client API::__request(const utility::string_t &url, PostData datas) noexcept
    {
        web::json::value postData;
        for (auto &&cur : datas) {
            auto[first, second] = cur;
            postData[first] = web::json::value::string(utility::conversions::to_string_t(second));
        }
        web::http::client::http_client client(url);
        _request.headers().clear();
        __prepareRequest(_request, postData);
        return client;
    }

    void API::__debugJson(const web::json::value &value) noexcept
    {
        using namespace utility::conversions;
#if defined(USING_WINDOWS)
        _log(lg::Debug) << "Http response receive ->\n" << utf16_to_utf8(value.serialize()) << std::endl;
#else
        _log(lg::Debug) << "Http response receive ->\n" << to_string_t(value.serialize()) << std::endl;
#endif
    }

    void API::__debugRequest(const web::http::http_request &request) noexcept
    {
#if defined(USING_WINDOWS)
        using namespace utility::conversions;
        _log(lg::Debug) << "Http request send ->\n" << utf16_to_utf8(request.to_string()) << std::endl;
#else
        _log(lg::Debug) << "Http request send ->\n" << request.to_string() << std::endl;
#endif
    }
}
