#pragma once
#include <cstdint>
#include <iostream>

namespace test_color_minus {
        
    enum class Stages : std::uint8_t
    {
        Stage1,
        Stage2,
        Stage3,

        NumberOfEntries
    };

    std::ostream& operator<<(std::ostream& os, Stages s);
}
