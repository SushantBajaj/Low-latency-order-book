#include "../common/OrderbookTypes.h"
#include "../include/OrderbookTwo.h"

#include <cstdint>
#include <vector>


void OrderbookTwo::add_order(uint64_t id, Side side, uint16_t price, uint8_t qty){};
bool OrderbookTwo::cancel_order(uint64_t id){};
BBO OrderbookTwo::get_bbo() const{};
std::vector<Level> OrderbookTwo::get_bids(int depth) const{};
std::vector<Level> OrderbookTwo::get_asks(int depth) const{};
