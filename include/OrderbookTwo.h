#pragma once

#include "../common/OrderbookTypes.h"
#include "./Orderbook.h"

#include <cstdint>
#include <vector>

class OrderbookTwo : public IOrderBook{
    public:
    void add_order(uint64_t id, Side side, uint16_t price, uint8_t qty) override;
    bool cancel_order(uint64_t id) override;
    BBO get_bbo() const override;
    std::vector<Level> get_bids(int depth = 10) const override;
    std::vector<Level> get_asks(int depth = 10) const override;
};