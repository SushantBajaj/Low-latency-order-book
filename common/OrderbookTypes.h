#pragma once

#include <cstdint>

enum class Side { Buy, Sell };

struct Level {
    uint16_t price;
    uint8_t qty;
};

struct BBO {
    bool has_bid;
    uint16_t bid_price;
    uint8_t bid_qty;
    bool has_ask;
    uint16_t ask_price;
    uint8_t ask_qty;
};