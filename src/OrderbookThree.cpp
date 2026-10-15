#include "../common/OrderbookTypes.h"
#include "../include/OrderbookThree.h"

#include <cstdint>
#include <vector>


void OrderbookThree::add_order(uint32_t id, Side side, uint16_t price, uint8_t qty){};
bool OrderbookThree::cancel_order(uint32_t id){};
BBO OrderbookThree::get_bbo() const{};
std::vector<Level> OrderbookThree::get_bids(int depth) const{};
std::vector<Level> OrderbookThree::get_asks(int depth) const{};
