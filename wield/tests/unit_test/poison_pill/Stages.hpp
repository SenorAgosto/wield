#pragma once
#include <cstdint>
#include <iostream>

namespace poison_pill {
        
    enum class Stages : uint8_t
    {
        Stage1,
        Stage2,
        Stage3,
        Stage4,
        
        NumberOfEntries
    };

    std::ostream& operator<<(std::ostream& os, Stages stage);
}
