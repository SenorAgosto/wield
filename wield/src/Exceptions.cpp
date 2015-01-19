#include <wield/Exceptions.h>

namespace wield {

    DuplicateStageRegistrationException::DuplicateStageRegistrationException()
            : std::runtime_error("DispatherBase::registerStage() duplicate registration of stage.")
    {
    }
}
