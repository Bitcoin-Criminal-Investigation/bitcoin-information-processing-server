
#include "GetNode.hpp"
#include <iostream>

int main()
{
  blocksci::Blockchain chain("/home/bitcoin-core/.blocksci/config.json");
  utility::string_t hash = U("bc1qwqdg6squsna38e46795at95yu9atm8azzmyvckulcc7kytlcckxswvvzej");
  clock_t start, end;
  start = clock();
  GetNode test(chain);
  std::unordered_set<std::string> result = test.nodeWithAddr(hash);

  end = clock();
  double exeT = (double)(end - start) / CLOCKS_PER_SEC;
  std::cout << "Total Time : " << exeT << std::endl;
  return 0;
}

GetNode::GetNode(blocksci::Blockchain &chain) : chain(chain) {}

std::unordered_set<std::string> GetNode::nodeWithAddr(utility::string_t &inputdata)
{
  std::string hash = utility::conversions::to_utf8string(inputdata);
  auto address = blocksci::getAddressFromString(hash, chain.getAccess());
  if (!address)
  {
    throw std::runtime_error("Invalid Address");
  }
  aftertime = beforetime = 0;
  std::unordered_set<std::string> result;
  result.insert(hash);
  clock_t start, end;
  int cnt = 0;
  int ccnt = 0;
  start = clock();
  for (const auto &tx : address->getTransactions())
  {
    cnt++;
    if (isInTime(tx.block().timestamp()))
    {
      // std::cout << tx.getHash().GetHex() << std::endl;
      ccnt++;
      result.insert(tx.getHash().GetHex());
    }
  }
  end = clock();
  double exeTime = (double)(end - start) / CLOCKS_PER_SEC;
  std::cout << "total txs : " << cnt << std::endl;
  std::cout << "in time txs : " << ccnt << std::endl;
  std::cout << "Execution Time : " << exeTime << std::endl;
  return result;
}

std::unordered_set<std::string> GetNode::nodeWithTx(utility::string_t &inputdata)
{
  std::string hash = utility::conversions::to_utf8string(inputdata);
  try
  {
    blocksci::Transaction tx(hash, chain.getAccess());
    setTime(tx.block().timestamp());

    std::unordered_set<std::string> result;

    return result;
  }
  catch (const std::exception &e)
  {
    throw e;
  }
}

void GetNode::setTime(time_t blockTime)
{
  aftertime = blockTime + DTIME / 2;
  beforetime = blockTime - DTIME / 2;
}
bool GetNode::isInTime(time_t blockTime)
{
  if (aftertime == beforetime)
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

/*Version 1

#include "GetNode.hpp"
#include <iostream>

int main()
{
  blocksci::Blockchain chain("/home/bitcoin-core/.blocksci/config.json");
  utility::string_t hash = U("bc1qwqdg6squsna38e46795at95yu9atm8azzmyvckulcc7kytlcckxswvvzej");
  clock_t start, end;
  start = clock();
  GetNode test(chain);
  std::unordered_set<std::string> result = test.nodeWithAddr(hash);

  end = clock();
  double exeT = (double)(end - start) / CLOCKS_PER_SEC;
  std::cout << "Total Time : " << exeT << std::endl;
  return 0;
}

GetNode::GetNode(blocksci::Blockchain &chain) : chain(chain) {}

std::unordered_set<std::string> GetNode::nodeWithAddr(utility::string_t &inputdata)
{
  std::string hash = utility::conversions::to_utf8string(inputdata);
  auto address = blocksci::getAddressFromString(hash, chain.getAccess());
  if (!address)
  {
    throw std::runtime_error("Invalid Address");
  }
  aftertime = beforetime = 0;
  std::unordered_set<std::string> result;
  result.insert(hash);
  clock_t start, end;
  int cnt = 0;
  int ccnt = 0;
  start = clock();
  for (const auto &tx : address->getTransactions())
  {
    cnt++;
    if (isInTime(tx.block().timestamp()))
    {
      // std::cout << tx.getHash().GetHex() << std::endl;
      ccnt++;
      result.insert(tx.getHash().GetHex());
    }
  }
  end = clock();
  double exeTime = (double)(end - start) / CLOCKS_PER_SEC;
  std::cout << "total txs : " << cnt << std::endl;
  std::cout << "in time txs : " << ccnt << std::endl;
  std::cout << "Execution Time : " << exeTime << std::endl;
  return result;
}

std::unordered_set<std::string> GetNode::nodeWithTx(utility::string_t &inputdata)
{
  std::string hash = utility::conversions::to_utf8string(inputdata);
  try
  {
    blocksci::Transaction tx(hash, chain.getAccess());
    setTime(tx.block().timestamp());

    std::unordered_set<std::string> result;

    return result;
  }
  catch (const std::exception &e)
  {
    throw e;
  }
}

void GetNode::setTime(time_t blockTime)
{
  aftertime = blockTime + DTIME / 2;
  beforetime = blockTime - DTIME / 2;
}
bool GetNode::isInTime(time_t blockTime)
{
  if (aftertime == beforetime)
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
*/