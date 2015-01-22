#include <wield/Exceptions.hpp>

namespace wield {

    DuplicateStageRegistrationException::DuplicateStageRegistrationException()
            : std::runtime_error("DispatherBase::registerStage() duplicate registration of stage.")
    {
    }
}
