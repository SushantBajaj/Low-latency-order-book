#include "../common/OrderbookTypes.h"
#include "../include/Orderbook.h"
#include "../include/OrderbookOne.h"
#include "../include/OrderbookTwo.h"
#include "../include/OrderbookThree.h"

#include <memory>
#include <stdexcept>
#include <string>
#include <cstdint>
#include <random>
#include <vector>
#include <iostream>
#include <chrono>

using u32 = std::uint32_t;
using u16 = std::uint16_t;
using u8 = std::uint8_t;
using Clock = std::chrono::steady_clock;


double to_ns(std::chrono::steady_clock::duration duration) {
    return std::chrono::duration<double, std::nano>(duration).count();
}

int main(int argc, char* argv[]){

    if (argc != 3) {
        throw std::invalid_argument("Incorrect number of arguments");
    }else if(std::string(argv[1])!="--impl"){
        throw std::invalid_argument("Incorrect arguments- Usage Example: ./bench --impl A");

    }


    std::unique_ptr<IOrderbook> book;
    if(std::string(argv[2])=="A"){
        book = std::make_unique<OrderbookOne>();
    }else if(std::string(argv[2])=="B"){
        book = std::make_unique<OrderbookTwo>();
    }else if(std::string(argv[2])=="C"){
        book = std::make_unique<OrderbookThree>();
    }else{
        throw std::invalid_argument("Incorrect arguments- Usage Example: ./bench --impl A");
    }


    u32 operations = 1e6;
    std::mt19937 rng(42); // to genearte pseudo random price and quantity

    std::vector<u32>order_ids; // to store added order ids for cancelling em
    order_ids.reserve(operations);

    int checksum {};
    u32 current_id {}; // not using mt19937 for order ids as it can have collisons and we need unique order ids

    Clock::duration add_order_time = Clock::duration::zero();
    Clock::duration get_bbo_time = Clock::duration::zero();
    Clock::duration cancel_order_time = Clock::duration::zero();
    Clock::duration get_bids_asks_time = Clock::duration::zero();
    u32 add_order_count {};
    u32 get_bbo_count {};
    u32 cancel_order_count {};
    u32 get_bids_asks_count {};


    auto total_start = Clock::now();

    while(operations--){
        if(operations%100==0){ // getting bids and ask every 100 operations
            get_bids_asks_count++;


            auto start = Clock::now();

            std::vector<Level> bids = book->get_bids();
            std::vector<Level> asks = book->get_asks();

            auto end = Clock::now();
            get_bids_asks_time+= end-start;

            checksum+=(bids.size()%2); // to prevent compiler from optimizing away this operation
            checksum+=(asks.size()%2);
        }

        u32 random_val = rng();

        if(operations%10==1){ // 10% get_bbo operations
            get_bbo_count++;

            auto start = Clock::now();

            BBO curr_bbo = book->get_bbo();

            auto end = Clock::now();
            get_bbo_time+=end-start;

            checksum+=curr_bbo.has_bid; // to prevent compiler from optimizing away this operation

        }else if(operations%10 == 2 || operations%10 == 3){ // 20% cancel operations
            if(order_ids.size()==0) continue; // no orders = no cancel
            cancel_order_count++;

            u32 cancel_idx = random_val%order_ids.size();

            auto start = Clock::now();

            bool cancelled = book->cancel_order(order_ids[cancel_idx]);
            
            auto end = Clock::now();
            cancel_order_time+=end-start;

        }else{// 70% add operations
            add_order_count++;

            if(random_val%2){ // Buy bid, 50% out of total add opertions
                
                u16 value = (random_val%5000) + 5000; // range = [5000,9999]
                u8 quantity = ((random_val+operations) %100) +1; // range = [1,100]

                auto start = Clock::now();
                
                book->add_order(current_id, Side::Buy, value, quantity);
                
                auto end = Clock::now();
                add_order_time+=end-start;
                
            }else{ //sell bid, 50% out of total add opertions
                
                u16 value = (random_val%5000) + 10001; // range = [10001, 15000]
                u8 quantity = ((random_val+operations) %100) +1; //
                
                auto start = Clock::now();

                book->add_order(current_id, Side::Sell, value, quantity);

                auto end = Clock::now();
                add_order_time+=end-start;
            }
            order_ids.push_back(current_id++); // add order id in vector to support cancellation

        }


    }

    auto total_end = Clock::now();

    double total_duration = to_ns(total_end-total_start);
    double average_add_order_time = to_ns(add_order_time)/add_order_count; 
    double average_cancel_order_time = to_ns(cancel_order_time)/cancel_order_count; 
    double average_get_bbo_time = to_ns(get_bbo_time)/get_bbo_count; 
    double average_get_bids_asks_time = to_ns(get_bids_asks_time)/get_bids_asks_count; 

    std::cout<<"Total execution time = "<<total_duration<<"ns"<<std::endl<<std::endl;
    std::cout<<"Average time per add order = "<<average_add_order_time<<"ns"<<std::endl;
    std::cout<<"Average time per cancel order = "<<average_cancel_order_time<<"ns"<<std::endl;
    std::cout<<"Average time per get BBO = "<<average_get_bbo_time<<"ns"<<std::endl;
    std::cout<<"Average time per get bids and asks = "<<average_get_bids_asks_time<<"ns"<<std::endl;



    std::cout<<"checksum = "<<checksum<<std::endl;

    

}
