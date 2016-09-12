#pragma once 
#include <wield/polling_policies/ExhaustivePollingPolicy.hpp>
#include <wield/schedulers/utils/NumberOfThreads.hpp>
#include <wield/schedulers/utils/ThreadAssignments.hpp>

#include <cstdint>

namespace wield { namespace schedulers { namespace color {

    // This scheduling policy implements the Color
    // scheduling algorithm. As events are dispatched
    // to a stage, that stage's name is enqueued in
    // a work queue. All threads in the system share
    // the queue.
    //
    // Caveat: <Queue> type must be concurrent. This
    // implementation of color uses a non-blocking
    // queue and will burn cores. TODO: implement
    // a back-off policy. 
    template<
          class StageEnum
        , class DispatcherType
        , class Stage
        , class Queue
        , class PollingPolicy = wield::polling_policies::ExhaustivePollingPolicy<StageEnum>>
    class Color : public PollingPolicy
    {
    public:

        using Dispatcher = DispatcherType;
        using StageType = Stage;
        using StageEnumType = StageEnum;
        using ThreadAssignments = utils::ThreadAssignments<StageEnumType>;
        using MaxConcurrencyContainer = typename ThreadAssignments::MaxConcurrencyContainer;

        
        // the concurrency map is set to one-thread per stage, and the max
        // number of threads is set to the number of stages.
        template<typename... Args>
        Color(Dispatcher& dispatcher, Queue& queue, Args&&... args);

        template<typename... Args>
        Color(Dispatcher& dispatcher, Queue& queue, const std::size_t maxNumberOfThreads, Args&&... args);

        // the number of threads will be determined by the maximum allowed by the concurrency map.
        template<typename... Args>
        Color(Dispatcher& dispatcher, Queue& queue, MaxConcurrencyContainer& maxConcurrency, Args&&... args);

        template<typename... Args>
        Color(Dispatcher& dispatcher, Queue& queue, MaxConcurrencyContainer& maxConcurrency, const std::size_t maxNumberOfThreads, Args&&... args);

        // number of threads the scheduler should create.
        std::size_t numberOfThreads() const;

        // assign the next stage to visit.
        StageType& nextStage(std::size_t threadId);

    private:
        // get the next stage from the work queue.
        StageEnumType dequeNextStage();

    private:
        Dispatcher& dispatcher_;
        Queue& workQueue_;

        ThreadAssignments threadAssignments_;
    };
    

    template<class StageEnum, class DispatcherType, class Stage, class Queue, class PollingPolicy>
    template<typename... Args>
    Color<StageEnum, DispatcherType, Stage, Queue, PollingPolicy>::Color(Dispatcher& dispatcher, Queue& queue, Args&&... args)
        : PollingPolicy(std::forward<Args>(args)...)
        , dispatcher_(dispatcher)
        , workQueue_(queue)
    {
    }

    template<class StageEnum, class DispatcherType, class Stage, class Queue, class PollingPolicy>
    template<typename... Args>
    Color<StageEnum, DispatcherType, Stage, Queue, PollingPolicy>::Color(Dispatcher& dispatcher, Queue& queue, const std::size_t maxNumberOfThreads, Args&&... args)
        : PollingPolicy(std::forward<Args>(args)...)
        , dispatcher_(dispatcher)
        , workQueue_(queue)
        , threadAssignments_(maxNumberOfThreads)
    {
    }

    template<class StageEnum, class DispatcherType, class Stage, class Queue, class PollingPolicy>
    template<typename... Args>
    Color<StageEnum, DispatcherType, Stage, Queue, PollingPolicy>::Color(Dispatcher& dispatcher, Queue& queue, MaxConcurrencyContainer& maxConcurrency, Args&&... args)
        : PollingPolicy(std::forward<Args>(args)...)
        , dispatcher_(dispatcher)
        , workQueue_(queue)
        , threadAssignments_(maxConcurrency)
    {
    }

    template<class StageEnum, class DispatcherType, class Stage, class Queue, class PollingPolicy>
    template<typename... Args>
    Color<StageEnum, DispatcherType, Stage, Queue, PollingPolicy>::Color(Dispatcher& dispatcher, Queue& queue, MaxConcurrencyContainer& maxConcurrency, const std::size_t maxNumberOfThreads, Args&&... args)
        : PollingPolicy(std::forward<Args>(args)...)
        , dispatcher_(dispatcher)
        , workQueue_(queue)
        , threadAssignments_(maxConcurrency, maxNumberOfThreads)
    {
    }

    template<class StageEnum, class DispatcherType, class Stage, class Queue, class PollingPolicy>
    inline
    std::size_t Color<StageEnum, DispatcherType, Stage, Queue, PollingPolicy>::numberOfThreads() const
    {
        return utils::numberOfThreads(threadAssignments_.size());
    }

    template<class StageEnum, class DispatcherType, class Stage, class Queue, class PollingPolicy>
    Stage& Color<StageEnum, DispatcherType, Stage, Queue, PollingPolicy>::nextStage(std::size_t threadId)
    {
        threadAssignments_.removeCurrentAssignment(threadId);

        auto next = StageEnumType::NumberOfEntries;

        do {
            next = dequeNextStage();
            const auto success = threadAssignments_.tryAssign(threadId, next);

            if(!success)
            {
                next = StageEnumType::NumberOfEntries;
            }

        // TODO: implement Idle policy.
        }
        while(next == StageEnumType::NumberOfEntries);

        return dispatcher_[next];
    }

    template<class StageEnum, class DispatcherType, class Stage, class Queue, class PollingPolicy>
    inline
    StageEnum Color<StageEnum, DispatcherType, Stage, Queue, PollingPolicy>::dequeNextStage()
    {
        StageEnumType next = StageEnumType::NumberOfEntries;
        workQueue_.try_pop(next);

        return next;
    }

}}}
