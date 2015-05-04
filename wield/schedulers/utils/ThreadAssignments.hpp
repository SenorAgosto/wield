#pragma once 

#include <array>
#include <atomic>
#include <cstddef>

namespace wield { namespace schedulers { namespace utils {

    template<class StageEnumType, std::size_t NumberOfThreads>
    class ThreadAssignments
    {
    public:
        static const std::size_t NumberOfStages = static_cast<std::size_t>(StageEnumType::NumberOfEntries);
        using MaxConcurrencyContainer = std::array<std::size_t, NumberOfStages>;

        // sets maximum concurrency for every stage to 1.
        ThreadAssignments();
        ThreadAssignments(const MaxConcurrencyContainer& concurrency);
        ThreadAssignments(MaxConcurrencyContainer&& concurrency);

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

    private:
        ThreadAssignments(const ThreadAssignments&) = delete;
        ThreadAssignments& operator=(const ThreadAssignments&) = delete;
        ThreadAssignments& operator=(ThreadAssignments&&) = delete;

        void init();

    private:
        std::array<StageEnumType, NumberOfThreads> threadAssignment_;
        std::array<std::atomic_size_t, NumberOfStages> threadsPerStage_;
        MaxConcurrencyContainer maximumConcurrency_;
    };


    template<class StageEnumType, std::size_t NumberOfThreads>
    ThreadAssignments<StageEnumType, NumberOfThreads>::ThreadAssignments()
    {
        // assign every element in maximumConcurrency to 1.
        for(auto& max : maximumConcurrency_)
        {
            max = 1;
        }

        init();
    }

    template<class StageEnumType, std::size_t NumberOfThreads>
    ThreadAssignments<StageEnumType, NumberOfThreads>::ThreadAssignments(const MaxConcurrencyContainer& concurrency)
        : maximumConcurrency_(concurrency)
    {
        init();
    }

    template<class StageEnumType, std::size_t NumberOfThreads>
    ThreadAssignments<StageEnumType, NumberOfThreads>::ThreadAssignments(MaxConcurrencyContainer&& concurrency)
        : maximumConcurrency_(std::move(concurrency))
    {
        init();
    }

    template<class StageEnumType, std::size_t NumberOfThreads>
    void ThreadAssignments<StageEnumType, NumberOfThreads>::init()
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

    template<class StageEnumType, std::size_t NumberOfThreads>
    StageEnumType ThreadAssignments<StageEnumType, NumberOfThreads>::currentAssignment(const std::size_t threadId)
    {
        return threadAssignment_[threadId];
    }

    template<class StageEnumType, std::size_t NumberOfThreads>
    void ThreadAssignments<StageEnumType, NumberOfThreads>::removeCurrentAssignment(const std::size_t threadId)
    {
        const auto currentAssignment = threadAssignment_[threadId];

        if(currentAssignment != StageEnumType::NumberOfEntries)
        {
            --threadsPerStage_[static_cast<std::size_t>(currentAssignment)];
            threadAssignment_[threadId] = StageEnumType::NumberOfEntries;
        }
    }

    template<class StageEnumType, std::size_t NumberOfThreads>
    bool ThreadAssignments<StageEnumType, NumberOfThreads>::tryAssign(const std::size_t threadId, StageEnumType next)
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

