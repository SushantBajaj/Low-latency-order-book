#pragma once

#include "../common/OrderbookTypes.h"
#include "./Orderbook.h"

#include <cstdint>
#include <vector>
#include <map>
#include <unordered_map>
#include <functional>

using u32 = std::uint32_t;
using u16 = std::uint16_t;
using u8 = std::uint8_t;

struct id_map_val{
    Side side;
    std::map<u16,u32>::iterator it;
    u8 quantity;
};

class OrderbookOne : public IOrderbook{
    private:
    
    std::map<u16,u32, std::greater<u16>>bids;
    std::map<u16,u32>asks;
    std::unordered_map<u32, id_map_val>id_map;
    

    public:

    void add_order(uint32_t id, Side side, uint16_t price, uint8_t qty) override;
    bool cancel_order(uint32_t id) override;
    BBO get_bbo() const override;
    std::vector<Level> get_bids(int depth = 10) const override;
    std::vector<Level> get_asks(int depth = 10) const override;
};