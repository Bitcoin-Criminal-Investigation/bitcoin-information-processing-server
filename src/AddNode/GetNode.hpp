
#include <blocksci/blocksci.hpp>
#include <string>
#include <nlohmann/json.hpp>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <unordered_set>
#include <vector>
#include <algorithm>

class GetNode
{
private:
  blocksci::Blockchain &chain;
  time_t aftertime;
  time_t beforetime;

public:
  GetNode() = default;
  GetNode(blocksci::Blockchain &chain);

  void nodeWithAddr(utility::string_t &hash);
  void nodeWitTx(utility::string_t &hash);
  bool isInTime(time_t &blockTime);
  void setTime(time_t &blockTime);
  void printNode();
}