#pragma once
#include <wield/MessageBase.h>

#include "../platform/ConcurrentQueue.h"
#include "ProcessingFunctor.h"

namespace test {
    using Queue = Concurrency::concurrent_queue<Message::smartptr>;
}