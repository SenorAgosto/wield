#pragma once 

#include <array>
#include <cstddef>
#include <numeric>

namespace wield { namespace schedulers { namespace utils {

    // This function returns the "idle" number of threads to run
    // an application with. It chooses the smaller of hardware
    // cores and number of stages, but correctly handles errors
    // in the number of reported cores.
    //
    // @numberOfStages is the number of stages in the stage graph.
    std::size_t numberOfThreads(const std::size_t numberOfStages);
    
    // This function counts the maximum concurrency possible on the
    // stage graph and passes this figure to numberOfThreads instead
    // of the stage count - therefore returning the smaller of hardware
    // cores and the maximum concurrency value.
    template<std::size_t NumberOfStages>
    std::size_t numberOfThreads(const std::array<std::size_t, NumberOfStages>& concurrencyMap)
    {
        return numberOfThreads(std::accumulate(begin(concurrencyMap), end(concurrencyMap), 0));
    }
}}}

