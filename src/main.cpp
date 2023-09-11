#include <blocksci/blocksci.hpp>
#include <iostream>

#include "BitcoinCore.hpp"
#include "Handler.hpp"
#include "MongoDB.hpp"

int main()
{
  MongoDB::Instance();
  MongoDB mongo("mongodb://localhost:27017");
  BitcoinCore bitcoinCore("bitcoin-core:0508");
  blocksci::Blockchain chain("/home/bitcoin-core/.blocksci/config.json");

  utility::string_t url = U("http://0.0.0.0:7776");
  Handler listener(url, mongo, bitcoinCore, chain);

  try
  {
    listener.open().wait();
    ucout << U("Listening for requests at: ") << listener.uri().to_string()
          << std::endl;
    ucout << U("Press ENTER to exit.") << std::endl;

    std::string line;
    std::getline(std::cin, line);

    listener.close().wait();
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  return 0;
}