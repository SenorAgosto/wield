#pragma once
#include <wield/platform/intrusive_ptr.hpp>

#include <atomic>
#include <cstddef>

namespace wield {

    /// @note [ARG]: scritly speaking, we don't need to parameterize
    /// Messages on the ProcessingFunctor. We could introduce an
    /// interface and force client code ProcessingFunctors to inherit
    /// from the interface, then use the interface for ProcessWith() here.
    ///
    /// However, the current solution allows an interesting potential
    /// for a message's ProcessWith to invoke methods on the ProcessingFunctor
    /// prior dispatching into it for processing.
    ///
    /// This flexability maybe useful for updating statistics or informing
    /// the scheduler of events via ProcessingFunctor - if that is how
    /// client code decides to feed statistics or information into their scheduler.
    template<class ProcessingFunctor>
	class MessageBase
	{
	public:
        typedef boost::intrusive_ptr<MessageBase> smartptr;
        virtual ~MessageBase(){}

		virtual void ProcessWith(ProcessingFunctor& process) = 0;

    protected:
        MessageBase()
            : reference_count_(0)
        {
        }

        MessageBase(const MessageBase&)
            : reference_count_(0)
        { 
        }

        // assignment operator is default implementation
        MessageBase& operator=(const MessageBase&){}

    private:
        friend inline void intrusive_ptr_add_ref(MessageBase<ProcessingFunctor>* m)
        {
            ++m->reference_count_;
        }

        friend inline void intrusive_ptr_release(MessageBase<ProcessingFunctor>* m)
        {
            if( 0 == --m->reference_count_)
            {
                delete m;
            }
        }

    private:
        std::atomic<std::size_t> reference_count_;
    };
}
