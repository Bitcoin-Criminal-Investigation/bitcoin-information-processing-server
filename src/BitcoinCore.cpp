#include "BitcoinCore.hpp"

#include <cmath>
#include <iostream>

using json = nlohmann::json;

BitcoinCore::BitcoinCore(std::string userpass) : userpass(userpass) {}

int BitcoinCore::GetHeight() {
  json rawData = Send("getchaintips");
  return rawData[0]["height"];
}
std::string BitcoinCore::GetBlockHash(const int height) {
  return Send("getblockhash", std::to_string(height));
}
json BitcoinCore::GetBlock(const std::string &blockhash, int verbosity) {
  return Send("getblock",
              "\"" + blockhash + "\", " + std::to_string(verbosity));
}
json BitcoinCore::GetRawTransaction(const std::string &txid) {
  json rawData = Send("getrawtransaction", txid + ", true");
  long long value;
  for (auto &vout : rawData["vout"]) {
    value = (double)vout["value"] * pow(10, 9);
    vout["value"] = value;
  }
  return rawData;
}

size_t BitcoinCore::WriteCallback(void *contents, size_t size, size_t nmemb,
                                  void *userp) {
  ((std::string *)userp)->append((char *)contents, size * nmemb);
  return size * nmemb;
}

json BitcoinCore::Send(const std::string &method, const std::string &params) {
  std::string data = R"({
        "jsonrpc": "1.0",
        "id": 1,
        "method": ")" +
                     method + R"(",
        "params": [)" +
                     params + R"(]
    })";

  return SendCurl(data);
}

json BitcoinCore::Send(const std::string &method) {
  std::string data = R"({
        "jsonrpc": "1.0",
        "id": 1,
        "method": ")" +
                     method + R"("
    })";

  return SendCurl(data);
}

json BitcoinCore::SendCurl(const std::string &data) {
  CURL *curl = curl_easy_init();
  std::string readBuffer;

  if (curl) {
    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "content-type: text/plain;");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_USERPWD, userpass.c_str());
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.length());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res)
                << std::endl;
    }

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
  }
  json jsonRes = json::parse(readBuffer);
  return jsonRes["result"];
}
