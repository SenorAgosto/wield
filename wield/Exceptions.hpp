#pragma once
#include <stdexcept>

namespace wield {

    class DuplicateStageRegistrationException final : public std::runtime_error
    {
    public:
        DuplicateStageRegistrationException();
    };
}
