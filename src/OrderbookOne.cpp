#include "../common/OrderbookTypes.h"
#include "../include/OrderbookOne.h"

#include <cstdint>
#include <vector>


void OrderbookOne::add_order(uint32_t id, Side side, uint16_t price, uint8_t qty){};
bool OrderbookOne::cancel_order(uint32_t id){};
BBO OrderbookOne::get_bbo() const{};
std::vector<Level> OrderbookOne::get_bids(int depth) const{};
std::vector<Level> OrderbookOne::get_asks(int depth) const{};
