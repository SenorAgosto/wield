#include "UnitTestSupport.h"

#include <wield/logging/Logging.h>
#include <wield/logging/ScopedClogRedirector.h>

#include <iostream>
#include <sstream>

namespace {

    struct LogToStringStreamFixture
    {
        wield::logging::ScopedClogRedirector<std::ostringstream> log;
    };

    TEST_FIXTURE(LogToStringStreamFixture, verifyInfoMessage)
    {
        std::clog << wield::logging::InfoMessage("Test Info Message");
    }

    TEST_FIXTURE(LogToStringStreamFixture, verifyWarningMessage)
    {
        std::clog << wield::logging::WarningMessage("Test Warning Message");
    }

    TEST_FIXTURE(LogToStringStreamFixture, verifyErrorMessage)
    {
        std::clog << wield::logging::WarningMessage("Test Error Message");
    }
}