#pragma once

namespace UsingIntrusivePtrIn { namespace details {
    
    // NullIntrusivePtrDebugPolicy is used with <UsingIntrusivePtrIn> mixin.
    //
    // Because the reference count base in protected we pass to versions of the pointer in,
    // the first as the Derived* type and the second as the ReferenceCountBase type so you
    // can look at the reference count.
    template<class Derived, class ReferenceCountBase>
    struct NullIntrusivePtrDebugPolicy
    {
        inline void BeforeReferenceCountIncrement(const Derived*, const ReferenceCountBase*) {}
        inline void AfterReferenceCountIncrement(const Derived*, const ReferenceCountBase*) {}
        
        inline void BeforeReferenceCountDecrement(const Derived*, const ReferenceCountBase*) {}
        inline void AfterReferenceCountDecrement(const Derived*, const ReferenceCountBase*) {}
        
        inline void BeforeDelete(const Derived*, const ReferenceCountBase*) {}
        inline void AfterDelete() {}
    };

}}