#pragma once

namespace test_raw {

    // A different message base class than we normally use.
    // The purpose of this one is to help demonstrate the 
    // use of raw pointers in the queue.  
    template<class ProcessingFunctor>
    class TestRawPointerMessage 
    {
    public:
        using smartptr = TestRawPointerMessage*;
        using ptr = TestRawPointerMessage*;
        
        virtual ~TestRawPointerMessage(){}
        virtual void processWith(ProcessingFunctor& process) = 0;

        void incrementReferenceCount();
    };

    template<class ProcessingFunctor>
    void TestRawPointerMessage<ProcessingFunctor>::incrementReferenceCount()
    {
    }
}
