#include <wield/logging/Log.h>
#include <wield/logging/LoggingPolicy.h>

#include "DefaultLoggingPolicy.h"

namespace wield { namespace logging {

    //static
    LoggingPolicyType Log::loggingPolicy_ = LoggingPolicyType(new DefaultLoggingPolicy());
    
    //static
    void Log::Info(const std::string& info)
    {
        loggingPolicy_->Info(info);
    }

    //static
    void Log::Warning(const std::string& warning)
    {
        loggingPolicy_->Warning(warning);
    }
    
    //static
    void Log::Error(const std::string& error)
    {
        loggingPolicy_->Error(error);
    }

    //static
    void Log::SetLoggingPolicy(LoggingPolicyType policy)
    {
        loggingPolicy_.swap(policy);
    }
}}