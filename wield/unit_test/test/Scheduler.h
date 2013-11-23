#pragma once
#include <wield/SchedulerBase.h>
#include <wield/platform/thread>

#include "Stages.h"
#include "Stage.h"
#include "Dispatcher.h"

#include <cstddef>
#include <limits>

template<typename Dispatcher>
class SchedulingPolicy final
{
public:
    using StageType = typename Dispatcher::StageType;
    using StageEnumType = typename Dispatcher::StageEnumType;
    
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

    inline std::size_t batchSize(typename Dispatcher::stage_enum_t /* stageName */) const
    {
        // return the configured batchSize for this stage.
        return std::numeric_limits<std::size_t>::max();
    }

    inline std::size_t emptyRetryCount(typename Dispatcher::stage_enum_t /* stageName */) const
    {
        return 10;
    }
    
    inline std::size_t numberOfThreads(void) const
    {
        return numberOfThreads_;
    }

private:
    SchedulingPolicy(const SchedulingPolicy&) = delete;
    SchedulingPolicy& operator=(const SchedulingPolicy&) = delete;

private:
    Dispatcher& dispatcher_;
    std::size_t numberOfThreads_;
};

using Scheduler = wield::SchedulerBase<SchedulingPolicy<Dispatcher>>;
