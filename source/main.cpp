#include "main.h"

int main() {
    Board game;
    std::unordered_map<size_t, size_t> position_count;
    position_count[game.get_hash(20)] = 1;
    while (true) {
        std::string res = process_query(position_count, game, std::cin);
        if (res.size()) {
            std::cout << res << "\n";
            std::cout.flush();
        }
    }
}
