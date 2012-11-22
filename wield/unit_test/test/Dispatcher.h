#pragma once
#include <wield/DispatcherBase.h>

#include "Stages.h"
#include "Stage.h"

typedef wield::DispatcherBase<Stages, TestStage> TestDispatcher;
