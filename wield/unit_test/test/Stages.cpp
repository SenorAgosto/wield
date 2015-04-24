#include "./Stages.hpp"

namespace test {

    namespace {

        class LookupStages
        {
        public:
            LookupStages(){}

            std::string operator[](Stages s) const
            {
                switch(s)
                {
                case Stages::Stage1:          return "Stages::Stage1";
                case Stages::Stage2:          return "Stages::Stage2";
                case Stages::Stage3:          return "Stages::Stage3";
                case Stages::NumberOfEntries: return "Stages::NumberOfEntries";
                };

                return "Unknown Value";
            }

        } static const lookup;
    }

    std::ostream& operator<<(std::ostream& os, Stages s)
    {
        os << test::lookup[s];
        return os;
    }
}
