#ifndef PROCESSAPI_HPP
#define PROCESSAPI_HPP
#include <blocksci/blocksci.hpp>
#include <nlohmann/json.hpp>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
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

private:
  blocksci::Blockchain &chain;
  BitcoinCore &bitcoinCore;
  MongoDB &mongo;
  json MakeInputData(blocksci::Input input);
  json MakeOutputData(blocksci::Output output);
  std::string onlyAddress(const std::string &fullString);
};

#endif
