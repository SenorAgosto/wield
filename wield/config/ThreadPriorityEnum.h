#pragma once
#include <wield/util/EnumUtils.h>

#ifdef _WINDOWS
#include <Windows.h>

namespace wield { namespace config {

    enum class ThreadPriorityEnum : uint32_t 
    {
        AboveNormal,
        BelowNormal,
        Highest,
        Idle,
        Lowest,
        Normal,
        TimeCritical,

        NumberOfEntries
    };

    static const char* ThreadPriorityStrings[] = 
    {
        "THREAD_PRIORITY_ABOVE_NORMAL",
        "THREAD_PRIORITY_BELOW_NORMAL",
        "THREAD_PRIORITY_HIGHEST",
        "THREAD_PRIORITY_IDLE",
        "THREAD_PRIORITY_LOWEST",
        "THREAD_PRIORITY_NORMAL",
        "THREAD_PRIORITY_TIME_CRITICAL",
        "NumberOfEntries"
    };
}}

namespace wield { namespace util {

    template<>
    int convertEnumTo(config::ThreadPriorityEnum e)
    {
        static int conversionMapping[] = {
            IDLE_PRIORITY_CLASS,
            BELOW_NORMAL_PRIORITY_CLASS,
            NORMAL_PRIORITY_CLASS,
            ABOVE_NORMAL_PRIORITY_CLASS,
            HIGH_PRIORITY_CLASS,
            REALTIME_PRIORITY_CLASS
        };

        return conversionMapping[static_cast<size_t>(e)];
    }
}}
#endif
