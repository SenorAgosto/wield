#include <wield/schedulers/utils/NumberOfThreads.hpp>

#include <algorithm>
#include <thread>

namespace wield { namespace schedulers { namespace utils {

    std::size_t numberOfThreads(const std::size_t numberOfStages)
    {
        std::size_t numCores = std::max<std::size_t>(1, std::thread::hardware_concurrency());
        return std::min(numCores, numberOfStages);
    }
}}}