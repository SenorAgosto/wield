#include "TestMessage.h"
#include "TestProcessingFunctor.h"

//virtual
void TestMessage::ProcessWith(ProcessingFunctor& process)
{
    process(*this);
}

//virtual
void TestMessage2::ProcessWith(ProcessingFunctor& process)
{
    process(*this);
}

//virtual
void TestMessage3::ProcessWith(ProcessingFunctor& process)
{
    process(*this);
}