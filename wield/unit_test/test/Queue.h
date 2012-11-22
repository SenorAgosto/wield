#pragma once
#include <wield/MessageBase.h>
#include <wield/QueueBase.h>

#include "platform/ConcurrentQueue.h"
#include "ProcessingFunctor.h"

typedef wield::QueueBase<ProcessingFunctor, Concurrency::concurrent_queue<Message::smartptr> > TestQueue;