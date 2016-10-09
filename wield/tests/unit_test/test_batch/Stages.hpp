#pragma once
#include <cstdint>
#include <iostream>

namespace test_batch {
        
    enum class Stages : uint8_t
    {
        Stage1,
        Stage2,
        Stage3,

        NumberOfEntries
    };

    std::ostream& operator<<(std::ostream& os, Stages stage);
}
