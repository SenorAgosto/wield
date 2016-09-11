#pragma once

#include <wield/details/SchedulingPolicyHolder.hpp>
#include <wield/logging/Log.hpp>
#include <wield/platform/thread.hpp>
#include <wield/platform/list.hpp>

#include <algorithm>
#include <atomic>
#include <forward_list>
#include <utility>

namespace wield {

    template<class SchedulingPolicy, class SchedulingPolicyOwnershipProperty = details::PolicyIsInternalToScheduler>
    class SchedulerBase : public details::SchedulerPolicyHolder<SchedulingPolicy, SchedulingPolicyOwnershipProperty>
    {
    public:
        
        template<typename... Args>
        SchedulerBase(Args&&... args);
        
        // start the stage-based application
        void start(void);

        // wait for all processing threads to exit
        void join(void);

        // signal all processing threads to stop
        void stop(void);

    private:
        SchedulerBase(const SchedulerBase&) = delete;
        SchedulerBase& operator=(const SchedulerBase&) = delete;

        bool done(void) const;  // @returns true if the thread should stop
        
        void tryProcess(const std::size_t thread_id);
        void process(const std::size_t thread_id);

    private:
        std::forward_list<std::thread> threads_;
        std::atomic_bool done_;
    };
    
    
    template<class SchedulingPolicy, class SchedulingPolicyOwnershipProperty>
    template<typename... Args>
    SchedulerBase<SchedulingPolicy, SchedulingPolicyOwnershipProperty>::SchedulerBase(Args&&... args)
        : details::SchedulerPolicyHolder<SchedulingPolicy, SchedulingPolicyOwnershipProperty>(std::forward<Args>(args)...)
        , done_(false)
    {
    }
    
    template<class SchedulingPolicy, class SchedulingPolicyOwnershipProperty>
    void SchedulerBase<SchedulingPolicy, SchedulingPolicyOwnershipProperty>::start(void)
    {
        auto processLambda = [this](const std::size_t thread_id)
        {
            tryProcess(thread_id);
        };
        
        std::size_t numberOfThreads = this->schedulingPolicy_.numberOfThreads();
        for(std::size_t t = 0; t < numberOfThreads; ++t)
        {
            threads_.emplace_front(processLambda, t);

            // ARG: TODO: forward the thread to an affinity policy.
        }
    }

    template<class SchedulingPolicy, class SchedulingPolicyOwnershipProperty>
    inline
    void SchedulerBase<SchedulingPolicy, SchedulingPolicyOwnershipProperty>::join(void)
    {
        for(auto& t : threads_)
        {
            t.join();
        }
    }

    template<class SchedulingPolicy, class SchedulingPolicyOwnershipProperty>
    inline
    void SchedulerBase<SchedulingPolicy, SchedulingPolicyOwnershipProperty>::stop(void)
    {
        done_.store(true, std::memory_order_release);
    }

    template<class SchedulingPolicy, class SchedulingPolicyOwnershipProperty>
    inline void SchedulerBase<SchedulingPolicy, SchedulingPolicyOwnershipProperty>::tryProcess(const std::size_t thread_id)
    {
        try
        {
            while(!done())
            {
                process(thread_id);
            }
        }
        catch (const std::exception& e)
        {
            logging::Log::Error(std::string("Scheduler: an exception occurred: ") + e.what());
        }
    }

    template<class SchedulingPolicy, class SchedulingPolicyOwnershipProperty>
    inline
    bool SchedulerBase<SchedulingPolicy, SchedulingPolicyOwnershipProperty>::done(void) const
    {
        return done_.load(std::memory_order_relaxed);
    }

    template<class SchedulingPolicy, class SchedulingPolicyOwnershipProperty>
    void SchedulerBase<SchedulingPolicy, SchedulingPolicyOwnershipProperty>::process(const std::size_t thread_id)
    {
        typename SchedulingPolicy::StageType& stage = this->schedulingPolicy_.nextStage(thread_id);
        typename SchedulingPolicy::PollingInformation pollingInfo(thread_id, stage.name());
        
        this->schedulingPolicy_.batchStart(pollingInfo);
        
        do
        {
            const bool messageProcessed = stage.process();
            pollingInfo.incrementMessageCount(messageProcessed);
            
        }
        while(this->schedulingPolicy_.continueProcessing(pollingInfo));
        
        this->schedulingPolicy_.batchEnd(pollingInfo);
    }
}
