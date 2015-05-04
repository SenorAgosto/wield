#pragma once 
#include <cstddef>

namespace wield { namespace schedulers { namespace utils {

    // This function returns the "idle" number of threads to run
    // an application with. It chooses the smaller of hardware
    // cores and number of stages, but correctly handles errors
    // in the number of reported cores.
    //
    // @numberOfStages is the number of stages in the stage graph.
    std::size_t numberOfThreads(const std::size_t numberOfStages);
}}}

