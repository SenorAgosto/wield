#pragma once
#include <string>

namespace wield { namespace logging {

    /* A logging interface
    
       Rather than wield defining where to log when there are problems,
       users are expected to implement this interface and inject it.
    */
    class LoggingPolicy
    {
    public:
        virtual ~LoggingPolicy() {}
      
        // Log informational message
        virtual void Info(const std::string& info) const = 0;
        
        // Log a warning message
        virtual void Warning(const std::string& warning) const = 0;
        
        // Log an error message
        virtual void Error(const std::string& error) const = 0;
    };
}}

