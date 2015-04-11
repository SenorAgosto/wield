#pragma once 

namespace wield { namespace schedulers {

    // This class implements a single threaded
    // round robin scheduling policy. This is
    // a convenience policy which can be useful
    // for scheduling 'background' stages on
    // a single utility thread.
    template<class StageEnum, class DispatcherType, class Stage, class PollingPolicy>
    class RoundRobin : public PollingPolicy
    {
    public:
        using Dispatcher = DispatcherType;
        using StageType = Stage;
        using StageEnumType = StageEnum;

        template<typename... Args>
        RoundRobin(Dispatcher& dispatcher, Args&&... args)
            : PollingPolicy(std::forward<Args>(args)...)
            , dispatcher_(dispatcher)
            , previousStage_(StageEnumType::NumberOfEntries)
        {
        }

        inline std::size_t numberOfThreads() const
        {
            return 1;
        }

        inline StageType& nextStage(std::size_t threadId)
        {
            return dispatcher_[incrementStage()];
        }

    private:
        StageEnumType incrementStage()
        {
            std::size_t stageIndex = static_cast<std::size_t>(previousStage_) + 1;

            return (stageIndex >= static_cast<std::size_t>(StageEnumType::NumberOfEntries)) ?
                static_cast<StageEnumType>(0) : static_cast<StageEnumType>(stageIndex);
        }

    private:
        Dispatcher& dispatcher_;
        StageEnumType previousStage_;
    };
}}
