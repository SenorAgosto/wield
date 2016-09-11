#pragma once 

#include <array>
#include <atomic>
#include <cstddef>
#include <numeric>
#include <vector>

namespace wield { namespace schedulers { namespace utils {

    template<class StageEnumType>
    class ThreadAssignments
    {
    public:
        static const std::size_t NumberOfStages = static_cast<std::size_t>(StageEnumType::NumberOfEntries);
        using MaxConcurrencyContainer = std::array<std::size_t, NumberOfStages>;

        // sets maximum concurrency for every stage to 1.
        ThreadAssignments(const std::size_t numThreads = static_cast<std::size_t>(StageEnumType::NumberOfEntries));
        
        ThreadAssignments(const MaxConcurrencyContainer& concurrency); // number of threads determined by concurrency map @concurrency.
        ThreadAssignments(const MaxConcurrencyContainer& concurrency, const std::size_t numThreads);
        
        ThreadAssignments(MaxConcurrencyContainer&& concurrency); // number of threads determined by concurrency map @concurrency.
        ThreadAssignments(MaxConcurrencyContainer&& concurrency, const std::size_t numThreads);
        
        // return the current assignment for a thread.
        StageEnumType currentAssignment(const std::size_t threadId);

        // remove the thread from the visitor count at its current assignment.
        inline void removeCurrentAssignment(const std::size_t threadId);

        // assign thread as a visitor to next stage if
        // the stage has room for another.
        //
        // On successful assignment, the visitor count of next
        // will be incremented.
        //
        // On failure, the visitor count of @next will be unchanged.
        //
        // @return true on success, false on failure.
        bool tryAssign(const std::size_t threadId, const StageEnumType next);

        // return the maximum number of threads we can support
        std::size_t size() const { return threadAssignment_.size(); }
        
    private:
        ThreadAssignments(const ThreadAssignments&) = delete;
        ThreadAssignments& operator=(const ThreadAssignments&) = delete;
        ThreadAssignments& operator=(ThreadAssignments&&) = delete;

        void init();

    private:
        std::vector<StageEnumType> threadAssignment_;
        std::array<std::atomic_size_t, NumberOfStages> threadsPerStage_;
        MaxConcurrencyContainer maximumConcurrency_;
    };


    template<class StageEnumType>
    ThreadAssignments<StageEnumType>::ThreadAssignments(const std::size_t numberOfThreads)
        : threadAssignment_(numberOfThreads, StageEnumType::NumberOfEntries)
    {
        // assign every element in maximumConcurrency to 1.
        for(auto& max : maximumConcurrency_)
        {
            max = 1;
        }

        init();
    }

    template<class StageEnumType>
    ThreadAssignments<StageEnumType>::ThreadAssignments(const MaxConcurrencyContainer& concurrency)
        : threadAssignment_(std::accumulate(begin(concurrency), end(concurrency), 0), StageEnumType::NumberOfEntries)
        , maximumConcurrency_(concurrency)
    {
        init();
    }
    
    template<class StageEnumType>
    ThreadAssignments<StageEnumType>::ThreadAssignments(const MaxConcurrencyContainer& concurrency, const std::size_t numberOfThreads)
        : threadAssignment_(numberOfThreads, StageEnumType::NumberOfEntries)
        , maximumConcurrency_(concurrency)
    {
        init();
    }

    template<class StageEnumType>
    ThreadAssignments<StageEnumType>::ThreadAssignments(MaxConcurrencyContainer&& concurrency)
        : threadAssignment_(std::accumulate(begin(concurrency), end(concurrency), 0), StageEnumType::NumberOfEntries)
        , maximumConcurrency_(std::move(concurrency))
    {
        init();
    }

    template<class StageEnumType>
    ThreadAssignments<StageEnumType>::ThreadAssignments(MaxConcurrencyContainer&& concurrency, const std::size_t numberOfThreads)
        : threadAssignment_(numberOfThreads, StageEnumType::NumberOfEntries)
        , maximumConcurrency_(std::move(concurrency))
    {
        init();
    }

    template<class StageEnumType>
    void ThreadAssignments<StageEnumType>::init()
    {
		for(auto& t : threadsPerStage_)
		{
			t = 0;
		}
    }

    template<class StageEnumType>
    StageEnumType ThreadAssignments<StageEnumType>::currentAssignment(const std::size_t threadId)
    {
        return threadAssignment_[threadId];
    }

    template<class StageEnumType>
    void ThreadAssignments<StageEnumType>::removeCurrentAssignment(const std::size_t threadId)
    {
        const auto currentAssignment = threadAssignment_[threadId];

        if(currentAssignment != StageEnumType::NumberOfEntries)
        {
            --threadsPerStage_[static_cast<std::size_t>(currentAssignment)];
            threadAssignment_[threadId] = StageEnumType::NumberOfEntries;
        }
    }

    template<class StageEnumType>
    bool ThreadAssignments<StageEnumType>::tryAssign(const std::size_t threadId, StageEnumType next)
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
                return true;
            }
        }

        return false;
    }
}}}

