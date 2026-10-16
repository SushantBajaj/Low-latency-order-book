#include "../common/OrderbookTypes.h"
#include "../include/OrderbookThree.h"

#include <cstdint>
#include <vector>


using u64 = std::uint64_t;
using u32 = std::uint32_t;
using u16 = std::uint16_t;
using u8 = std::uint8_t;


size_t arr_size = (5000/64) + 1;
size_t bid_offset = (9999-(arr_size*64)) +1;
size_t ask_offset = (10001);
size_t quantity_arr_offset = 5000;


OrderbookThree::OrderbookThree(){
    bids = std::vector<u64>(arr_size,0);
    asks = std::vector<u64>(arr_size,0);
    quantities = std::vector<u32>(10001);

}

u8 last_set_bit(u64 qword){
    u8 curr = {};
    u32 low_dword = static_cast<u32>(qword);
    u32 high_dword = static_cast<u32>(qword>>32);
    u32 dword = {};

    if(high_dword!=0){
        dword = high_dword;
        curr+=32;
    }else{
        dword = low_dword;
    }

    u16 low_word = static_cast<u16>(dword);
    u16 high_word = static_cast<u16>(dword>>16);
    u16 word = {};

    if(high_word!=0){
        word = high_word;
        curr+=16;
    }else{
        word = low_word;
    }

    u8 low_byte = static_cast<u8>(word);
    u8 high_byte = static_cast<u8>(word>>8);
    u8 byte = {};

    if(high_byte!=0){
        byte = high_byte;
        curr+=8;
    }else{
        byte = low_byte;
    }

    for(int i =7;i>=0;i--){
        if(byte & (u8{1}<<i)){
            curr+=i;
            break;
        }
    }
    return curr;

}


void OrderbookThree::add_order(uint32_t id, Side side, uint16_t price, uint8_t qty){
    size_t bit_position;
    if(side==Side::Sell){
        bit_position = price-ask_offset;
    }else{
        bit_position = price-bid_offset;
    }
    size_t qword_idx = bit_position/64;
    bit_position %=64;

    if(side==Side::Sell){
        asks[qword_idx] |= (u64{1}<<(63-bit_position));
    }else{
        bids[qword_idx] |= (u64{1}<<(63-bit_position));
    }

    quantities[price-quantity_arr_offset]+=qty;
    id_map.insert({id,{price,qty,side}});

    
}
bool OrderbookThree::cancel_order(uint32_t id){
    auto it = id_map.find(id);
    if(it != id_map.end()){
        Side side = it->second.side;
        u16 price = it->second.price;
        u8 qty = it->second.qty;

        size_t qty_idx = price-quantity_arr_offset;
        quantities[qty_idx]-=qty;

        if(quantities[qty_idx]==0){

            size_t bit_position;
            if(side==Side::Sell){
                bit_position = price-ask_offset;
            }else{
                bit_position = price-bid_offset;
            }
            size_t qword_idx = bit_position/64;
            bit_position %=64;
            u64 mask = u64{1} << (63 - bit_position);
            if(side==Side::Sell){
                asks[qword_idx] &= ~mask;
            }else{
                bids[qword_idx] &= ~mask;
            }

        }
        
        id_map.erase(it);
        return true;
    }
    return false;
};
BBO OrderbookThree::get_bbo(){
    BBO temp;
    bool has_bid = false;
    size_t bid_qword_idx;
    for(int i  = bids.size()-1;i>=0;i--){
        if(bids[i]!=0){
            bid_qword_idx = i;
            has_bid = true;
            break;
        }
    }
    temp.has_bid = has_bid;
    if(has_bid){
        u8 bit_position = last_set_bit(bids[bid_qword_idx]);
        u16 price = ((bid_qword_idx*64)+(63-bit_position))+bid_offset;
        temp.bid_price = price;
        temp.bid_qty = quantities[price-quantity_arr_offset];

    }else{
        temp.bid_price = 0;
        temp.bid_qty = 0;
    }

    bool has_ask = false;
    size_t ask_qword_idx;
    for(int i =0;i<asks.size();i++){
        if(asks[i]!=0){
            ask_qword_idx = i;
            has_ask = true;
            break;
        }
    }
    temp.has_ask = has_ask;

    if(has_ask){
        u8 bit_position = last_set_bit(asks[ask_qword_idx]);
        u16 price = ((ask_qword_idx*64)+ (63-bit_position)) + ask_offset;
        temp.ask_price = price;
        temp.ask_qty = quantities[price+quantity_arr_offset];
    }else{
        temp.ask_price = 0;
        temp.ask_qty = 0;
    }

    return temp;
};
std::vector<Level> OrderbookThree::get_bids(int depth) {};
std::vector<Level> OrderbookThree::get_asks(int depth) {};
