
#include "GetNode.hpp"
#include <iostream>

GetNode::GetNode(blocksci::Blockchain &chain) : chain(chain) {}

void GetNode::nodeWithAddr(utility::string_t &hash)
{
}
void GetNode::nodeWithTx(utility::string_t &hash)
{
}
void GetNode::setTime(time_t blockTime)
{
  aftertime = blockTime + DTIME / 2;
  beforetime = blockTime - DTIME / 2;
}
bool GetNode::isIn3MonthsTransaction(time_t blockTime)
{
  if (type == "wallet")
  {
    time_t current = std::time(nullptr);
    time_t dest = current - DTIME;
    return blockTime >= dest;
  }
  else
  {
    return blockTime >= beforetime && blockTime <= aftertime;
  }
}

void GetNode::printNode()
{
  if (type == "wallet")
  {
    std::cout << "You Searched 'Wallet' type hash...." << std::endl;
    std::cout << "What you researched : " << hash << std::endl;
  }
  else
  {
    std::cout << "You Searched 'Transaction' type hash...." << std::endl;
    std::cout << "What you researched : " << hash << std::endl;
  }
  std::cout << "Wallets >> " << std::endl;
  for (const auto &v : waddress)
  {
    std::cout << v.toString() << std::endl;
  }
  std::cout << std::endl;
  std::cout << "======================================" << std::endl;
  std::cout << "Transactions >> " << std::endl;
  for (const auto &v : txids)
  {
    std::cout << v.getHash().GetHex() << std::endl;
  }
  std::cout << std::endl;
}