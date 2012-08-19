#pragma once

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>

#include <wield/logging/Message.h>

namespace wield { namespace logging {

    class InfoMessage : public Message
    {
    public:
        InfoMessage(const std::string& str)
            : Message(str)
        {
        }

        friend std::ostream& operator<<(std::ostream& os, const InfoMessage& m)
        {
            // TODO: implement time service, get the time from that.
            std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
            std::time_t now_c = std::chrono::system_clock::to_time_t(now);

            os << "[" << std::put_time(std::gmtime(&now_c), "%Y-%m-%d %H:%M:%S") << "] INFO: " << m.str_ << "\n";
            return os;
        }
    };

    class WarningMessage : public Message
    {
    public:
        WarningMessage(const std::string& str)
            : Message(str)
        {
        }

        friend std::ostream& operator<<(std::ostream& os, const WarningMessage& m)
        {
            // TODO: implement time service, get the time from that.
            std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
            std::time_t now_c = std::chrono::system_clock::to_time_t(now);

            os << "[" << std::put_time(std::gmtime(&now_c), "%Y-%m-%d %H:%M:%S") << "] WARNING: " << m.str_ << "\n";
            return os;
        }
    };

    class ErrorMessage : public Message
    {
    public:
        ErrorMessage(const std::string& str)
            : Message(str)
        {
        }

        friend std::ostream& operator<<(std::ostream& os, const ErrorMessage& m)
        {
            // TODO: implement time service, get the time from that.
            std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
            std::time_t now_c = std::chrono::system_clock::to_time_t(now);

            os << "[" << std::put_time(std::gmtime(&now_c), "%Y-%m-%d %H:%M:%S") << "] ERROR: " << m.str_ << "\n";
            return os;
        }
    };
}}
