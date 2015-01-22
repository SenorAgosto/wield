#include <wield/logging/LoggingPolicy.hpp>

namespace wield { namespace logging {

    class DefaultLoggingPolicy : public LoggingPolicy
    {
    public:
        void Info(const std::string& info) const override;
        void Warning(const std::string& warning) const override;
        void Error(const std::string& error) const override;
    };
}}