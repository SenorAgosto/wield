#include "platform/UnitTestSupport.h"

#include "test/Traits.h"
#include "test/ProcessingFunctor.h"
#include "test/Message.h"

#include <algorithm>
#include <array>

namespace {

    using namespace test;
    using Dispatcher = Traits::Dispatcher;
    using Queue = Traits::Queue;
    using Stage = Traits::Stage;
    using Message = Traits::Message;
    
    template<class ...Args>
    class AggregateProcessingFunctor : public ProcessingFunctorInterface
    {
    public:
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wmissing-braces"
        AggregateProcessingFunctor(Args ...args)
            : processingFunctors_{args...}
        {
        }
        #pragma clang diagnostic pop
        
        void operator()(Message& msg) override
        {
            for(auto func : processingFunctors_)
            {
                (*func)(msg);
            }
        }
        
        void operator()(TestMessage& msg) override
        {
            for(auto func : processingFunctors_)
            {
                (*func)(msg);
            }
        }
        
        void operator()(TestMessage2& msg) override
        {
            for(auto func : processingFunctors_)
            {
                (*func)(msg);
            }
        }
        
    private:
        std::array<ProcessingFunctorInterface*, sizeof...(Args)> processingFunctors_;
    };


    // this example shows how a user could define a processing functor
    // which is an aggregate of other processing functors and then
    // assign the aggregate processor to a stage.
    //
    // NOTE: aggregating in this way is better than using the ProcessingFunctorChain.
    // The ProcessingFunctorChain has to do double-dispatch for each processing functor
    // in the chain, here we escape that fate, double dispatch happens once & we
    // piggy back on the first dispatch once we know what the message type is.
    // 
    // WARNING: if a processing functor which is not the last in the chain
    // dispatches the message to another stage, you could end up introducing
    // a race conditions in your application (if this stage and the other
    // operate on the same data on the message).
    TEST(exampleDemonstrateUserDefinedAggregateProcessingFunctor)
    {
        Dispatcher d;
        Queue q;
        ProcessingFunctor f;
        
        // in practice, each processing functor would likely be different, here we'll call f() 3 times.
        // f -> f -> f
        AggregateProcessingFunctor<ProcessingFunctor*, ProcessingFunctor*, ProcessingFunctor*> aggregateFunctor(&f, &f, &f);
        
        Stage s(Stages::Stage1, d, q, aggregateFunctor);
        
        CHECK_EQUAL(0, f.messageBaseCallCount_);
        CHECK_EQUAL(0, f.message1CallCount_);
        CHECK_EQUAL(0, f.message2CallCount_);
        
        Message::smartptr m = new TestMessage;
        s.push(m);
        s.process();
        
        CHECK_EQUAL(0, f.messageBaseCallCount_);
        CHECK_EQUAL(3, f.message1CallCount_);
        CHECK_EQUAL(0, f.message2CallCount_);
        
        m = new TestMessage2;
        s.push(m);
        s.process();
        
        CHECK_EQUAL(0, f.messageBaseCallCount_);
        CHECK_EQUAL(3, f.message1CallCount_);
        CHECK_EQUAL(3, f.message2CallCount_);
    }
}
