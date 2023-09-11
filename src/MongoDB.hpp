#ifndef MONGODB_HPP
#define MONGODB_HPP
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
class MongoDB
{
public:
  MongoDB(const std::string &url);
  ~MongoDB();
  static void Instance();
  json getProfile(std::string target);
  // void CreateIndexes();
  // void UpdateHeight(int);
  // int GetSavedHeight();
  // nlohmann::json GetWalletData(std::string);
  // void StoreWalletData(nlohmann::json &);
  // void UpdateWalletData(std::string, nlohmann::json &);

private:
  mongocxx::client client;
  mongocxx::database db;
  mongocxx::collection profiles;
  mongocxx::collection flags;
  mongocxx::collection clusters;
};

#endif