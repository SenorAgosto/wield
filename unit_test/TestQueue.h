#pragma once
#include <wield/MessageBase.h>
#include <wield/QueueBase.h>
#include <concurrent_queue.h>

#include "TestProcessingFunctor.h"

typedef wield::QueueBase<ProcessingFunctor, Concurrency::concurrent_queue<Message::smartptr> > TestQueue;