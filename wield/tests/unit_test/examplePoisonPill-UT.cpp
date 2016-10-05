#include "./platform/UnitTestSupport.hpp"

#include "./poison_pill/Traits.hpp"
#include "./poison_pill/Message.hpp"
#include "./poison_pill/ProcessingFunctor.hpp"
#include "./poison_pill/Scheduler.hpp"

#include <wield/adapters/FanoutProcessingFunctor.hpp>
#include <sstream>

namespace {

    using namespace poison_pill;
    
    using Dispatcher = Traits::Dispatcher;
    using Message = Traits::Message;
    using Scheduler = Traits::Scheduler;
    using Stage = Traits::Stage;
    using Queue = Traits::Queue;

    class FanoutProcessingFunctor : public wield::adapters::FanoutProcessingFunctor<Dispatcher, ProcessingFunctorInterface>
    {
    public:
        using wield::adapters::FanoutProcessingFunctor<Dispatcher, ProcessingFunctorInterface>::FanoutProcessingFunctor;
        
        void operator()(Message& msg) override
        {
            this->dispatch(msg);
        }
        
        void operator()(LineMessage& msg) override
        {
            this->dispatch(msg);
        }
        
        void operator()(PoisonPill& msg) override
        {
            msg.done();
            this->dispatch(msg);
        }
    };
    
    
    TEST(exampleDemonstrateUseOfPoisonPillToStopDataflowApp)
    {
        // The pipeline for this example is as follows.
        //
        //           s3
        //          /
        //  s1 -> s2
        //          \
        //           s4
        //
        // The assumption is we have a simple dataflow application
        // which needs to terminate after all inputs are consumed.
        //
        // We're using the fork in the pipeline to demonstrate, all
        // stages need to get the poison pill before having the scheduler
        // stop the processing threads.
        
        Dispatcher d;
        Scheduler scheduler(d);
        
        Queue q;
        ProcessingFunctorWithDispatcherAndNextStage<Dispatcher> pf(d, Stages::Stage2);
        Stage s1(Stages::Stage1, d, q, pf);
        
        Queue q2;
        FanoutProcessingFunctor pf2(d, {{Stages::Stage3, Stages::Stage4}});
        Stage s2(Stages::Stage2, d, q2, pf2);
        
        Queue q3;
        BaseProcessingFunctor pf3;
        Stage s3(Stages::Stage3, d, q3, pf3);
        
        Queue q4;
        BaseProcessingFunctor pf4;
        Stage s4(Stages::Stage4, d, q4, pf4);
        
        
        std::stringstream ss;
        ss << "Line 1\n"
           << "Line 2\n"
           << "Line 3\n";
        
        scheduler.start();
        
        std::string line;
        while(std::getline(ss, line))
        {
            Message::smartptr lineMessage = new LineMessage(line);
            d.dispatch(Stages::Stage1, *lineMessage);
        }
        
        Message::smartptr poisonPill = new PoisonPill([&scheduler]{ scheduler.stop(); });
        d.dispatch(Stages::Stage1, *poisonPill);
        
        scheduler.join();
        
        CHECK_EQUAL(3U, pf4.lineCount); // three line messages in our data stream
    }
}
