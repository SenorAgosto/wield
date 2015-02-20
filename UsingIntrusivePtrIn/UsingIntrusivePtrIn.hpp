#pragma once
#include <UsingIntrusivePtrIn/details/NullIntrusivePtrDebugPolicy.hpp>
#include <UsingIntrusivePtrIn/details/ThreadSafeReferenceCountBase.hpp>

#include <UsingIntrusivePtrIn/platform/intrusive_ptr.hpp>

namespace UsingIntrusivePtrIn {

    // <Derived> must inherit from <UsingIntrusiveIn>
    // <ReferenceCountBase> defaults to threadsafe.
    // We use virtual inheritance to ensure reuse of <UsingIntrusiveIn> within
    // a class hierarchy doesn't result in multiple refrence count members.
    template<class Derived, class ReferenceCountBase = details::ThreadSafeReferenceCountBase, template<class Derived2, class ReferenceCountBase2> class DebugPolicy = details::NullIntrusivePtrDebugPolicy>
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

        using DebugPolicyType = DebugPolicy<Derived, ReferenceCountBase>;
        static DebugPolicyType& GetIntrusivePtrDebugPolicy() { return debug; }
        
        static DebugPolicyType debug;
    };

    // initialize the static variable.
    template<class Derived, class ReferenceCountBase, template<class Derived2, class ReferenceCountBase2> class DebugPolicy>
    DebugPolicy<Derived, ReferenceCountBase> UsingIntrusivePtrIn<Derived, ReferenceCountBase, DebugPolicy>::debug = DebugPolicy<Derived, ReferenceCountBase>();
}