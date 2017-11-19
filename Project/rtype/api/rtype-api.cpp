//
// Created by szterg_r on 22/10/2017.
//

#include <rtype/api/rtype-api.hpp>

namespace rtype
{
    lg::Logger API::log = lg::Logger{"RType-API", lg::Debug};
    web::http::http_request API::request = web::http::http_request();

    pplx::task<void> API::requestLogin(const std::string &username, const std::string &password,
                                       bool &freshAccount, std::error_code &ec) noexcept
    {
        auto url = utility::conversions::to_string_t(cfg::game::apiURL + "user/signin");
        auto &&client = __request(url, {std::make_pair(U("name"), username), std::make_pair(U("password"), password)});
        DEBUG_REQUEST(request);
        return client.request(request).then([&freshAccount, &ec](const web::http::http_response &response) {
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
        auto url = utility::conversions::to_string_t(cfg::game::apiURL + "user");
        web::json::value postData;
        postData["nickname"] = web::json::value::string(utility::conversions::to_string_t(pseudo));
        postData["faction"] = web::json::value::number(factionId);
        web::http::client::http_client client(url);
        request.headers().clear();
        __prepareRequest(request, postData);
        request.headers().add(U("Authorization"), utility::conversions::to_string_t(tokens));
        DEBUG_REQUEST(request);
        return client.request(request).then([&ec](const web::http::http_response &response) {
            if (response.status_code() == web::http::status_codes::OK) {
                return response.extract_json();
            }
            return pplx::task_from_result(web::json::value());
        }).then([&ec](const pplx::task<web::json::value> &previousTask) {
            __create(previousTask, ec);
        });
    }
}

namespace rtype
{
    void API::__prepareRequest(web::http::http_request &request, web::json::value &postData) noexcept
    {
        request.set_method(web::http::methods::POST);
        request.headers().add(U("Content-Type"), U("application/json"));
        request.headers().add(U("X-Hub-Signature"), U("eAVZepqfXsrSW6LVjTuqb3W3CHsf9mAUa5776cGZ2hLGzztK4PAT5gkJE52h"));
        request.set_body(postData.serialize());
    }

    void API::__login(bool &freshAccount, const pplx::task<web::json::value> &task, std::error_code &ec) noexcept
    {
        try {
            auto response = task.get();
            web::json::object obj = response.as_object();
            DEBUG_JSON(response);
            ec = static_cast<RTypeErrorCode>(obj.at(U("payload")).as_integer());

            if (!ec) {
                log(lg::Info) << ec.message() << std::endl;
#if defined(USING_WINDOWS)
                const utility::string_t &wtoken = obj.at(U("content")).as_object().at(U("token")).as_string();
                std::string token = utility::conversions::utf16_to_utf8(wtoken);
#else
                const utility::string_t &token = obj.at(U("content")).as_object().at(U("token")).as_string();
#endif
                log(lg::Info) << "Token Session -> " << token << std::endl;
                freshAccount = obj.at(U("content")).as_object().at(U("new")).as_bool();
                cfg::game::tokenSession.append(token);
            } else {
                log(lg::Warning) << ec.message() << std::endl;
            }
        }
        catch (const web::http::http_exception &e) {
            ec = RTypeErrorCode::InternalError;
            log(lg::Error) << "Http exception occured : " << e.what() << std::endl;
        }
        catch (const std::exception &e) {
            ec = RTypeErrorCode::InternalError;
            log(lg::Error) << "Unexpected exception occurred : " << e.what() << std::endl;
        }
    }

    void API::__create(const pplx::task<web::json::value> &previousTask, std::error_code &ec) noexcept
    {
        try {
            auto response = previousTask.get();
            web::json::object obj = response.as_object();
            DEBUG_JSON(response);
            ec = static_cast<RTypeErrorCode>(obj.at(U("payload")).as_integer());
            if (!ec) {
                log(lg::Info) << ec.message() << std::endl;
            } else {
                log(lg::Warning) << ec.message() << std::endl;
            }
        }
        catch (const web::http::http_exception &e) {
            ec = RTypeErrorCode::InternalError;
            log(lg::Error) << "Http exception occured : " << e.what() << std::endl;
        }
        catch (const std::exception &e) {
            ec = RTypeErrorCode::InternalError;
            log(lg::Error) << "Unexpected exception occurred : " << e.what() << std::endl;
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
        request.headers().clear();
        __prepareRequest(request, postData);
        return client;
    }
}