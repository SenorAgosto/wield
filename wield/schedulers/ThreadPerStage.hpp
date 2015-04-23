#pragma once 

namespace wield { namespace schedulers {

    // This class implements a thread-per-stage
    // scheduling policy. This is a convenience
    // policy which is useful to get off the
    // ground and get running without worrying
    // about performance tuning 'til later.
    template<class StageEnum, class DispatcherType, class Stage, class PollingPolicy>
    class ThreadPerStage : public PollingPolicy
    {
    public:
        using Dispatcher = DispatcherType;
        using StageType = Stage;
        using StageEnumType = StageEnum;

        template<typename... Args>
        ThreadPerStage(Dispatcher& dispatcher, Args&&... args)
            : PollingPolicy(std::forward<Args>(args)...)
            , dispatcher_(dispatcher)
        {
        }

        inline std::size_t numberOfThreads() const
        {
            return static_cast<std::size_t>(StageEnumType::NumberOfEntries);
        }

        inline StageType& nextStage(const std::size_t threadId)
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
