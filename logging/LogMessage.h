#pragma once
#include <string>

namespace wield { namespace logging {

    class LogMessage
    {
    public:
        LogMessage(const std::string& str)
            : str_(str)
        {
        }

        virtual ~LogMessage() = 0;

    protected:
        std::string str_;
    };
}}
