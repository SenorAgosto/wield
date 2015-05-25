#pragma once
#include <queue_stress/details/ArbiterLineTraits.hpp>
#include <arbiter/SequenceArbiter.hpp>

namespace queue_stress { namespace details {

    using SequenceArbiter = arbiter::SequenceArbiter<details::ArbiterLineTraits>;
}}
