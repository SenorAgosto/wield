#include "platform/UnitTestSupport.h"
#include <wield/Exceptions.h>

namespace {

    TEST(verifyDuplicateStageRegistrationException)
    {
        wield::DuplicateStageRegistrationException e;
        CHECK_EQUAL("DispatherBase::registerStage() duplicate registration of stage.", e.what());
    }
}