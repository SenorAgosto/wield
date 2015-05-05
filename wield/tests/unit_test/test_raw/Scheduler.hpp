#pragma once
#include <wield/platform/thread>

#include <cstddef>
#include <limits>

namespace test_raw { namespace details {
    
    template<typename Dispatcher, typename PollingPolicy>
    class SchedulingPolicy : public PollingPolicy
    {
    public:
        using StageType = typename Dispatcher::StageType;
        using StageEnumType = typename Dispatcher::StageEnumType;

        SchedulingPolicy(Dispatcher& dispatcher, const std::size_t numberOfThreads = std::thread::hardware_concurrency())
            : dispatcher_(dispatcher)
            , numberOfThreads_(numberOfThreads)
        {
        }

        inline std::size_t numberOfThreads(void) const
        {
            return numberOfThreads_;
        }

        typename Dispatcher::StageType& nextStage(const std::size_t /* thread_id */)
        {
            // this is where the magic happens...
            return dispatcher_[Dispatcher::StageEnumType::Stage1];
        }

    private:
        SchedulingPolicy(const SchedulingPolicy&) = delete;
        SchedulingPolicy& operator=(const SchedulingPolicy&) = delete;

    private:
        Dispatcher& dispatcher_;
        std::size_t numberOfThreads_;
    };
}}
