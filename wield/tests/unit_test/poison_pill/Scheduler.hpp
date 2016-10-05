#pragma once
#include <wield/schedulers/ThreadPerStage.hpp>

namespace poison_pill {

    template<typename Dispatcher, typename PollingPolicy>
    using SchedulingPolicy = wield::schedulers::ThreadPerStage<Dispatcher, PollingPolicy>;
}
