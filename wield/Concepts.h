#pragma once
#include <type_traits>

namespace wield {
    
    template<typename Enum>
    class IsEnumConcept
    {
        static_assert(std::is_enum<Enum>::value, "IsEnumConcept failure. Argument is not an enum type.");
    };
}
