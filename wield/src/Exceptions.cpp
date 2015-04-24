#include <wield/Exceptions.hpp>

namespace wield {

    DuplicateStageRegistrationException::DuplicateStageRegistrationException()
        : std::runtime_error("DispatherBase::registerStage() duplicate registration of stage.")
    {
    }

    IllegallyPushedMessageOntoQueueAdapter::IllegallyPushedMessageOntoQueueAdapter()
        : std::runtime_error("Pushed message onto MultipleInputQueueAdapter rather than contained queue.")
    {
    }
}

