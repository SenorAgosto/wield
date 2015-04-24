#include "./platform/UnitTestSupport.hpp"

#include <wield/logging/Log.hpp>
#include <wield/logging/LoggingPolicy.hpp>

namespace {
    
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
