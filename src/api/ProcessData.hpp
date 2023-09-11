#ifndef PROCESSDATA_HPP
#define PROCESSDATA_HPP
#include "../BitcoinCore.hpp"
#include "../MongoDB.hpp"

class ProcessData
{
public:
  ProcessData(BitcoinCore &, MongoDB &);
  void ProcessBlock(int);

private:
  int height;
  BitcoinCore &bitcoinCore;
  MongoDB &mongo;
  void ProcessTx(std::string, nlohmann::json &);
  // json MakeNewInputWallet(string &, json &, json &, json &, int);
  void StoreNewWallet(std::string &, nlohmann::json &, nlohmann::json &);
  void UpdateWallet(nlohmann::json &, nlohmann::json &, nlohmann::json &);
  nlohmann::json MakeInputData(nlohmann::json &, int);
  nlohmann::json MakeOutputData(nlohmann::json &);
  nlohmann::json GetPrevData(std::string, int);
  nlohmann::json MakeTxData(std::string &, nlohmann::json &, nlohmann::json &,
                            long long = 0);
  void InputSpentData(nlohmann::json &, std::string, int, std::string, int);
};

#endif