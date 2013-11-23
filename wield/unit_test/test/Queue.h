#pragma once
#include <wield/MessageBase.h>
#include <wield/QueueBase.h>

#include "../platform/ConcurrentQueue.h"
#include "ProcessingFunctor.h"

using Queue = wield::QueueBase<ProcessingFunctorInterface, Concurrency::concurrent_queue<Message::smartptr>>;