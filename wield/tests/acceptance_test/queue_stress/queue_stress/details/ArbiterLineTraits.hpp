#pragma once 
#include <queue_stress/details/ArbiterErrorReportingPolicy.hpp>

namespace queue_stress { namespace details {

    struct ArbiterLineTraits
    {
        static constexpr std::size_t FirstExpectedSequenceNumber() { return 0; }
        static constexpr std::size_t LargestRecoverableGap() { return 1000; }
        static constexpr std::size_t NumberOfLines() { return 1; } 
        static constexpr std::size_t HistoryDepth() { return 5000; }

        using SequenceType = std::size_t;
        using ErrorReportingPolicy = ArbiterErrorReportingPolicy;
    };
}}
