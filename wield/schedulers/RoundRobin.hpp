#pragma once 
#include <wield/schedulers/utils/ThreadAssignments.hpp>
#include <cstddef>

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

        using ThreadAssignments = utils::ThreadAssignments<StageEnumType>;
        using MaxConcurrencyContainer = typename ThreadAssignments::MaxConcurrencyContainer;
        
        struct MaxThreads {};   // a tag type to avoid ambigous call
        
        // This constructor assumes the maximum concurrency of a stage is 1,
        // and creates a maximum of 1 thread per stage.
        template<typename... Args>
        RoundRobin(Dispatcher& dispatcher, Args&&... args);

        // This constructor assumes the maximum concurrency of a stage is 1,
        // and creates a maximum of @maxNumberOfThreads.
        template<typename... Args>
        RoundRobin(Dispatcher& dispatcher, const MaxThreads, const std::size_t maxNumberOfThreads, Args&&... args);

        // This constructor takes the maximum concurrency information in @maxConcurrency,
        // and creates a maximum number of threads based on the maximum concurrency possible with
        // @maxConcurrency.
        template<typename... Args>
        RoundRobin(Dispatcher& dispatcher, MaxConcurrencyContainer& maxConcurrency, Args&&... args);
        
        // This constructor takes the maximum concurrency information in @maxConcurrency,
        // and creates a maximum of @maxNumberOfThreads.
        template<typename... Args>
        RoundRobin(Dispatcher& dispatcher, MaxConcurrencyContainer& maxConcurrency, const std::size_t maxNumberOfThreads, Args&&... args);

        // @return number of threads to create and schedule
        std::size_t numberOfThreads() const;

        // @return next stage to visit
        StageType& nextStage(const std::size_t threadId);

    private:
        // calculate the next stage to visit
        StageEnumType incrementStage(const StageEnumType stage);
        
    private:
        Dispatcher& dispatcher_;
        ThreadAssignments threadAssignments_;
    };
    

    template<class StageEnum, class DispatcherType, class Stage, class PollingPolicy>
    template<typename... Args>
    RoundRobin<StageEnum, DispatcherType, Stage, PollingPolicy>::RoundRobin(Dispatcher& dispatcher, Args&&... args)
        : PollingPolicy(std::forward<Args>(args)...)
        , dispatcher_(dispatcher)
    {
    }

    template<class StageEnum, class DispatcherType, class Stage, class PollingPolicy>
    template<typename... Args>
    RoundRobin<StageEnum, DispatcherType, Stage, PollingPolicy>::RoundRobin(Dispatcher& dispatcher, const MaxThreads, const std::size_t maxNumberOfThreads, Args&&... args)
        : PollingPolicy(std::forward<Args>(args)...)
        , dispatcher_(dispatcher)
        , threadAssignments_(maxNumberOfThreads)
    {
    }

    template<class StageEnum, class DispatcherType, class Stage, class PollingPolicy>
    template<typename... Args>
    RoundRobin<StageEnum, DispatcherType, Stage, PollingPolicy>::RoundRobin(Dispatcher& dispatcher, MaxConcurrencyContainer& maxConcurrency, Args&&... args)
        : PollingPolicy(std::forward<Args>(args)...)
        , dispatcher_(dispatcher)
        , threadAssignments_(maxConcurrency)
    {
    }

    template<class StageEnum, class DispatcherType, class Stage, class PollingPolicy>
    template<typename... Args>
    RoundRobin<StageEnum, DispatcherType, Stage, PollingPolicy>::RoundRobin(Dispatcher& dispatcher, MaxConcurrencyContainer& maxConcurrency, const std::size_t maxNumberOfThreads, Args&&... args)
        : PollingPolicy(std::forward<Args>(args)...)
        , dispatcher_(dispatcher)
        , threadAssignments_(maxConcurrency, maxNumberOfThreads)
    {
    }

    template<class StageEnum, class DispatcherType, class Stage, class PollingPolicy>
    inline
    std::size_t RoundRobin<StageEnum, DispatcherType, Stage, PollingPolicy>::numberOfThreads() const
    {
        return threadAssignments_.size();
    }

    template<class StageEnum, class DispatcherType, class Stage, class PollingPolicy>
    inline
    Stage& RoundRobin<StageEnum, DispatcherType, Stage, PollingPolicy>::nextStage(const std::size_t threadId)
    {
        auto next = threadAssignments_.removeCurrentAssignment(threadId);
        bool hasAssignment = false;
        
        do {
            next = incrementStage(next);    // if this stage is busy, move to the next.
            hasAssignment = threadAssignments_.tryAssign(threadId, next);

        // TODO: implement Idle policy.
        }
        while(!hasAssignment);

        return dispatcher_[next];
    }

    template<class StageEnum, class DispatcherType, class Stage, class PollingPolicy>
    StageEnum RoundRobin<StageEnum, DispatcherType, Stage, PollingPolicy>::incrementStage(const StageEnum stage)
    {
        StageEnumType newStage = stage;
        std::size_t stageIndex = static_cast<std::size_t>(stage) + 1;

        if(stageIndex >= static_cast<std::size_t>(StageEnumType::NumberOfEntries))
        {
            newStage = static_cast<StageEnumType>(0);
        }
        else
        {
            newStage = static_cast<StageEnumType>(stageIndex);
        }

        return newStage;
    }

}}
