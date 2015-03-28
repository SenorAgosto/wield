#pragma once 
#include <wield/polling_policies/ExhaustivePollingPolicy.hpp>

#include <array>
#include <atomic>
#include <cstdint>
#include <thread>

namespace wield { namespace schedulers { namespace color {

    // This scheduling policy implements the Color
    // scheduling algorithm. As events are dispatched
    // to a stage, that stage's name is enqueued in
    // a work queue. All threads in the system share
    // the queue. When they are done.
    //
    // Caveat: <Queue> type must be concurrent.
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
        using MaxConcurrencyContainer = std::array<std::size_t, static_cast<std::size_t>(StageEnumType::NumberOfEntries)>;

        template<typename... Args>
        Color(Dispatcher& dispatcher, Queue& queue, MaxConcurrencyContainer& maxConcurrency, Args&&... args)
            : PollingPolicy(std::forward<Args>(args)...)
            , dispatcher_(dispatcher)
            , workQueue_(queue)
            , maximumConcurrency_(maxConcurrency)
            , numCores_(0)
        {
            for(auto& t : threadsPerStage_)
            {
                t = 0;
            }

            for(auto& a : threadAssignment_)
            {
                a = StageEnumType::NumberOfEntries;
            }
        }

        // number of threads the scheduler should create.
        inline std::size_t numberOfThreads() const
        {
            numCores_ = std::thread::hardware_concurrency();
            numCores_ = std::max(1, numCores_);

            numCores_ = std::min(numCores_, StageEnumType::NumberOfEntries);
            return numCores_;
        }

        // assign the next stage to visit.
        Stage& nextStage(std::size_t threadId)
        {
            removeCurrentAssignment(threadId);

            auto next = StageEnumType::NumberOfEntries;

            do {
                next = dequeNextStage();
                tryAssign(threadId, next);

            // TODO: implement Idle policy.
            } while(next == StageEnumType::NumberOfEntries);

            return dispatcher_[next];
        }

    private:
        // remove this thread from the visitor count at its
        // current stage.
        inline void removeCurrentAssignment(std::size_t threadId)
        {
            const auto currentAssignment = threadAssignment_[threadId];

            if(currentAssignment != StageEnumType::NumberOfEntries)
            {
                --threadsPerStage_[static_cast<std::size_t>(currentAssignment)];
                threadAssignment_[threadId] = StageEnumType::NumberOfEntries;
            }
        }

        // assign thread as a visitor to next stage if
        // the stage has room for another.
        //
        // On successful assignment, the visitor count of next
        // will be incremented.
        //
        // On failure, the visitor count of @next will be unchanged,
        // and @next will be reset to StageEnumType::NumberOfEntries.
        void tryAssign(std::size_t threadId, StageEnumType& next)
        {
            const std::size_t nextIndex = static_cast<std::size_t>(next);

            const std::size_t maxVisitors = maximumConcurrency_[nextIndex];
            std::size_t currentVisitors = threadsPerStage_[nextIndex];

            if(currentVisitors < maxVisitors)
            {
                bool success = threadsPerStage_[nextIndex].compare_exchange_strong(currentVisitors, currentVisitors + 1);
                if(success)
                {
                    threadAssignment_[threadId] = next;
                    return;
                }
            }

            next = StageEnumType::NumberOfEntries;
        }

        // get the next stage from the work queue.
        inline StageEnumType dequeNextStage()
        {
            StageEnumType next = StageEnumType::NumberOfEntries;
            workQueue_.try_pop(next);

            return next;
        }

        // iterate through the assignment array
        // return the first stage without
        // a thread assigned.
        StageEnumType findNextAvailableStage() const
        {
            std::size_t index = 0;

            for(auto workerCount : threadsPerStage_)
            {
                // change this to use a relaxed load
                // instead of strict memory barrier.
                if((index < numCores_) && (workerCount < maximumConcurrency_[index]))
                {
                    return static_cast<StageEnumType>(index);
                }

                index++;
            }

            return StageEnumType::NumberOfEntries;
        }

    private:
        Dispatcher& dispatcher_;
        Queue& workQueue_;

        std::array<std::atomic_size_t, static_cast<std::size_t>(StageEnumType::NumberOfEntries)> threadsPerStage_;
        MaxConcurrencyContainer maximumConcurrency_;

        std::array<StageEnumType, static_cast<std::size_t>(StageEnumType::NumberOfEntries)> threadAssignment_;


        // save the number of cores, it saves us from
        // needing to traverse all of the assignments_
        // array in the event we have fewer threads
        // than stages.
        mutable std::size_t numCores_;
    };
}}}
