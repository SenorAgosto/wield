#pragma once 
#include <wield/schedulers/utils/ThreadAssignments.hpp>

#include <array>
#include <cstddef>
#include <random>
#include <utility>
#include <vector>

namespace wield { namespace schedulers {

    // This class implements a scheduling policy which
    // visits stages in a random order.
    template<class StageEnum, class DispatcherType, class Stage, class PollingPolicy, std::size_t TableSizeFactor = 4>
    class RandomVisit : public PollingPolicy
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
        RandomVisit(Dispatcher& dispatcher, Args&&... args);

        // This constructor assumes the maximum concurrency of a stage is 1,
        // and creates a maximum of @maxNumberOfThreads.
        template<typename... Args>
        RandomVisit(Dispatcher& dispatcher, const MaxThreads, const std::size_t maxNumberOfThreads, Args&&... args);

        // This constructor takes the maximum concurrency information in @maxConcurrency,
        // and creates a maximum number of threads based on the maximum concurrency possible with
        // @maxConcurrency.
        template<typename... Args>
        RandomVisit(Dispatcher& dispatcher, MaxConcurrencyContainer& maxConcurrency, Args&&... args);
        
        // This constructor takes the maximum concurrency information in @maxConcurrency,
        // and creates a maximum of @maxNumberOfThreads.
        template<typename... Args>
        RandomVisit(Dispatcher& dispatcher, MaxConcurrencyContainer& maxConcurrency, const std::size_t maxNumberOfThreads, Args&&... args);

        // @return number of threads to create and schedule
        std::size_t numberOfThreads() const;

        // @return next stage to visit
        StageType& nextStage(const std::size_t threadId);

    private:
        // choose the next stage from our visit table, shuffle if needed.
        StageEnum randomStage(const std::size_t threadId);
        void randomShuffle(const std::size_t threadId);
        
        void initVisitTable();
        
    private:
        Dispatcher& dispatcher_;
        ThreadAssignments threadAssignments_;
        
        using VisitTable = std::array<StageEnum, static_cast<std::size_t>(StageEnum::NumberOfEntries) * TableSizeFactor>;
        std::vector<VisitTable> visitTable_;
        std::vector<typename VisitTable::const_iterator> visitIndex_;
        
        std::random_device randomDevice_;   // use hardware generated entropy.
        std::uniform_int_distribution<std::size_t> rand_;
    };
    

    template<class StageEnum, class DispatcherType, class Stage, class PollingPolicy, std::size_t TableSizeFactor>
    template<typename... Args>
    RandomVisit<StageEnum, DispatcherType, Stage, PollingPolicy, TableSizeFactor>::RandomVisit(Dispatcher& dispatcher, Args&&... args)
        : PollingPolicy(std::forward<Args>(args)...)
        , dispatcher_(dispatcher)
        , threadAssignments_()
        , visitTable_(threadAssignments_.size())
        , visitIndex_(threadAssignments_.size())
        , rand_(0, static_cast<std::size_t>(StageEnum::NumberOfEntries) - 1)
    {
        initVisitTable();
    }

    template<class StageEnum, class DispatcherType, class Stage, class PollingPolicy, std::size_t TableSizeFactor>
    template<typename... Args>
    RandomVisit<StageEnum, DispatcherType, Stage, PollingPolicy, TableSizeFactor>::RandomVisit(Dispatcher& dispatcher, const MaxThreads, const std::size_t maxNumberOfThreads, Args&&... args)
        : PollingPolicy(std::forward<Args>(args)...)
        , dispatcher_(dispatcher)
        , threadAssignments_(maxNumberOfThreads)
        , visitTable_(threadAssignments_.size())
        , visitIndex_(threadAssignments_.size())
        , rand_(0, static_cast<std::size_t>(StageEnum::NumberOfEntries) - 1)
    {
        initVisitTable();
    }

    template<class StageEnum, class DispatcherType, class Stage, class PollingPolicy, std::size_t TableSizeFactor>
    template<typename... Args>
    RandomVisit<StageEnum, DispatcherType, Stage, PollingPolicy, TableSizeFactor>::RandomVisit(Dispatcher& dispatcher, MaxConcurrencyContainer& maxConcurrency, Args&&... args)
        : PollingPolicy(std::forward<Args>(args)...)
        , dispatcher_(dispatcher)
        , threadAssignments_(maxConcurrency)
        , visitTable_(threadAssignments_.size())
        , visitIndex_(threadAssignments_.size())
        , rand_(0, static_cast<std::size_t>(StageEnum::NumberOfEntries) - 1)
    {
        initVisitTable();
    }

