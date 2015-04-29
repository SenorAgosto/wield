#pragma once 

namespace wield { namespace details {

    // primary template for raw pointers, ignores
    // the no_increment parameter.
    template<typename MessageType, bool is_raw>
    struct SmartPtrCreatorImpl
    {
        static inline
        typename MessageType::smartptr create(typename MessageType::ptr p, bool)
        {
            return p;
        }
    };

    // partial specialization is for boost::intrusive_ptr types,
    // forwards the no_increment parameter into the constructor.
    template<typename MessageType>
    struct SmartPtrCreatorImpl<MessageType, false>
    {
        static inline
        typename MessageType::smartptr create(typename MessageType::ptr p, bool no_increment)
        {
            return typename MessageType::smartptr(p, no_increment);
        }
    };

    template<typename MessageType>
    struct SmartPtrCreator
        : public SmartPtrCreatorImpl<MessageType, std::is_pointer<typename MessageType::smartptr>::value>
    {
    };

    // Helper function that instantiates the proper class and invokes
    // static function
    template<typename MessageType>
    inline typename MessageType::smartptr create_smartptr(typename MessageType::ptr p, bool no_increment)
    {
        return SmartPtrCreator<MessageType>::create(p, no_increment);
    }

}}
