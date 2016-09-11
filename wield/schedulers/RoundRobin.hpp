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
        RoundRobin(Dispatcher& dispatcher, Args&&... args);

        // @return number of threads to create and schedule
        std::size_t numberOfThreads() const;

        // @return next stage to visit
        StageType& nextStage(const std::size_t /*threadId*/);

    private:
        // calculate the next stage to visit
        StageEnumType incrementStage();
        
    private:
        Dispatcher& dispatcher_;
        StageEnumType previousStage_;
    };
    

    template<class StageEnum, class DispatcherType, class Stage, class PollingPolicy>
    template<typename... Args>
    RoundRobin<StageEnum, DispatcherType, Stage, PollingPolicy>::RoundRobin(Dispatcher& dispatcher, Args&&... args)
        : PollingPolicy(std::forward<Args>(args)...)
        , dispatcher_(dispatcher)
        , previousStage_(StageEnumType::NumberOfEntries)
    {
    }

    template<class StageEnum, class DispatcherType, class Stage, class PollingPolicy>
    inline
    std::size_t RoundRobin<StageEnum, DispatcherType, Stage, PollingPolicy>::numberOfThreads() const
    {
        return 1;
    }

    template<class StageEnum, class DispatcherType, class Stage, class PollingPolicy>
    inline
    Stage& RoundRobin<StageEnum, DispatcherType, Stage, PollingPolicy>::nextStage(const std::size_t /*threadId*/)
    {
        return dispatcher_[incrementStage()];
    }

    template<class StageEnum, class DispatcherType, class Stage, class PollingPolicy>
    StageEnum RoundRobin<StageEnum, DispatcherType, Stage, PollingPolicy>::incrementStage()
    {
        std::size_t stageIndex = static_cast<std::size_t>(previousStage_) + 1;

        if(stageIndex >= static_cast<std::size_t>(StageEnumType::NumberOfEntries))
        {
            previousStage_ = static_cast<StageEnumType>(0);
        }
        else
        {
            previousStage_ = static_cast<StageEnumType>(stageIndex);
        }

        return previousStage_;
    }

}}
