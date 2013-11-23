#pragma once
#include <wield/MessageBase.h>
#include <wield/QueueBase.h>
#include <wield/adapters/QueueAdapter.h>

#include "../platform/ConcurrentQueue.h"
#include "ProcessingFunctor.h"

namespace test_adapter {
    using Queue = wield::adapters::QueueInterface<ProcessingFunctorInterface>;
}
