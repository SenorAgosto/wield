#pragma once 
#include <wield/polling_policies/ExhaustivePollingPolicy.hpp>

#include <wield/schedulers/utils/MessageCount.hpp>
#include <wield/schedulers/utils/NumberOfThreads.hpp>
#include <wield/schedulers/utils/ThreadAssignments.hpp>

#include <cstdint>

namespace wield { namespace schedulers { namespace color_minus {

    // This scheduling policy implements the Color-
    // scheduling algorithm; Color minus the queue.
    //
    // As events are enqueued with a stage, a
    // monotonically increasing counter is incremented.
    //
    // Threads needing work, walk over the stats array
    // looking for the stage with the most work,
    // which has fewer than the maximum allowed
    // threads visiting and attempts to visit
    // that stage.
    template<
          class StageEnum
        , class DispatcherType
        , class StageType
        , class PollingPolicy = wield::polling_policies::ExhaustivePollingPolicy<StageEnum>>
    class ColorMinus : public PollingPolicy
    {
    public:

        using Dispatcher = DispatcherType;
        using MessageCount = schedulers::utils::MessageCount<StageEnum>;
        using Stage = StageType;
        using StageEnumType = StageEnum;

        using ThreadAssignments = utils::ThreadAssignments<StageEnumType, static_cast<std::size_t>(StageEnumType::NumberOfEntries)>;
        using MaxConcurrencyContainer = typename ThreadAssignments::MaxConcurrencyContainer;

        template<typename... Args>
        ColorMinus(Dispatcher& dispatcher, MessageCount& messageCounts, Args&&... args)
            : PollingPolicy(std::forward<Args>(args)...)
            , dispatcher_(dispatcher)
        {
        }

        template<typename... Args>
        ColorMinus(Dispatcher& dispatcher, MessageCount& stats, MaxConcurrencyContainer& maxConcurrency, Args&&... args)
            : PollingPolicy(std::forward<Args>(args)...)
            , dispatcher_(dispatcher)
            , stats_(stats)
            , threadAssignments_(maxConcurrency)
        {
        }

        // number of threads the scheduler should create.
        inline std::size_t numberOfThreads() const
        {
            static const std::size_t numberOfStages = static_cast<std::size_t>(StageEnumType::NumberOfEntries);
            return utils::numberOfThreads(numberOfStages);
        }

        // assign the next stage to visit.
        Stage& nextStage(std::size_t threadId)
        {
            threadAssignments_.removeCurrentAssignment(threadId);

            auto next = StageEnumType::NumberOfEntries;

            do {
                next = dequeNextStage();
                bool status = threadAssignments_.tryAssign(threadId, next);

                stats_.updatePrevious(next);
                if(!status)
                {
                    next = StageEnumType::NumberOfEntries;
                }

            // TODO: implement Idle policy.
            } while(next == StageEnumType::NumberOfEntries);

            return dispatcher_[next];
        }

    private:

        // get the stage that has the most work.
        inline StageEnumType dequeNextStage()
        {
            return stats_.highwaterStage();
        }

    private:
        Dispatcher& dispatcher_;
        MessageCount& stats_;

        ThreadAssignments threadAssignments_;
    };
}}}
