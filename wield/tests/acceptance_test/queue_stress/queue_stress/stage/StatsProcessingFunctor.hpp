#pragma once 
#include <queue_stress/ProcessingFunctorBase.hpp>
#include <queue_stress/TimePoint.hpp>

#include <queue_stress/message/TestMessage.hpp>

#include <chrono>
#include <iostream>

namespace queue_stress { namespace stage {

    class StatsProcessingFunctor : public ProcessingFunctorBase
    {
    public:
        StatsProcessingFunctor()
            : count_(0)
        {
        }

        void operator()(message::TestMessage&) override
        {
            // if we don't do any logic here,
            // we actually run slower because
            // of the effect of cache-line ping-pong
            // on the message reference count.
            count_++;
        }

    private:
        static const std::size_t ReportInterval = 1000000;
        std::size_t count_;
    };
}}
