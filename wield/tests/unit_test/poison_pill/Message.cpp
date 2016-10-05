#include "./Message.hpp"
#include "./ProcessingFunctor.hpp"

namespace poison_pill {
    
    LineMessage::LineMessage(const std::string& line)
        : line_(line)
    {
    }
    
    //virtual
    void LineMessage::processWith(ProcessingFunctorInterface& process)
    {
        process(*this);
    }

    //virtual
    void PoisonPill::processWith(ProcessingFunctorInterface& process)
    {
        process(*this);
    }
}

