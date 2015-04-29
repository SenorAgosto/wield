#pragma once
#include <UsingIntrusivePtrIn/UsingIntrusivePtrIn.hpp>
#include <UsingIntrusivePtrIn/Handle.hpp>

namespace wield {

    // @note we parameterize on ProcessingFunctor so that we can support
    // double dispatch against ProcessingFunctors when not all message types
    // can be known by the wield toolkit.
    //
    // However, this also allows the interesting potential for a message's
    // processWith method to invoke non-virtual methods on the ProcessingFunctor
    // prior to dispatching into it for proecssing.
    //
    // This flexability maybe useful for updating statistics or informing
    // the scheduler of events via ProcessingFunctor - if that is how
    // client code decides to feed statistics or information into their scheduler.
    template<class ProcessingFunctor>
    class MessageBase : public UsingIntrusivePtrIn::UsingIntrusivePtrIn<MessageBase<ProcessingFunctor>>
	{
	public:
        using smartptr = UsingIntrusivePtrIn::Handle<MessageBase>;
        using ptr = MessageBase*;

        virtual ~MessageBase(){}
		virtual void processWith(ProcessingFunctor& process) = 0;

        inline void incrementReferenceCount();
    };


    template<class ProcessingFunctor>
    void MessageBase<ProcessingFunctor>::incrementReferenceCount()
    {
        intrusive_ptr_add_ref(this);
    }


    // inform boost::intrusive_ptr constructor
    // not to increment the reference count.
    static const bool no_increment = false;
}
