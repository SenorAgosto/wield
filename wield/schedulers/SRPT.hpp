#pragma once 
#include <wield/schedulers/utils/NumberOfThreads.hpp>
#include <wield/schedulers/utils/ThreadAssignments.hpp>
#include <cstddef>

namespace wield { namespace schedulers {

    // Shortest Remaining Processing Time scheduling policy.
    // This policy attempts to process events which have the
    // shortest remaining processing time. In a stage based architecture
    // (with or without cycles) the best estimate for SRPT is to
    // process events towards the end of the pipeline first, and
    // work forward.
    //
    // This policy assumes the stage enum is laid out
    // such that stages in the beginning of the pipeline are first,
    // followed by downstream stages. If two stages appear at the same
    // depth, the stage with the greater enum value will be given preference
    // by the scheduler.
    template<class DispatcherType, class PollingPolicy>
    class SRPT : public PollingPolicy
    {
    public:
        using Dispatcher = DispatcherType;
        using PollingInformation = typename PollingPolicy::PollingInformation;
        using StageType = typename Dispatcher::StageType;
        using StageEnumType = typename Dispatcher::StageEnumType;

        using ThreadAssignments = utils::ThreadAssignments<StageEnumType>;
        using MaxConcurrencyContainer = typename ThreadAssignments::MaxConcurrencyContainer;
        
        struct MaxThreads {};   // a tag type to avoid ambigous call
        
        // This constructor assumes the maximum concurrency of a stage is 1,
        // and creates a maximum of 1 thread per stage.
        template<typename... Args>
        SRPT(Dispatcher& dispatcher, Args&&... args);

        // This constructor assumes the maximum concurrency of a stage is 1,
        // and creates a maximum of @maxNumberOfThreads.
        template<typename... Args>
        SRPT(Dispatcher& dispatcher, const MaxThreads, const std::size_t maxNumberOfThreads, Args&&... args);

        // This constructor takes the maximum concurrency information in @maxConcurrency,
        // and creates a maximum number of threads based on the maximum concurrency possible with
        // @maxConcurrency.
        template<typename... Args>
        SRPT(Dispatcher& dispatcher, MaxConcurrencyContainer& maxConcurrency, Args&&... args);
        
        // This constructor takes the maximum concurrency information in @maxConcurrency,
        // and creates a maximum of @maxNumberOfThreads.
        template<typename... Args>
        SRPT(Dispatcher& dispatcher, MaxConcurrencyContainer& maxConcurrency, const std::size_t maxNumberOfThreads, Args&&... args);

        // @return number of threads to create and schedule
        std::size_t numberOfThreads() const;

        // @return next stage to visit
        StageType& nextStage(const std::size_t threadId);

        // overload the base class batchEnd so we can collect information
        // from pollingInfo
        void batchEnd(PollingInformation& pollingInfo) { hadMessages_ = pollingInfo.hadMessage(); }

        // @return true if stage is the last stage in the enum
        bool lastStage(const StageEnumType stage) { return static_cast<std::size_t>(stage) == (static_cast<std::size_t>(StageEnumType::NumberOfEntries) - 1); }
        
    private:
        // calculate the next stage to visit
        StageEnumType decrementStage(const StageEnumType stage);
        
    private:
        Dispatcher& dispatcher_;
        ThreadAssignments threadAssignments_;
        
        bool hadMessages_;  // track whether last assigned stage had messages
    };
    

    template<class DispatcherType, class PollingPolicy>
    template<typename... Args>
    SRPT<DispatcherType, PollingPolicy>::SRPT(Dispatcher& dispatcher, Args&&... args)
        : PollingPolicy(std::forward<Args>(args)...)
        , dispatcher_(dispatcher)
        , hadMessages_(false)
    {
    }

    template<class DispatcherType, class PollingPolicy>
    template<typename... Args>
    SRPT<DispatcherType, PollingPolicy>::SRPT(Dispatcher& dispatcher, const MaxThreads, const std::size_t maxNumberOfThreads, Args&&... args)
        : PollingPolicy(std::forward<Args>(args)...)
        , dispatcher_(dispatcher)
        , threadAssignments_(maxNumberOfThreads)
        , hadMessages_(false)
    {
    }

    template<class DispatcherType, class PollingPolicy>
    template<typename... Args>
    SRPT<DispatcherType, PollingPolicy>::SRPT(Dispatcher& dispatcher, MaxConcurrencyContainer& maxConcurrency, Args&&... args)
        : PollingPolicy(std::forward<Args>(args)...)
        , dispatcher_(dispatcher)
        , threadAssignments_(maxConcurrency)
        , hadMessages_(false)
    {
    }

    template<class DispatcherType, class PollingPolicy>
    template<typename... Args>
    SRPT<DispatcherType, PollingPolicy>::SRPT(Dispatcher& dispatcher, MaxConcurrencyContainer& maxConcurrency, const std::size_t maxNumberOfThreads, Args&&... args)
        : PollingPolicy(std::forward<Args>(args)...)
        , dispatcher_(dispatcher)
        , threadAssignments_(maxConcurrency, maxNumberOfThreads)
        , hadMessages_(false)
    {
    }

    template<class DispatcherType, class PollingPolicy>
    inline
    std::size_t SRPT<DispatcherType, PollingPolicy>::numberOfThreads() const
    {
        return utils::numberOfThreads(threadAssignments_.size());
    }

    template<class DispatcherType, class PollingPolicy>
    inline
    typename DispatcherType::StageType& SRPT<DispatcherType, PollingPolicy>::nextStage(const std::size_t threadId)
    {
        using StageEnum = typename DispatcherType::StageEnumType;
        
        static const StageEnum lastStage = static_cast<StageEnum>(static_cast<std::size_t>(StageEnum::NumberOfEntries) - 1);
        const auto original = threadAssignments_.removeCurrentAssignment(threadId);
        
        // goto the second last stage if we're coming from the last stage
        const auto last = original == lastStage ? original : StageEnum::NumberOfEntries;
        auto next = hadMessages_ ? last : original;
        bool hasAssignment = false;
        
        do
        {
            next = decrementStage(next);
            hasAssignment = threadAssignments_.tryAssign(threadId, next);
            
        // TODO: implement Idle policy.
        }
        while(!hasAssignment);

        return dispatcher_[next];
    }

    template<class DispatcherType, class PollingPolicy>
    typename DispatcherType::StageEnumType SRPT<DispatcherType, PollingPolicy>::decrementStage(const typename DispatcherType::StageEnumType stage)
    {
        using StageEnumType = typename DispatcherType::StageEnumType;
        
        StageEnumType newStage = stage;
        std::size_t stageIndex = static_cast<std::size_t>(stage) - 1;

        if(stageIndex >= static_cast<std::size_t>(StageEnumType::NumberOfEntries))
        {
            stageIndex = static_cast<std::size_t>(StageEnumType::NumberOfEntries) - 1;
            newStage = static_cast<StageEnumType>(stageIndex);
        }
        else
        {
            newStage = static_cast<StageEnumType>(stageIndex);
        }

        return newStage;
    }
    
}}
