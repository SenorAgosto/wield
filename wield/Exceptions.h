#pragma once
#include <stdexcept>

namespace wield {

    class DuplicateStageRegistrationException final : public std::runtime_error
    {
    public:
        DuplicateStageRegistrationException()
            : std::runtime_error("DispatherBase::registerStage() duplicate registration of stage.")
        {
        }
    };
}
