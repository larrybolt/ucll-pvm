#include "cache.h"
#include "shared.h"
#include <iostream>
#include <vector>


namespace
{
    void process(std::vector<uint64_t>& ns, unsigned block_size, unsigned reads_per_item)
    {
        uint64_t result = 0;

        if (ns.size() % block_size)
        {
            std::cerr << "invalid block size" << std::endl;
            abort();
        }

        auto block_count = ns.size() / block_size;

        for (unsigned block_index = 0; block_index < block_count; ++block_index)
        {
            unsigned start_i = block_index * block_size;

            for (unsigned k = 0; k != reads_per_item * block_size; ++k)
            {
                unsigned i = k * (k ^ 0x395A13C5);
                i = (k ^ (97 * k)) * i;
                i %= block_size;

                result ^= ns[start_i + i];
            }
        }

        ns[0] = result;
    }

    void test(std::vector<uint64_t>& ns, unsigned block_size, unsigned repeats_per_block)
    {
        std::cout
            << "Block size " << block_size * sizeof(uint64_t) << " bytes"
            << ", " << repeats_per_block << " repeats" 
            << ", " << ns.size() / block_size << " blocks"
            << " --> " << measure_time([&]() {process(ns, block_size, repeats_per_block);}) /repeats_per_block << "ms per repeat"
            << std::endl;
    }
}

void test_cache()
{
    std::vector<uint64_t> ns(268435456);

    for (auto i : std::vector<unsigned>{ 1,2,3,4,5,10,15,16,17,18,19,23,24 })
    {
        unsigned block_size = 8 * (1 << i);

        test(ns, block_size, 1);
        test(ns, block_size, 2);
        test(ns, block_size, 4);
    }
}
