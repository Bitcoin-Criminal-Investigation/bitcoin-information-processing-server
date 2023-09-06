#ifndef HANDLER_HPP
#define HANDLER_HPP
#include <cpprest/http_listener.h>
#include <cpprest/json.h>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

class Handler : public http_listener {
 public:
  Handler() = default;
  Handler(const utility::string_t& url);
  Handler(const utility::string_t& url, http_listener_config& config);

 private:
  void handle_get(const http_request& request, const utility::string_t& path);
  void handle_put(const http_request& request, const utility::string_t& path);
  void handle_post(const http_request& request, const utility::string_t& path);
  void handle_del(const http_request& request, const utility::string_t& path);
  void handle_request(http_request request);
  json::value from_string(std::string const& input);
  json::value from_string(std::wstring const& input);
};

#endif