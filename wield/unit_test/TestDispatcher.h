#pragma once
#include <wield/DispatcherBase.h>

#include "Stages.h"
#include "TestStage.h"

typedef wield::DispatcherBase<Stages, TestStage> TestDispatcher;
