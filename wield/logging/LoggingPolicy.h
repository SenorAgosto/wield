#pragma once
#include <string>

namespace wield { namespace logging {

    class LoggingPolicy
    {
    public:
        virtual ~LoggingPolicy() {}
        
        virtual void Info(const std::string& info) const = 0;
        virtual void Warning(const std::string& warning) const = 0;
        virtual void Error(const std::string& error) const = 0;
    };
}}