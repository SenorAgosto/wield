#pragma once
#include <cstddef>

namespace wield { namespace details {
    
    // single threaded base class, for use with <UsingIntrusivePtrIn> mixin.
    struct SingleThreadedReferenceCountBase
    {
        SingleThreadedReferenceCountBase()
        : referenceCount_(0)
        {
        }
        
        SingleThreadedReferenceCountBase(const SingleThreadedReferenceCountBase&)
        : referenceCount_(0)
        {
        }
        
        SingleThreadedReferenceCountBase(SingleThreadedReferenceCountBase&&)
        :referenceCount_(0)
        {
        }
        
        SingleThreadedReferenceCountBase& operator=(const SingleThreadedReferenceCountBase&)
        {
            return *this;
        }
        
        SingleThreadedReferenceCountBase& operator=(SingleThreadedReferenceCountBase&&)
        {
            return *this;
        }
        
        ~SingleThreadedReferenceCountBase()
        {
        }
        
        void swap(SingleThreadedReferenceCountBase&)
        {
        }
        
        mutable std::size_t referenceCount_;
    };
    
}}