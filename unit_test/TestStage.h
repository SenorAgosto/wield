#pragma once
#include <wield/StageBase.h>

#include "TestProcessingFunctor.h"
#include "TestQueue.h"
#include "Stages.h"

typedef wield::StageBase<Stages, ProcessingFunctor, TestQueue, wield::DispatcherBase> TestStage;
