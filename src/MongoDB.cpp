#include "MongoDB.hpp"

using json = nlohmann::json;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;

MongoDB::MongoDB(std::string url) : client{mongocxx::uri{url}} {
  db = client["bitcoin"];
  updateCol = db["update"];
  walletCol = db["wallets"];
}

void MongoDB::CreateIndexes() {
  auto result = walletCol.find_one({});
  if (result)
    return;
  else {
    auto index_specification = make_document(kvp("address", 1));
    auto index_option = make_document(kvp("unique", true));
    walletCol.create_index(std::move(index_specification),
                           std::move(index_option));
  }
}

int MongoDB::GetSavedHeight() {
  auto result = updateCol.find_one({});
  if (result) {
    auto height = result->view()["height"];
    return height.get_int32().value;
  } else {
    auto doc = make_document(kvp("height", 0), kvp("time", time(NULL)));
    updateCol.insert_one(doc.view());

    return 0;
  }
}

void MongoDB::UpdateHeight(int saveBlock) {
  auto doc = make_document(kvp("$set", make_document(kvp("height", saveBlock),
                                                     kvp("time", time(NULL)))));
  updateCol.update_many({}, doc.view());
}

json MongoDB::GetWalletData(std::string addr) {
  auto result = walletCol.find_one(make_document(kvp("address", addr)));
  if (result)
    return json::parse(bsoncxx::to_json(*result));
  else {
    return {};
  }
}

void MongoDB::StoreWalletData(json& data) {
  auto document = bsoncxx::from_json(data.dump());
  walletCol.insert_one(document.view());
}

void MongoDB::UpdateWalletData(std::string id, json& updateData) {
  std::string oid_value = id.substr(9, 24);
  walletCol.update_one(
      make_document(kvp("_id", bsoncxx::oid(oid_value))),
      make_document(kvp("$set", bsoncxx::from_json(updateData.dump()))));
}

void MongoDB::Instance() { mongocxx::instance inst{}; }

MongoDB::~MongoDB() { client.reset(); }