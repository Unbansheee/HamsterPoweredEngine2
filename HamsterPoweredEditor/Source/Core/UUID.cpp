#include "UUID.h"

namespace HP
{
    UUID::UUID()
    {
        Generate();
    }

    UUID::UUID(const std::string& str)
    {
        SetID(str);
    }

    void UUID::Generate()
    {
    
        std::random_device rd;
        auto seed_data = std::array<int, std::mt19937::state_size> {};
        std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
        std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
        std::mt19937 generator(seq);
        uuids::uuid_random_generator gen{generator};

        id = gen();
    
    
    }
}
