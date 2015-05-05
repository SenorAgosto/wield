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
            , lastCount_(0)
            , lastTime_(std::chrono::high_resolution_clock::now())
        {
        }

        void operator()(message::TestMessage& message) override
        {
            if(count_++ % 10000 == 0)
            {
//                report_stats();
                std::cout << "Yo! (" << message.sequenceNumber() << ")\n";
            }
        }

    private:

        void report_stats() 
        {
            std::size_t messages = count_ - lastCount_;
            lastCount_ = count_;

            TimePoint now = std::chrono::high_resolution_clock::now();

            std::chrono::high_resolution_clock::duration duration = now - lastTime_;
            lastTime_ = now;

            // normalize the duration into seconds...
            auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count();

            if(seconds > 0)
            {
                // normalize to messages/sec.
                auto messagesPerSecond = messages / static_cast<std::size_t>(seconds);
                std::cout << messagesPerSecond << "Messages/s\n";
            }
        }

    private:

        std::size_t count_;
        std::size_t lastCount_;

        TimePoint lastTime_;
    };
}}
