#pragma once
#include <wield/details/NullIntrusivePtrDebugPolicy.hpp>
#include <wield/details/ThreadSafeReferenceCountBase.hpp>

#include <wield/platform/intrusive_ptr.hpp>

namespace wield {

    // <Derived> must inherit from <UsingIntrusiveIn>
    // <ReferenceCountBase> defaults to threadsafe.
    // We use virtual inheritance to ensure reuse of <UsingIntrusiveIn> within
    // a class hierarchy doesn't result in multiple refrence count members.
    template<class Derived, class ReferenceCountBase = details::ThreadSafeReferenceCountBase, class DebugPolicy = details::NullIntrusivePtrDebugPolicy<Derived, ReferenceCountBase>>
    struct UsingIntrusivePtrIn : virtual protected ReferenceCountBase
    {
        using smartptr = boost::intrusive_ptr<Derived>;
        
        friend inline void intrusive_ptr_add_ref(const Derived* d)
        {
            debug.BeforeReferenceCountIncrement(d, (const ReferenceCountBase*) d);
            
            ++((const ReferenceCountBase*) d)->referenceCount_;

            debug.AfterReferenceCountIncrement(d, (const ReferenceCountBase*) d);
        }
        
        friend inline void intrusive_ptr_release(const Derived* d)
        {
            debug.BeforeReferenceCountDecrement(d, (const ReferenceCountBase*)d);
            
            --((const ReferenceCountBase*) d)->referenceCount_;
            
            debug.AfterReferenceCountDecrement(d, (const ReferenceCountBase*)d);
            
            if( 0 == ((const ReferenceCountBase*) d)->referenceCount_)
            {
                debug.BeforeDelete(d, (const ReferenceCountBase*)d);
                
                delete d;
                
                debug.AfterDelete();
            }
        }

        using DebugPolicyType = DebugPolicy;
        static DebugPolicy debug;
    };

    // initialize the static variable.
    template<class Derived, class ReferenceCountBase, class DebugPolicy>
    DebugPolicy UsingIntrusivePtrIn<Derived, ReferenceCountBase, DebugPolicy>::debug = DebugPolicy();
}