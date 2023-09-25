#define DTIME 7776000

#include <blocksci/blocksci.hpp>
#include <string>
#include <nlohmann/json.hpp>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <ctime>

class GetNode
{
private:
  blocksci::Blockchain &chain;
  time_t aftertime;
  time_t beforetime;

public:
  GetNode() = default;
  GetNode(blocksci::Blockchain &chain);

  std::unordered_set<std::string> nodeWithAddr(utility::string_t &inputdata);
  std::unordered_set<std::string> nodeWithTx(utility::string_t &inputdata);
  bool isInTime(time_t blockTime);
  void setTime(time_t blockTime);
};