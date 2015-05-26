#include <queue_stress/details/ArbiterErrorReportingPolicy.hpp>
#include <iostream>

namespace queue_stress { namespace details {

    void ArbiterErrorReportingPolicy::FirstSequenceNumberOutOfSequence(const std::size_t line, const std::size_t sequence)
    {
        std::cerr << "First sequence number out of order: (" << line << "): " << sequence << std::endl;
    }

    void ArbiterErrorReportingPolicy::DuplicateOnLine(const std::size_t line, const std::size_t sequence)
    {
        std::cerr << "Duplicate sequence number on line (" << line << "): " << sequence << std::endl;
    }
    
    void ArbiterErrorReportingPolicy::Gap(const std::size_t start, const std::size_t length)
    {
        std::cerr << "Gap at sequence number " << start << " for " << length << " messages." << std::endl;
    }

    void ArbiterErrorReportingPolicy::GapFill(const std::size_t start, const std::size_t length)
    {
        std::cerr << "Gap Fill: " << start << ", length: " << length << std::endl;
    }

    void ArbiterErrorReportingPolicy::LinePositionOverrun(const std::size_t /*slowLine*/, const std::size_t /*overrunByLine*/)
    {
    }

    void ArbiterErrorReportingPolicy::UnrecoverableGap(const std::size_t start, const std::size_t length)
    {
        std::cerr << "Unrecoverable gap at " << start << " of length " << length << std::endl;
    }

    void ArbiterErrorReportingPolicy::UnrecoverableLineGap(const std::size_t line, const std::size_t sequenceNumber)
    {
        std::cerr << "Unrecoverable line gap on (" << line << ") at " << sequenceNumber << std::endl;
    }

}}
