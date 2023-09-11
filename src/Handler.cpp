#include "Handler.hpp"

#include <iostream>

Handler::Handler(const utility::string_t &url, MongoDB &mongo,
                 BitcoinCore &bitcoinCore,
                 blocksci::Blockchain &chain)
    : http_listener(url), processApi(mongo, bitcoinCore, chain)
{
  support(std::bind(&Handler::handle_request, this, std::placeholders::_1));
}

Handler::Handler(const utility::string_t &url, MongoDB &mongo,
                 BitcoinCore &bitcoinCore,
                 blocksci::Blockchain &chain,
                 http_listener_config &config)
    : http_listener(url, config), processApi(mongo, bitcoinCore, chain)
{
  support(std::bind(&Handler::handle_request, this, std::placeholders::_1));
}

void Handler::handle_get(const http_request &request,
                         const utility::string_t &path)
{
  json::value response;
  if (path == U("/info/addr"))
  {
    auto query_map = uri::split_query(request.relative_uri().query());
    auto find = query_map.find(U("hash"));
    if (query_map.end() != find)
    {
      utility::string_t hash = find->second;
      try
      {
        std::string raw = processApi.getTxData(hash);
        response = from_string(raw);
      }
      catch (std::exception e)
      {
        request.reply(status_codes::BadRequest);
      }
    }
    else
    {
      request.reply(status_codes::BadRequest);
    }
  }
  else if (path == U("/info/txid"))
  {
    auto query_map = uri::split_query(request.relative_uri().query());
    auto find = query_map.find(U("hash"));
    if (query_map.end() != find)
    {
      utility::string_t hash = find->second;
      try
      {
        std::string test = processApi.getTxData(hash);
        response = from_string(test);
      }
      catch (std::exception e)
      {
        request.reply(status_codes::BadRequest);
      }
    }
    else
    {
      request.reply(status_codes::BadRequest);
    }
  }
  else if (path == U("/info/cluster"))
  {
    auto query_map = uri::split_query(request.relative_uri().query());
    auto find = query_map.find(U("hash"));
    if (query_map.end() != find)
    {
      utility::string_t hash = find->second;
      try
      {
        std::string test = processApi.getTxData(hash);
        response = from_string(test);
      }
      catch (std::exception e)
      {
        request.reply(status_codes::BadRequest);
      }
    }
    else
    {
      request.reply(status_codes::BadRequest);
    }
  }
  request.reply(status_codes::OK, response);
}

void Handler::handle_post(const http_request &request,
                          const utility::string_t &path)
{
  if (path == U("/foo/bar"))
  {
    if (request.headers().content_type() == U("application/json"))
    {
      request.extract_json()
          .then([&request](const json::value &json_val)
                {
            // Process the JSON content
            // For simplicity, here we just return the JSON back to the client
            request.reply(status_codes::OK, json_val); })
          .wait();
    }
    else
    {
      request.reply(status_codes::BadRequest, U("Expected JSON content-type."));
    }
  }
  else if (path == U("/foo"))
  {
    request.reply(status_codes::OK, U("POST /foo"));
  }
}

void Handler::handle_put(const http_request &request,
                         const utility::string_t &path)
{
  if (path == U("/foo/bar"))
  {
    request.reply(status_codes::OK, U("PUT /foo/bar"));
  }
  else if (path == U("/foo"))
  {
    request.reply(status_codes::OK, U("PUT /foo"));
  }
}

void Handler::handle_del(const http_request &request,
                         const utility::string_t &path)
{
  if (path == U("/foo/bar"))
  {
    request.reply(status_codes::OK, U("DELETE /foo/bar"));
  }
  else if (path == U("/foo"))
  {
    request.reply(status_codes::OK, U("DELETE /foo"));
  }
}

void Handler::handle_request(http_request request)
{
  utility::string_t path = request.relative_uri().path();

  if (request.method() == methods::GET)
  {
    handle_get(request, path);
  }
  else if (request.method() == methods::POST)
  {
    handle_post(request, path);
  }
  else if (request.method() == methods::PUT)
  {
    handle_put(request, path);
  }
  else if (request.method() == methods::DEL)
  {
    handle_del(request, path);
  }
  else
  {
    request.reply(status_codes::NotImplemented, U("Method not supported."));
  }
}

json::value Handler::from_string(const std::string &input)
{
  return json::value::parse(utility::conversions::to_string_t(input));
}

json::value Handler::from_string_t(const utility::string_t &input)
{
  return json::value::parse(input);
}
