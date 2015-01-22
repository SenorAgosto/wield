#include "./DefaultLoggingPolicy.hpp"
#include <iostream>

namespace wield { namespace logging {
    
    void DefaultLoggingPolicy::Info(const std::string& info) const
    {
        std::cerr << "[Info] " << info << std::endl;
    }
    
    void DefaultLoggingPolicy::Warning(const std::string& warning) const
    {
        std::cerr << "[Warning]" << warning << std::endl;
    }
    
    void DefaultLoggingPolicy::Error(const std::string& error) const
    {
        std::cerr << "[Error]" << error << std::endl;
    }
}}
