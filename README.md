using uint16_t ticks instead of double for prices, uint16 because with ticks price can go upto 15000, uint16_t can store upto 2^16-1 (65535), so why not save memeory

using uint8_t for quantity since max quantity is 100, and it can store upto 2^8-1 ie 255

changed uint64_t to uint32_t for order id, as values returned by mt19937 fits well in 32 bits

will be using a very classic map as my implementation 1, to create a nice baseline for timings and correctness both, obv would be slow

had to use u32 whereever we were storing aggregate quantitits to prevent overflow

first run on map impl results-
```
Total execution time = 1.61724e+08ns

Average time per add order = 153.275ns
Average time per cancel order = 126.504ns
Average time per get BBO = 17.4426ns
Average time per get bids and asks = 145.364ns
checksum = 100000
```

first perf on it
```
 Performance counter stats for './b --impl A' (50 runs):

            159.75 msec task-clock:u                                                            ( +-  0.65% )
      35,84,75,525      cpu_atom/cycles/u                                                       ( +-  5.01% )  (0.15%)
      55,93,11,800      cpu_core/cycles/u                                                       ( +-  0.62% )  (99.85%)
      39,81,72,921      cpu_atom/instructions/u                                                 ( +-  5.37% )  (0.15%)
      58,21,75,976      cpu_core/instructions/u                                                 ( +-  0.12% )  (99.85%)
       7,72,65,787      cpu_atom/branches/u                                                     ( +-  5.48% )  (0.15%)
      11,51,72,596      cpu_core/branches/u                                                     ( +-  0.12% )  (99.85%)
         36,57,979      cpu_atom/branch-misses/u                                                ( +-  5.82% )  (0.15%)
         52,16,128      cpu_core/branch-misses/u                                                ( +-  0.12% )  (99.85%)
          5,08,862      cpu_atom/cache-misses/u                                                 ( +- 13.57% )  (0.15%)
          6,55,300      cpu_core/cache-misses/u                                                 ( +-  9.87% )  (99.85%)
   <not supported>      cpu_atom/L1-dcache-load-misses/u                                      
         83,55,894      cpu_core/L1-dcache-load-misses/u                                        ( +-  0.22% )  (99.85%)
            22,547      cpu_atom/dTLB-load-misses/u                                             ( +- 22.36% )  (0.15%)
          3,66,196      cpu_core/dTLB-load-misses/u                                             ( +-  0.79% )  (99.85%)
             9,960      page-faults:u                                                           ( +-  0.00% )

           0.16177 +- 0.00106 seconds time elapsed  ( +-  0.65% )

```


trying removing branching-
earlier buy sell branch was predictable, so made that a bit more unpredicatable first
then started adding some branchless code-
4.66%->4.51% () - used map pointer to sleect map in add_order on impl1

using monotonic_buffer for the ds of implA really helped, times improved by a lot, and the best thing that happened was that cache miss reduced by 36%, this prolly happened because with mono allots nodes for the map, it gives them in order while the default allocator finds free slots and fill em, so mono improved our cache locality, didnt measure memeory, but mono will ofc take more