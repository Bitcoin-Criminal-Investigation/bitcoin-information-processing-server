#ifndef BITCOINCORE_HPP
#define BITCOINCORE_HPP
#include <curl/curl.h>

#include <nlohmann/json.hpp>

class BitcoinCore {
 public:
  BitcoinCore(std::string);
  int GetHeight();
  nlohmann::json GetBlock(const std::string &, int = 1);
  std::string GetBlockHash(const int);
  nlohmann::json GetRawTransaction(const std::string &);

 private:
  std::string userpass;
  std::string url = "http://localhost:8332";

  nlohmann::json Send(const std::string &, const std::string &);
  nlohmann::json Send(const std::string &);
  nlohmann::json SendCurl(const std::string &);

  static size_t WriteCallback(void *, size_t, size_t, void *);
};

#endif
