#pragma once 
#include <cstddef> 

namespace queue_stress { namespace details {

    struct ArbiterErrorReportingPolicy
    {
        void FirstSequenceNumberOutOfSequence(const std::size_t line, const std::size_t sequence);
		void DuplicateOnLine(const std::size_t line, const std::size_t sequence);
		void Gap(const std::size_t start, const std::size_t length);
		void GapFill(const std::size_t start, const std::size_t length);
		void LinePositionOverrun(const std::size_t /*slowLine*/, const std::size_t /*overrunByLine*/);
		void UnrecoverableGap(const std::size_t start, const std::size_t length = 1);
        void UnrecoverableLineGap(const std::size_t line, const std::size_t sequenceNumber);
    };
}}
