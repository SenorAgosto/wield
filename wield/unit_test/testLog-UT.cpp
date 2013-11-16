#include "platform/UnitTestSupport.h"

#include <wield/logging/Log.h>
#include <wield/logging/LoggingPolicy.h>

namespace {
    
    TEST(verifyLogCanBeInstantiated)
    {
        wield::logging::Log::Info("testing 1 2 3");
        wield::logging::Log::Warning("testing 1 2 3");
        wield::logging::Log::Error("testing 1 2 3");
    }
    
    class NullLoggingPolicy : public wield::logging::LoggingPolicy
    {
    public:
        void Info(const std::string& info) const override {}
        void Warning(const std::string& warning) const override {}
        void Error(const std::string& error) const override {}
    };
    
    TEST(verifyLogPolicyCanBeChanged)
    {
        wield::logging::Log::SetLoggingPolicy(wield::logging::LoggingPolicyType(new NullLoggingPolicy));
        wield::logging::Log::Info("null info");
        wield::logging::Log::Warning("null warning");
        wield::logging::Log::Error("null error");
    }
}