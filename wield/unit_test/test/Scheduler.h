#pragma once
#include <wield/SchedulerBase.h>
#include <wield/platform/thread>

#include "Stages.h"
#include "Stage.h"
#include "Dispatcher.h"

#include <wield/policies/ExhaustivePollingPolicy.h>

#include <cstddef>
#include <limits>

namespace test {
    
    template<typename Dispatcher, typename PollingPolicy>
    class SchedulingPolicy final
    {
    public:
        using StageType = typename Dispatcher::StageType;
        using StageEnumType = typename Dispatcher::StageEnumType;
        using PollingInformation = typename PollingPolicy::PollingInformation;
        
        SchedulingPolicy(Dispatcher& dispatcher, const std::size_t numberOfThreads = std::thread::hardware_concurrency())
            : dispatcher_(dispatcher)
            , numberOfThreads_(numberOfThreads)
        {
        }

        typename Dispatcher::StageType& nextStage(const std::size_t /* thread_id */)
        {
            // this is where the magic happens...
            return dispatcher_[Dispatcher::StageEnumType::Stage1];
        }
        
        inline bool continueProcessing(PollingInformation& pollingInfo)
        {
            return pollingPolicy_.continueProcessing(pollingInfo);
        }
        
        inline void batchStart(PollingInformation& pollingInfo)
        {
            pollingPolicy_.batchStart(pollingInfo);
        }
        
        inline void batchEnd(PollingInformation& pollingInfo)
        {
            pollingPolicy_.batchEnd(pollingInfo);
        }
        
        inline std::size_t numberOfThreads(void) const
        {
            return numberOfThreads_;
        }

    private:
        SchedulingPolicy(const SchedulingPolicy&) = delete;
        SchedulingPolicy& operator=(const SchedulingPolicy&) = delete;

    private:
        PollingPolicy pollingPolicy_;
        Dispatcher& dispatcher_;
        std::size_t numberOfThreads_;
    };

    using PollingPolicy = wield::policies::ExhaustivePollingPolicy<Stages>;
    using Scheduler = wield::SchedulerBase<SchedulingPolicy<Dispatcher, PollingPolicy>>;
}
