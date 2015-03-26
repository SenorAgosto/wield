#pragma once 
#include <wield/policies/ExhaustivePollingPolicy.hpp>

namespace wield { namespace policies {

    // This class implements a thread-per-stage
    // scheduling policy. This is a convenience
    // policy which is useful to get off the
    // ground and get running without worrying
    // about performance tuning 'til later.
    template<class Traits, class PollingPolicy = wield::policies::ExhaustivePollingPolicy<typename Traits::StageEnumType>>
    class ThreadPerStageSchedulingPolicy
        : public PollingPolicy
    {
    public:
        using Dispatcher = typename Traits::Dispatcher;
        using Stage = typename Traits::Stage;
        using StageEnumType = typename Traits::StageEnumType;

        template<typename... Args>
        ThreadPerStageSchedulingPolicy(Dispatcher& dispatcher, Args&&... args)
            : PollingPolicy(std::forward<Args>(args)...)
            , dispatcher_(dispatcher)
        {
        }

        inline std::size_t numberOfThreads() const
        {
            return static_cast<std::size_t>(StageEnumType::NumberOfEntries);
        }

        inline Stage& nextStage(std::size_t threadId)
        {
            // we control the number of threads, so we know
            // we can send them to the stages of the same
            // number.
            return dispatcher_[static_cast<StageEnumType>(threadId)];
        }

    private:
        Dispatcher& dispatcher_;
    };
}}