#include <wield/Message.h>
#include <wield/ProcessingFunctor.h>

namespace wield {

    Message::Message()
        : reference_count_(0)
    {
    }

    Message::~Message()
    {
    }

	//virtual
	void Message::ProcessWith(ProcessingFunctor& process)
	{
		process(*this);
	}

    void intrusive_ptr_add_ref(Message* m)
    {
        ++m->reference_count_;
    }

    void intrusive_ptr_release(Message* m)
    {
        if( 0 == --m->reference_count_)
        {
            delete m;
        }
    }
}