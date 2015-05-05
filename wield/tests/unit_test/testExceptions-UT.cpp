#include "./platform/UnitTestSupport.hpp"
#include <wield/Exceptions.hpp>

namespace {

    TEST(verifyDuplicateStageRegistrationException)
    {
        wield::DuplicateStageRegistrationException e;
        CHECK_EQUAL("DispatherBase::registerStage() duplicate registration of stage.", e.what());
    }
}

