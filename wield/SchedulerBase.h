#pragma once

#include <wield/logging/Logging.h>

#include <algorithm>
#include <atomic>
#include <thread>
#include <list>
#include <utility>

namespace wield {

    template<typename SchedulingPolicy>
    class SchedulerBase final
    {
    public:
        SchedulerBase()
            : schedulingPolicy_() 
            , done_(false)
        {
        }

        template<typename Arg1>
        SchedulerBase(Arg1&& arg1)
            : schedulingPolicy_(std::forward<Arg1>(arg1)) 
            , done_(false)
        {
        }

        template<typename Arg1, typename Arg2>
        SchedulerBase(Arg1&& arg1, Arg2&& arg2)
            : schedulingPolicy_(std::forward<Arg1>(arg1), std::forward<Arg2>(arg2))
            , done_(false)
        {
        }

        template<typename Arg1, typename Arg2, typename Arg3>
        SchedulerBase(Arg1&& arg1, Arg2&& arg2, Arg3&& arg3)
            : schedulingPolicy_(std::forward<Arg1>(arg1), std::forward<Arg2>(arg2), std::forward<Arg3>(arg3))
            , done_(false)
        {
        }

        template<typename Arg1, typename Arg2, typename Arg3, typename Arg4>
        SchedulerBase(Arg1&& arg1, Arg2&& arg2, Arg3&& arg3, Arg4&& arg4)
            : schedulingPolicy_(std::forward<Arg1>(arg1), std::forward<Arg2>(arg2), std::forward<Arg3>(arg3), std::forward<Arg4>(arg4))
            , done_(false)
        {
        }

        // five is infinity.
        template<typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
        SchedulerBase(Arg1&& arg1, Arg2&& arg2, Arg3&& arg3, Arg4&& arg4, Arg5&& arg5)
            : schedulingPolicy_(std::forward<Arg1>(arg1), std::forward<Arg2>(arg2), std::forward<Arg3>(arg3), std::forward<Arg4>(arg4), std::forward<Arg5>(arg5))
            , done_(false)
        {
        }

        ~SchedulerBase()
        {
        }

        void start(void)
        {
            auto processLambda = [this](const std::size_t thread_id)
            {
                try_process(thread_id);
            };
            
            std::size_t numberOfThreads = schedulingPolicy_.numberOfThreads();
            for(std::size_t t = 0; t < numberOfThreads; ++t)
            {
                threads_.emplace_back(processLambda, t);
            }
        }

        inline void join(void)
        {
            std::for_each(threads_.begin(), threads_.end(), [](std::thread& t)
            {
                t.join();
            });
        }

        inline void stop(void)
        {
            done_.store(true, std::memory_order_release);
        }

    private:
        SchedulerBase(const SchedulerBase&);
        SchedulerBase& operator=(const SchedulerBase&);

        inline void try_process(const std::size_t thread_id)
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
                std::clog << logging::ErrorMessage(std::string("Scheduler: an exception occurred: ") + e.what());
            }
        }

        inline bool done(void) const
        {
            return done_.load(std::memory_order_acquire);
        }

        inline void process(const std::size_t thread_id)
        {
            register SchedulingPolicy::stage_t& stage = schedulingPolicy_.nextStage(thread_id);
            register std::size_t batchCount = schedulingPolicy_.batchSize(stage.name());
            register std::size_t emptyRetryCount = schedulingPolicy_.emptyRetryCount(stage.name());
            register bool continueProcessing = true;

            // process until the batchCount for the stage is reached, 
            // if the stage queue is empty, then retry for at least emptyRetryCount attempts
            while(continueProcessing && (batchCount > 0))
            {
                continueProcessing = stage.process();
                --batchCount;

                if(!continueProcessing && (emptyRetryCount > 0))
                {
                    continueProcessing = true;
                    --emptyRetryCount;
                }
            }
        }

    private:
        std::list<std::thread> threads_;
        std::atomic<bool> done_;

        SchedulingPolicy schedulingPolicy_;
    };
}
