#pragma once

#include <algorithm>
#include <atomic>
#include <thread>
#include <list>

namespace wield {

    template<typename StageEnum, typename SchedulingPolicy>
    class SchedulerBase final
    {
    public:
        SchedulerBase()
            : done_(false)
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
                threads_.emplace_back(std::thread(processLambda));
            }

            waitForThreads();
        }

        inline void stop(void)
        {
            done_.store(true, std::memory_order_release);
        }

    private:
        inline void try_process(void) const
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

        inline void process(std::thread::id thread_id) const
        {
            size_t batchCount = 0;

            // ask the SchedulingPolicy for the next stage.
            // process until the batchCount for the stage is reached, or the stage queue is empty.
        }

        inline void waitForThreads(void)
        {
            std::for_each(threads_.begin(), threads_.end(), [](std::thread& t)
            {
                t.join();
            });
        }

    private:
        std::atomic<bool> done_;
        std::list<std::thread> threads_;
    };
}
