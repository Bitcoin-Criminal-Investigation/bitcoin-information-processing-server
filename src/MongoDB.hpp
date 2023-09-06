#ifndef MONGODB_HPP
#define MONGODB_HPP
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <nlohmann/json.hpp>

class MongoDB {
 public:
  MongoDB(std::string);
  ~MongoDB();
  void CreateIndexes();
  static void Instance();
  void UpdateHeight(int);
  int GetSavedHeight();
  nlohmann::json GetWalletData(std::string);
  void StoreWalletData(nlohmann::json &);
  void UpdateWalletData(std::string, nlohmann::json &);

 private:
  mongocxx::client client;
  mongocxx::database db;
  mongocxx::collection updateCol;
  mongocxx::collection walletCol;
};

#endif