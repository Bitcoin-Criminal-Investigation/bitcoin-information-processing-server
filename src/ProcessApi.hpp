#ifndef PROCESSAPI_HPP
#define PROCESSAPI_HPP
#include <blocksci/blocksci.hpp>
#include <nlohmann/json.hpp>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <future>
#include <mutex>
#include <atomic>
#include <thread>
#include <chrono>
#include <thread>
#include <ctime>
#include <regex>
#include "MongoDB.hpp"
#include "BitcoinCore.hpp"
using json = nlohmann::json;

class ProcessApi
{
public:
  ProcessApi() = default;
  ProcessApi(MongoDB &mongo, BitcoinCore &bitcoinCore,
             blocksci::Blockchain &chain);
  std::string getTxData(const utility::string_t &input);
  std::string getWalletData(const utility::string_t &req);
  std::string getClusterData(const utility::string_t &req);

private:
  blocksci::Blockchain &chain;
  BitcoinCore &bitcoinCore;
  MongoDB &mongo;
  json MakeInputData(blocksci::Input input);
  json MakeOutputData(blocksci::Output output);
  std::string onlyAddress(const std::string &fullString);
  void addTxToVector(json &tx, std::vector<json> &vec, std::mutex &vec_mutex);
};

#endif