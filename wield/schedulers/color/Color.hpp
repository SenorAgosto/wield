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
        , class StageType
        , class Queue
        , class PollingPolicy = wield::polling_policies::ExhaustivePollingPolicy<StageEnum>>
    class Color : public PollingPolicy
    {
    public:

        using Dispatcher = DispatcherType;
        using Stage = StageType;
        using StageEnumType = StageEnum;
        using ThreadAssignments = utils::ThreadAssignments<StageEnumType, static_cast<std::size_t>(StageEnumType::NumberOfEntries)>;
        using MaxConcurrencyContainer = typename ThreadAssignments::MaxConcurrencyContainer;


        template<typename... Args>
        Color(Dispatcher& dispatcher, Queue& queue, Args&&... args)
            : PollingPolicy(std::forward<Args>(args)...)
            , dispatcher_(dispatcher)
            , workQueue_(queue)
        {
        }

        template<typename... Args>
        Color(Dispatcher& dispatcher, Queue& queue, MaxConcurrencyContainer& maxConcurrency, Args&&... args)
            : PollingPolicy(std::forward<Args>(args)...)
            , dispatcher_(dispatcher)
            , workQueue_(queue)
            , threadAssignments_(maxConcurrency)
        {
        }

        // number of threads the scheduler should create.
        inline std::size_t numberOfThreads() const
        {
            const std::size_t numberOfStages = static_cast<std::size_t>(StageEnumType::NumberOfEntries);
            return utils::numberOfThreads(numberOfStages);
        }

        // assign the next stage to visit.
        Stage& nextStage(std::size_t threadId)
        {
            threadAssignments_.removeCurrentAssignment(threadId);

            auto next = StageEnumType::NumberOfEntries;

            do {
                next = dequeNextStage();
                auto success = threadAssignments_.tryAssign(threadId, next);

                if(!success)
                {
                    next = StageEnumType::NumberOfEntries;
                }

            // TODO: implement Idle policy.
            } while(next == StageEnumType::NumberOfEntries);

            return dispatcher_[next];
        }

    private:

        // get the next stage from the work queue.
        inline StageEnumType dequeNextStage()
        {
            StageEnumType next = StageEnumType::NumberOfEntries;
            workQueue_.try_pop(next);

            return next;
        }

    private:
        Dispatcher& dispatcher_;
        Queue& workQueue_;

        ThreadAssignments threadAssignments_;
    };
}}}
