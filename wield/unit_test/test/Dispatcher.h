#pragma once
#include <wield/DispatcherBase.h>

#include "Stages.h"
#include "Stage.h"

using Dispatcher = wield::DispatcherBase<Stages, Stage>;
