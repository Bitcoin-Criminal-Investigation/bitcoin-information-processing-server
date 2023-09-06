#include "Handler.hpp"

#include <iostream>

Handler::Handler(const utility::string_t& url) : http_listener(url) {
  support(std::bind(&Handler::handle_request, this, std::placeholders::_1));
}
Handler::Handler(const utility::string_t& url, http_listener_config& config)
    : http_listener(url, config) {
  support(std::bind(&Handler::handle_request, this, std::placeholders::_1));
}

void Handler::handle_get(const http_request& request,
                         const utility::string_t& path) {
  if (path == U("/info/addr")) {
    auto query_map = uri::split_query(request.relative_uri().query());
    utility::string_t response = U("GET /foo/bar\n");
    for (const auto& pair : query_map) {
      response += pair.first + U(": ") + pair.second + U("\n");
    }
    request.reply(status_codes::OK, response);
  } else if (path == U("/info/txid")) {
    auto query_map = uri::split_query(request.relative_uri().query());
    utility::string_t response = U("GET /foo/bar\n");
    for (const auto& pair : query_map) {
      response += pair.first + U(": ") + pair.second + U("\n");
    }
    request.reply(status_codes::OK, response);
  } else if (path == U("/info/cluster")) {
    auto query_map = uri::split_query(request.relative_uri().query());
    utility::string_t response = U("GET /foo/bar\n");
    for (const auto& pair : query_map) {
      response += pair.first + U(": ") + pair.second + U("\n");
    }
    request.reply(status_codes::OK, response);
  }
}

void Handler::handle_post(const http_request& request,
                          const utility::string_t& path) {
  if (path == U("/foo/bar")) {
    if (request.headers().content_type() == U("application/json")) {
      request.extract_json()
          .then([&request](const json::value& json_val) {
            // Process the JSON content
            // For simplicity, here we just return the JSON back to the client
            request.reply(status_codes::OK, json_val);
          })
          .wait();
    } else {
      request.reply(status_codes::BadRequest, U("Expected JSON content-type."));
    }
  } else if (path == U("/foo")) {
    request.reply(status_codes::OK, U("POST /foo"));
  }
}

void Handler::handle_put(const http_request& request,
                         const utility::string_t& path) {
  if (path == U("/foo/bar")) {
    request.reply(status_codes::OK, U("PUT /foo/bar"));
  } else if (path == U("/foo")) {
    request.reply(status_codes::OK, U("PUT /foo"));
  }
}

void Handler::handle_del(const http_request& request,
                         const utility::string_t& path) {
  if (path == U("/foo/bar")) {
    request.reply(status_codes::OK, U("DELETE /foo/bar"));
  } else if (path == U("/foo")) {
    request.reply(status_codes::OK, U("DELETE /foo"));
  }
}

void Handler::handle_request(http_request request) {
  utility::string_t path = request.relative_uri().path();

  if (request.method() == methods::GET) {
    handle_get(request, path);
  } else if (request.method() == methods::POST) {
    handle_post(request, path);
  } else if (request.method() == methods::PUT) {
    handle_put(request, path);
  } else if (request.method() == methods::DEL) {
    handle_del(request, path);
  } else {
    request.reply(status_codes::NotImplemented, U("Method not supported."));
  }
}

web::json::value Handler::from_string(std::string const& input) {
  utility::stringstream_t s;

  std::wstring wstr;
  wstr.assign(input.begin(), input.end());  // 입력 string -> wstring형변환.

  s << wstr;
  web::json::value ret = web::json::value::parse(
      s);  // 입력받은 Json형식 문자열이 json::value 로 만들어짐.
  return ret;
}

json::value Handler::from_string(std::wstring const& input) {
  utility::stringstream_t s;
  s << input;
  json::value ret = json::value::parse(
      s);  // 입력받은 Json형식 문자열이 json::value 로 만들어짐.
  return ret;
}
