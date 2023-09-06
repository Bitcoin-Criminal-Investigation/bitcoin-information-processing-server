#include <iostream>

#include "Handler.hpp"

int main() {
  utility::string_t url = U("http://0.0.0.0:7776");
  Handler listener(url);

  try {
    listener.open().wait();
    ucout << U("Listening for requests at: ") << listener.uri().to_string()
          << std::endl;
    ucout << U("Press ENTER to exit.") << std::endl;

    std::string line;
    std::getline(std::cin, line);

    listener.close().wait();
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  return 0;
}