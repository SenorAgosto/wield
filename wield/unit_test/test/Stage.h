#pragma once
#include <wield/StageBase.h>
#include <wield/DispatcherBase.h>

#include "ProcessingFunctor.h"
#include "Queue.h"
#include "Stages.h"

typedef wield::StageBase<Stages, ProcessingFunctor, TestQueue, wield::DispatcherBase> TestStage;
