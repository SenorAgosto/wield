#pragma once
#include <wield/MessageBase.h>
#include <wield/adapters/polymorphic/QueueAdapter.h>

#include "../platform/ConcurrentQueue.h"
#include "ProcessingFunctor.h"

namespace test_adapter {
    using Queue = wield::adapters::polymorphic::QueueInterface<ProcessingFunctorInterface>;
}
