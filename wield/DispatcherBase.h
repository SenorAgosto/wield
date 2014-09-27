#pragma once
#include <wield/Exceptions.h>

#include <cstddef>
#include <type_traits>

namespace wield {

    /* Dispatcher
    
       This is the dispatcher class, parameterized on your applications StageEnum type
       and Stage type. The dispatcher knows about all the stages in the system and is
       passed to stages so they can get messages from their processing functor to another
       stage in the system. Routing is done by StageEnum which defines the stage names.
    */
    template<typename StageEnum, class Stage>
    class DispatcherBase
    {
    public:
        static_assert(std::is_enum<StageEnum>::value, "StageEnum parameter is not an enum type.");

        using StageType = Stage;
        using StageEnumType = StageEnum;
        
        DispatcherBase()
        {
            memset(stages, 0, sizeof(stages));
        }

        ~DispatcherBase()
        {
        }

        /* Register a <Stage> with the Dispatcher
           @stageName is the name this stage will be referenced as
           @stage is the pointer to the stage
        */
        inline void registerStage(StageEnumType stageName, StageType* stage)
        {
            if(nullptr != stages[static_cast<std::size_t>(stageName)])
            {
                throw wield::DuplicateStageRegistrationException(); 
            }

            stages[static_cast<std::size_t>(stageName)] = stage;
        }

        /* Send a message to a stage
           @stageName the stage to dispatch the message to.
           @message is the message to send
        */
        inline void dispatch(StageEnumType stageName, typename StageType::MessageType& message)
        {
            stages[static_cast<std::size_t>(stageName)]->push( typename StageType::MessageType::smartptr(&message) );
        }

        /* Stage lookup function
           @stageName is the name of the stage to get a reference to.
         
           @return a reference to the requested stage.
        */
        inline StageType& operator[](StageEnumType stageName)
        {
            return *stages[static_cast<std::size_t>(stageName)];
        }

    private:
        DispatcherBase(const DispatcherBase&) = delete;
        DispatcherBase& operator=(const DispatcherBase&) = delete;
        
    private:
        StageType* stages[static_cast<std::size_t>(StageEnum::NumberOfEntries)];
    };
}
