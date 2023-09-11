#ifndef HANDLER_HPP
#define HANDLER_HPP

#include "ProcessApi.hpp"

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

class Handler : public http_listener
{
public:
        Handler() = default;
        Handler(const utility::string_t &url, MongoDB &mongo,
                BitcoinCore &bitcoinCore, blocksci::Blockchain &chain);
        Handler(const utility::string_t &url, MongoDB &mongo,
                BitcoinCore &bitcoinCore, blocksci::Blockchain &chain,
                http_listener_config &config);

private:
        ProcessApi processApi;
        void handle_get(const http_request &request, const utility::string_t &path);
        void handle_put(const http_request &request, const utility::string_t &path);
        void handle_post(const http_request &request, const utility::string_t &path);
        void handle_del(const http_request &request, const utility::string_t &path);
        void handle_request(http_request request);
        json::value from_string(const std::string &input);
        json::value from_string_t(const utility::string_t &input);
};

#endif