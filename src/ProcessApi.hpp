#ifndef PROCESSAPI_HPP
#define PROCESSAPI_HPP
#include <blocksci/blocksci.hpp>
#include <nlohmann/json.hpp>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <regex>
#include <deque>
#include "MongoDB.hpp"
using json = nlohmann::json;

class ProcessApi
{
public:
  ProcessApi() = default;
  ProcessApi(blocksci::Blockchain &chain, const std::string &mongoUri);
  std::string getTxData(const utility::string_t &input);
  std::string getWalletData(const utility::string_t &req);
  std::string getTxInWallet(const std::string &hash, const time_t &startDate, const time_t &endDate);
  std::string getClusterData(const utility::string_t &req);

private:
  blocksci::Blockchain &chain;
  const std::string &mongoUri;
  json MakeInputData(blocksci::Input input);
  json MakeOutputData(blocksci::Output output);
  std::string onlyAddress(const std::string &fullString);
};

#endif