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
            , lastTime_(std::chrono::high_resolution_clock::now())
        {
        }

        void operator()(message::TestMessage&) override
        {
            if(count_++ % ReportInterval == 0)
            {
                report_stats();
            }
        }

    private:

        void report_stats() 
        {
            TimePoint now = std::chrono::high_resolution_clock::now();

            std::chrono::high_resolution_clock::duration duration = now - lastTime_;
            lastTime_ = now;

            double milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
            auto messageRate = (static_cast<double>(ReportInterval) / milliseconds) * 1000;

            std::cout << messageRate << " msg/s\n";
        }

    private:
        static const std::size_t ReportInterval = 1000000;
        std::size_t count_;

        TimePoint lastTime_;
    };
}}
