#include <blocksci/blocksci.hpp>

int main() {
        blocksci::Blockchain chain{"/home/bitcoin-core/.blocksci/config.json"};
        return 0;
}