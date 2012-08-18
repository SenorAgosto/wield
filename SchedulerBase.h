#pragma once

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
        SchedulerBase(Arg1 arg1)
            : schedulingPolicy_(std::forward<Arg1>(arg1)) 
            , done_(false)
        {
        }

        template<typename Arg1, typename Arg2>
        SchedulerBase(Arg1 arg1, Arg2 arg2)
            : schedulingPolicy_(std::forward<Arg1>(arg1), std::forward<Arg2>(arg2))
            , done_(false)
        {
        }

        template<typename Arg1, typename Arg2, typename Arg3>
        SchedulerBase(Arg1 arg1, Arg2 arg2, Arg3 arg3)
            : schedulingPolicy_(std::forward<Arg1>(arg1), std::forward<Arg2>(arg2), std::forward<Arg3>(arg3))
            , done_(false)
        {
        }

        template<typename Arg1, typename Arg2, typename Arg3, typename Arg4>
        SchedulerBase(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4)
            : schedulingPolicy_(std::forward<Arg1>(arg1), std::forward<Arg2>(arg2), std::forward<Arg3>(arg3), std::forward<Arg4>(arg4))
            , done_(false)
        {
        }

        template<typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
        SchedulerBase(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5)
            : schedulingPolicy_(std::forward<Arg1>(arg1), std::forward<Arg2>(arg2), std::forward<Arg3>(arg3), std::forward<Arg4>(arg4), std::forward<Arg5>(arg5))
            , done_(false)
        {
        }

        ~SchedulerBase()
        {
        }

        void start(size_t numberOfThreads = std::thread::hardware_concurrency() )
        {
            auto processLambda = [this]()
            {
                try_process();
            };

            for(size_t t = 0; t < numberOfThreads; ++t)
            {
                threads_.emplace_back(processLambda);
            }

            waitForThreads();
        }

        inline void stop(void)
        {
            done_.store(true, std::memory_order_release);
        }

    private:
        inline void try_process(void)
        {
            try
            {
                std::thread::id thread_id = std::this_thread::get_id();
                while(!done())
                {
                    process(thread_id);
                }
            }
            catch (const std::exception&)
            {
                // TODO: log the exception
            }
        }

        inline bool done(void) const
        {
            return done_.load(std::memory_order_acquire);
        }

        inline void process(std::thread::id thread_id)
        {
            register SchedulingPolicy::stage_t& stage = schedulingPolicy_.nextStage(thread_id);
            register size_t batchCount = schedulingPolicy_.batchSize(stage.name());
            register size_t emptyRetryCount = schedulingPolicy_.emptyRetryCount(stage.name());
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

        inline void waitForThreads(void)
        {
            std::for_each(threads_.begin(), threads_.end(), [](std::thread& t)
            {
                t.join();
            });
        }

    private:
        std::list<std::thread> threads_;
        std::atomic<bool> done_;

        SchedulingPolicy schedulingPolicy_;
    };
}
