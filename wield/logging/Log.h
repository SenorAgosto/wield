#pragma once
#include <memory>
#include <string>

namespace wield { namespace logging {
    
    class LoggingPolicy;
    using LoggingPolicyType = std::unique_ptr<LoggingPolicy>;
    
    class Log
    {
    public:
        static void Info(const std::string& info);
        static void Warning(const std::string& warning);
        static void Error(const std::string& error);
        
        // Replace default logging policy with user defined logging implementation.
        static void SetLoggingPolicy(LoggingPolicyType policy);
        
    private:
        Log() = delete;
        
        static LoggingPolicyType loggingPolicy_;
    };
}}