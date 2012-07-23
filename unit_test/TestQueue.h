#pragma once
#include <wield/MessageBase.h>
#include <wield/QueueBase.h>
#include <queue>

#include "TestProcessingFunctor.h"

typedef wield::QueueBase<ProcessingFunctor, std::queue<Message::smartptr> > TestQueue;
