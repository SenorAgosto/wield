#include "./platform/UnitTestSupport.hpp"

#include <wield/adapters/FanoutProcessingFunctor.hpp>
#include <wield/CloneMessageTag.hpp>

#include "./test/Traits.hpp"
#include "./test/ProcessingFunctor.hpp"
#include "./test/Message.hpp"

namespace {
    
    using namespace test;
    using Dispatcher = Traits::Dispatcher;
    using Queue = Traits::Queue;
    using Stage = Traits::Stage;
    using Message = Traits::Message;
    
    class FanoutProcessingFunctor : public wield::adapters::FanoutProcessingFunctor<Dispatcher, ProcessingFunctorInterface>
    {
    public:
        using wield::adapters::FanoutProcessingFunctor<Dispatcher, ProcessingFunctorInterface>::FanoutProcessingFunctor;
        
        void operator() (Message& m) override
        {
            this->dispatch(m);
        }
        
        void operator()(TestMessage& m) override
        {
            this->dispatch(m);
        }
        
        void operator()(TestMessage2& m) override
        {
            this->dispatch(m, wield::clone_message);
        }
    };
    
    struct FanoutProcessingFunctorFixture
    {
        FanoutProcessingFunctorFixture()
            : fanout(d, {{Stages::Stage2, Stages::Stage3}})
            , s1(Stages::Stage1, d, q1, fanout)
            , s2(Stages::Stage2, d, q2, pf2)
            , s3(Stages::Stage3, d, q3, pf3)
        {
        }
        
        Dispatcher d;

        Queue q1;
        FanoutProcessingFunctor fanout;
        Stage s1;
        
        Queue q2;
        ProcessingFunctor pf2;
        Stage s2;
        
        Queue q3;
        ProcessingFunctor pf3;
        Stage s3;
    };
    
    TEST_FIXTURE(FanoutProcessingFunctorFixture, verifyInstantiation)
    {
    }
    
    TEST_FIXTURE(FanoutProcessingFunctorFixture, verifyFanoutOnProcessing)
    {
        Message::smartptr m = new TestMessage();
        d.dispatch(Stages::Stage1, *m);
        
        CHECK(s1.process());   // this should cause the message to be dispatched to s2 & s3.
        
        CHECK(s2.process());
        CHECK(s3.process());
        
        CHECK(pf2.message1Called_); // ensure we processed a TestMessage
        CHECK(pf3.message1Called_); // ensure we processed a TestMessage
    }

    TEST_FIXTURE(FanoutProcessingFunctorFixture, verifyFanoutConfigurationCanBeChanged)
    {
        Message::smartptr m = new TestMessage2();
        d.dispatch(Stages::Stage1, *m);
        
        CHECK(s1.process());    // dispatch m to s2 & s3
        CHECK(s2.process());
        CHECK(s3.process());
        
        CHECK_EQUAL(1U, pf2.message2CallCount_);
        CHECK_EQUAL(1U, pf3.message2CallCount_);
        
        const std::vector<Stages> stages = {Stages::Stage3};
        fanout.updateStages(stages);
        
        // send m to s1 again
        d.dispatch(Stages::Stage1, *m);
        
        CHECK(s1.process());    // processing now dispatches only to s3
        CHECK(!s2.process());   // s2 should not get m this time
        CHECK(s3.process());
        
        CHECK_EQUAL(1U, pf2.message2CallCount_);
        CHECK_EQUAL(2U, pf3.message2CallCount_);
    }

    TEST_FIXTURE(FanoutProcessingFunctorFixture, verifyFanoutConfigurationCanBeChangedWithRvalueReference)
    {
        Message::smartptr m = new TestMessage();
        d.dispatch(Stages::Stage1, *m);
        
        CHECK(s1.process());    // dispatch m to s2 & s3
        CHECK(s2.process());
        CHECK(s3.process());
        
        CHECK_EQUAL(1U, pf2.message1CallCount_);
        CHECK_EQUAL(1U, pf3.message1CallCount_);
        
        fanout.updateStages({Stages::Stage3});
        
        // send m to s1 again
        d.dispatch(Stages::Stage1, *m);
        
        CHECK(s1.process());    // processing now dispatches only to s3
        CHECK(!s2.process());   // s2 should not get m this time
        CHECK(s3.process());
        
        CHECK_EQUAL(1U, pf2.message1CallCount_);
        CHECK_EQUAL(2U, pf3.message1CallCount_);
    }
    
    TEST_FIXTURE(FanoutProcessingFunctorFixture, verifyFanoutOnProcessingWithClonedMessage)
    {
        Message::smartptr m = new TestMessage2();
        d.dispatch(Stages::Stage1, *m);
        
        CHECK(s1.process());   // this should cause the message to be dispatched to s2 & s3.
        
        CHECK(s2.process());
        CHECK(s3.process());
        
        CHECK(pf2.message2Called_); // ensure we processed a TestMessage
        CHECK(pf3.message2Called_); // ensure we processed a TestMessage
    }
}
