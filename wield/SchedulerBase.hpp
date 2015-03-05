#pragma once

#include <wield/logging/Log.hpp>
#include <wield/platform/thread>
#include <wield/platform/list>

#include <algorithm>
#include <atomic>
#include <forward_list>
#include <utility>

namespace wield {

    template<class SchedulingPolicy>
    class SchedulerBase
    {
    public:
        
        template<typename... Args>
        SchedulerBase(Args&&... arg)
            : done_(false)
            , schedulingPolicy_(std::forward<Args>(arg)...)
        {
        }
        
        ~SchedulerBase()
        {
        }

        void start(void)
        {
            auto processLambda = [this](const std::size_t thread_id)
            {
                tryProcess(thread_id);
            };
            
            std::size_t numberOfThreads = schedulingPolicy_.numberOfThreads();
            for(std::size_t t = 0; t < numberOfThreads; ++t)
            {
                threads_.emplace_front(processLambda, t);
            }
        }

        inline void join(void)
        {
            for(auto& t : threads_)
            {
                t.join();
            }
        }

        inline void stop(void)
        {
            done_.store(true, std::memory_order_release);
        }

    private:
        SchedulerBase(const SchedulerBase&) = delete;
        SchedulerBase& operator=(const SchedulerBase&) = delete;

        inline void tryProcess(const std::size_t thread_id)
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

        inline bool done(void) const
        {
            return done_.load(std::memory_order_relaxed);
        }

        inline void process(const std::size_t thread_id)
        {
            typename SchedulingPolicy::StageType& stage = schedulingPolicy_.nextStage(thread_id);
            typename SchedulingPolicy::PollingInformation pollingInfo(thread_id, stage.name());
            
            schedulingPolicy_.batchStart(pollingInfo);
            
            do
            {
                const bool messageProcessed = stage.process();
                pollingInfo.incrementMessageCount(messageProcessed);
                
            } while(schedulingPolicy_.continueProcessing(pollingInfo));
            
            schedulingPolicy_.batchEnd(pollingInfo);
        }

    private:
        std::forward_list<std::thread> threads_;
        std::atomic_bool done_;

        SchedulingPolicy schedulingPolicy_;
    };
}
