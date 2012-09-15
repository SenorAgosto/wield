#pragma once
#include <wield/util/EnumUtils.h>

#ifdef _WINDOWS
#include <Windows.h>

namespace wield { namespace config {

    enum class ProcessPriorityEnum : uint32_t
    {
        Idle,
        BelowNormal,
        Normal,
        AboveNormal,
        HighPriority,
        RealTime,

        NumberOfEntries
    };

    static const char* ProcessPriorityStrings[] = 
    {
        "IDLE_PRIORITY_CLASS",
        "BELOW_NORMAL_PRIORITY_CLASS",
        "NORMAL_PRIORITY_CLASS",
        "ABOVE_NORMAL_PRIORITY_CLASS",
        "HIGH_PRIORITY_CLASS",
        "REALTIME_PRIORITY_CLASS",

        "NumberOfEntries"
    };
}}

namespace wield { namespace util {
    
    template<>
    DWORD convertEnumTo(config::ProcessPriorityEnum e)
    {
        static DWORD conversionMapping[] = {
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