#pragma once
#include <wield/platform/intrusive_ptr.hpp>

#include <atomic>
#include <cstddef>

namespace wield {

    // single threaded base class
    struct ReferenceCountBase
    {
        ReferenceCountBase()
            : referenceCount_(0)
        {
        }
        
        ReferenceCountBase(const ReferenceCountBase&)
            : referenceCount_(0)
        {
        }
        
        ReferenceCountBase(ReferenceCountBase&&)
            :referenceCount_(0)
        {
        }
        
        ReferenceCountBase& operator=(const ReferenceCountBase&)
        {
            return *this;
        }
        
        ReferenceCountBase& operator=(ReferenceCountBase&&)
        {
            return *this;
        }
        
        ~ReferenceCountBase()
        {
        }
        
        void swap(ReferenceCountBase&)
        {
        }
        
        mutable std::size_t referenceCount_;
    };
    
    // multi-threaded base class
    struct ThreadSafeReferenceCountBase
    {
        ThreadSafeReferenceCountBase()
            : referenceCount_(0)
        {
        }
        
        ThreadSafeReferenceCountBase(const ThreadSafeReferenceCountBase&)
            : referenceCount_(0)
        {
        }
        
        ThreadSafeReferenceCountBase(ThreadSafeReferenceCountBase&& base)
            : referenceCount_(0)
        {
        }
        
        ThreadSafeReferenceCountBase& operator=(const ThreadSafeReferenceCountBase&)
        {
            return *this;
        }
        
        ThreadSafeReferenceCountBase& operator=(ThreadSafeReferenceCountBase&&)
        {
            return *this;
        }
        
        void swap(ThreadSafeReferenceCountBase&)
        {
        }
        
        ~ThreadSafeReferenceCountBase()
        {
        }
        
        mutable std::atomic<std::size_t> referenceCount_;
    };

    // because the reference count base in protected we pass to versions of the pointer in,
    // the first as the Derived* type and the second as the ReferenceCountBase type so you
    // can look at the reference count.
    template<class Derived, class ReferenceCountBase>
    struct NullDebugPolicy
    {
        inline void BeforeReferenceCountIncrement(const Derived*, const ReferenceCountBase*) {}
        inline void AfterReferenceCountIncrement(const Derived*, const ReferenceCountBase*) {}
        
        inline void BeforeReferenceCountDecrement(const Derived*, const ReferenceCountBase*) {}
        inline void AfterReferenceCountDecrement(const Derived*, const ReferenceCountBase*) {}
        
        inline void BeforeDelete(const Derived*, const ReferenceCountBase*) {}
        inline void AfterDelete() {}
    };
    
    // <Derived> must inherit from <UsingIntrusiveIn>
    // <ReferenceCountBase> defaults to threadsafe.
    // We use virtual inheritance to ensure reuse of <UsingIntrusiveIn> within
    // a class hierarchy doesn't result in multiple refrence count members.
    template<class Derived, class ReferenceCountBase = ThreadSafeReferenceCountBase, class DebugPolicy = NullDebugPolicy<Derived, ReferenceCountBase>>
    struct UsingIntrusiveIn : virtual protected ReferenceCountBase
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
    DebugPolicy UsingIntrusiveIn<Derived, ReferenceCountBase, DebugPolicy>::debug = DebugPolicy();
}