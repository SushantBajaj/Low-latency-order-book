using uint16_t ticks instead of double for prices, uint16 because with ticks price can go upto 15000, uint16_t can store upto 2^16-1 (65535), so why not save memeory

using uint8_t for quantity since max quantity is 100, and it can store upto 2^8-1 ie 255

changed uint64_t to uint32_t for order id, as values returned by mt19937 fits well in 32 bits