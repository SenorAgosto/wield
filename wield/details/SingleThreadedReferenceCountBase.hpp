#pragma once
#include <cstddef>

namespace wield { namespace details {
    
    // single threaded base class, for use with <UsingIntrusivePtrIn> mixin.
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
    
}}