#include "Handler.hpp"

#include <iostream>

// Handler::Handler(const utility::string_t &url, blocksci::Blockchain &chain)
//     : http_listener(url), processApi(chain)
// {
//   support(std::bind(&Handler::handle_request, this, std::placeholders::_1));
// }

Handler::Handler(const utility::string_t &url,
                 blocksci::Blockchain &chain, const std::string &mongoUri)
    : http_listener(url), processApi(chain, mongoUri)
{
  support(std::bind(&Handler::handle_request, this, std::placeholders::_1));
}

Handler::Handler(const utility::string_t &url, blocksci::Blockchain &chain,
                 http_listener_config &config, const std::string &mongoUri)
    : http_listener(url, config), processApi(chain, mongoUri)
{
  support(std::bind(&Handler::handle_request, this, std::placeholders::_1));
}

/* GET Method 처리 */
void Handler::handle_get(const http_request &request, const utility::string_t &path)
{
  json::value response;
  utility::string_t query_param;
  std::string raw;

  auto query_map = uri::split_query(request.relative_uri().query());

  if (path == U("/info/addr"))
  {
    query_param = U("hash");
  }
  else if (path == U("/info/txid"))
  {
    query_param = U("hash");
  }
  else if (path == U("/info/cluster"))
  {
    query_param = U("target");
  }
  else
  {
    request.reply(status_codes::NotFound);
    return;
  }

  auto find = query_map.find(query_param);
  if (query_map.end() != find)
  {
    utility::string_t value = find->second;
    try
    {
      if (path == U("/info/addr"))
      {
        raw = processApi.getWalletData(value);
      }
      else if (path == U("/info/txid"))
      {
        raw = processApi.getTxData(value);
      }
      else if (path == U("/info/cluster"))
      {
        raw = processApi.getClusterData(value);
      }
      response = from_string(raw);
    }
    catch(const std::runtime_error& e)
    {
      request.reply(status_codes::BadRequest, U(e.what()));
      return;
    }
  }
  else
  {
    request.reply(status_codes::BadRequest, U("Query string not found."));
    return;
  }

  request.reply(status_codes::OK, response);
}

/* POST Method 처리 */
void Handler::handle_post(const http_request &request,
                          const utility::string_t &path)
{
  if (path == U("/info/addr"))
  {
    if (request.headers().content_type() != U("application/json"))
    {
      request.reply(status_codes::BadRequest, U("Expected JSON content-type."));
      return;
    }

    request.extract_json()
        .then([this, request](json::value json_val) -> pplx::task<void> // 반환 타입을 명시적으로 지정
              {
                  std::string hash;
                  time_t startDate, endDate;

                  if(!json_val.has_field(U("hash")) || !json_val[U("hash")].is_string())
                  {
                    return request.reply(status_codes::BadRequest, U("Missing or invalid 'hash'."));
                  }
                  
                  hash = json_val[U("hash")].as_string();

                  startDate = json_val.has_field(U("start_date")) && json_val[U("start_date")].is_integer() 
                              ? static_cast<time_t>(json_val[U("start_date")].as_integer()) 
                              : 1230940800;

                  endDate = json_val.has_field(U("end_date")) && json_val[U("end_date")].is_integer()
                            ? static_cast<time_t>(json_val[U("end_date")].as_integer()) 
                            : std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

                  try 
                  {
                    std::string raw = this->processApi.getTxInWallet(hash, startDate, endDate);
                    json::value response = this->from_string(raw);
                    return request.reply(status_codes::OK, response);
                  } 
                  catch(const std::runtime_error& e) 
                  {
                    return request.reply(status_codes::BadRequest, U(e.what()));
                  } });
  }
  else if (path == U("/foo"))
  {
    request.reply(status_codes::OK, U("POST /foo"));
  }
}

/* PUT Method 처리 */
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

/* DEL Method 처리 */
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