    template<class StageEnum, class DispatcherType, class Stage, class PollingPolicy, std::size_t TableSizeFactor>
    template<typename... Args>
    RandomVisit<StageEnum, DispatcherType, Stage, PollingPolicy, TableSizeFactor>::RandomVisit(Dispatcher& dispatcher, MaxConcurrencyContainer& maxConcurrency, const std::size_t maxNumberOfThreads, Args&&... args)
        : PollingPolicy(std::forward<Args>(args)...)
        , dispatcher_(dispatcher)
        , threadAssignments_(maxConcurrency, maxNumberOfThreads)
        , visitTable_(threadAssignments_.size())
        , visitIndex_(threadAssignments_.size())
        , rand_(0, static_cast<std::size_t>(StageEnum::NumberOfEntries) - 1)
    {
        initVisitTable();
    }

    template<class StageEnum, class DispatcherType, class Stage, class PollingPolicy, std::size_t TableSizeFactor>
    inline
    std::size_t RandomVisit<StageEnum, DispatcherType, Stage, PollingPolicy, TableSizeFactor>::numberOfThreads() const
    {
        return threadAssignments_.size();
    }

    template<class StageEnum, class DispatcherType, class Stage, class PollingPolicy, std::size_t TableSizeFactor>
    inline
    Stage& RandomVisit<StageEnum, DispatcherType, Stage, PollingPolicy, TableSizeFactor>::nextStage(const std::size_t threadId)
    {
        auto next = threadAssignments_.removeCurrentAssignment(threadId);
        bool hasAssignment = false;
        
        do {
            next = randomStage(threadId);
            hasAssignment = threadAssignments_.tryAssign(threadId, next);

        // TODO: implement Idle policy.
        }
        while(!hasAssignment);

        return dispatcher_[next];
    }

    template<class StageEnum, class DispatcherType, class Stage, class PollingPolicy, std::size_t TableSizeFactor>
    StageEnum RandomVisit<StageEnum, DispatcherType, Stage, PollingPolicy, TableSizeFactor>::randomStage(const std::size_t threadId)
    {
        if(visitIndex_[threadId] == visitTable_[threadId].end())
        {
            visitIndex_[threadId] = visitTable_[threadId].begin();
            randomShuffle(threadId);
        }
        
        return *visitIndex_[threadId]++;
    }

    template<class StageEnum, class DispatcherType, class Stage, class PollingPolicy, std::size_t TableSizeFactor>
    void RandomVisit<StageEnum, DispatcherType, Stage, PollingPolicy, TableSizeFactor>::randomShuffle(const std::size_t threadId)
    {
        using diff_t = typename std::iterator_traits<typename VisitTable::iterator>::difference_type;
        
        diff_t n = visitTable_[threadId].end() - visitTable_[threadId].begin();
        for(diff_t i = n - 1; i > 0; --i)
        {
            using std::swap;
            swap(visitTable_[threadId][i], visitTable_[threadId][randomDevice_() % (i + 1)]);
        }
    }
    
    template<class StageEnum, class DispatcherType, class Stage, class PollingPolicy, std::size_t TableSizeFactor>
    void RandomVisit<StageEnum, DispatcherType, Stage, PollingPolicy, TableSizeFactor>::initVisitTable()
    {
        // maximum concurrency for each stage
        const auto& maxConcurrency = threadAssignments_.maxConcurrency();
        
        std::for_each(begin(visitTable_), end(visitTable_), [this, &maxConcurrency](VisitTable& visitTable)
        {
            const auto tableSize = visitTable.size();
            
            for(std::size_t i = 0; i < tableSize; )
            {
                auto v = rand_(randomDevice_);
                if(maxConcurrency[v] != 0)
                {
                    visitTable[i] = static_cast<StageEnum>(v);
                    ++i;
                }
            }
        });
        
        const std::size_t numThreads = threadAssignments_.size();
        for(std::size_t i = 0; i < numThreads; ++i)
        {
            visitIndex_[i] = visitTable_[i].begin();
        }
    }
}}
