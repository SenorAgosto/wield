#include <UnitTest++/UnitTest++.h>
#include <wield/logging/ScopedClogRedirector.h>

#include <sstream>
#include <boost/filesystem.hpp>

namespace {

    TEST(verifyScopedClogRedirectorWorksWithStringStream)
    {
        wield::logging::ScopedClogRedirector<std::ostringstream> logRedirector;
        std::clog << "Testing 1 2 3";

        CHECK_EQUAL("Testing 1 2 3", logRedirector.str());
    }

    TEST(verifyScopedClogRedirectorToFile)
    {
        std::string testLogFile("test.log");
        {
            wield::logging::ScopedClogRedirector<std::ofstream> logRedirector(testLogFile);
            CHECK(boost::filesystem::exists(logRedirector.logFile()));

            std::clog << "Testing 1 2 3" << std::endl;
        }
        
        {
            std::ifstream testLog(testLogFile);
            std::string line;

            std::getline(testLog, line);
            CHECK_EQUAL("Testing 1 2 3", line);
        }

        if(boost::filesystem::exists(testLogFile))
        {
            boost::filesystem::remove(testLogFile);
        }
    }
}