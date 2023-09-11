#define DTIME 7776000

#include <string>
#include <blocksci/blocksci.hpp>
#include <unordered_set>

// const std::unordered_set<std::string> walletinfo;
// const std::vector<std::string> txinfo;

class getNode{
  private :
  std::unordered_set<blocksci::Address> waddress;
  std::vector<blocksci::Transaction> txid;
  public :
  getNode(const std::string& target, const std::string& type);
  getWalletInfo(const std::string& address, const std::string& pre_info);
  getTxInfo(const std::string& txid);
}
