#include "cache.h"
#include "shared.h"
#include <iostream>
#include <vector>
#include <memory>


namespace
{
    uint64_t process_block(uint64_t* p, unsigned block_size, uint64_t repeats, const std::vector<unsigned>& indices)
    {
        uint64_t result = 0;

        for (uint64_t repetition = 0; repetition != repeats; ++repetition)
        {
            for (auto i : indices)
            {
                result ^= p[i];
            }
        }

        return result;
    }

    void benchmark(uint64_t* p, unsigned block_size, uint64_t n_accesses)
    {
        const uint64_t repeats = n_accesses / block_size + 1;
        uint64_t result;
        auto indices = range<unsigned>(0, block_size);

        double sequential_time = measure_time<uint64_t>([p, block_size, repeats, &indices]() { return process_block(p, block_size, repeats, indices); }, &result);
        shuffle(indices);
        double random_time = measure_time<uint64_t>([p, block_size, repeats, &indices]() { return process_block(p, block_size, repeats, indices); }, &result);

        sequential_time /= block_size * repeats;
        random_time /= block_size * repeats;
        sequential_time *= 1000000;
        random_time *= 1000000;

        std::cout
            << "Block size " << block_size
            << ", " << repeats << " repeats"
            << ", sequential access: " << sequential_time << "ns/item"
            << std::endl;

        std::cout
            << "Block size " << block_size
            << ", " << repeats << " repeats"
            << ", random access: " << random_time << "ns/item"
            << std::endl;

        std::cout << std::endl;
    }
}

void test_cache()
{
    std::unique_ptr<uint64_t[]> buffer(new uint64_t[64 * 1024 * 1024]);
    uint64_t* p = buffer.get();
    
    const uint64_t n_accesses = 1000000000;

    benchmark(p, 32, n_accesses);
    benchmark(p, 64, n_accesses);
    benchmark(p, 128, n_accesses);
    benchmark(p, 256, n_accesses);
    benchmark(p, 512, n_accesses);
    benchmark(p, 1024, n_accesses);
    benchmark(p, 2 * 1024, n_accesses);
    benchmark(p, 4 * 1024, n_accesses);
    benchmark(p, 8 * 1024, n_accesses);
    benchmark(p, 16 * 1024, n_accesses);
    benchmark(p, 32 * 1024, n_accesses);
    benchmark(p, 64 * 1024, n_accesses);
    benchmark(p, 128 * 1024, n_accesses);
    benchmark(p, 256 * 1024, n_accesses);
    benchmark(p, 8 * 1024 * 1024, n_accesses);
    benchmark(p, 16 * 1024 * 1024, n_accesses);
    benchmark(p, 32 * 1024 * 1024, n_accesses);
}
