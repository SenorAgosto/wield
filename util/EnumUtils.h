#pragma once
#include <string>
#include <type_traits>

namespace wield { namespace util {

    // given an array of conversion strings and an enum type, convert a string to the associated enum value
    template<typename EnumType, typename EnumStrings>
    EnumType convertStringToEnum(const std::string& enumString, EnumStrings enumStrings)
    {
        typedef typename std::underlying_type<EnumType>::type underlying_type;
        for(underlying_type i = 0; i < static_cast<underlying_type>(EnumType::NumberOfEntries); ++i)
        {
            if(enumString == enumStrings[static_cast<size_t>(i)])
            {
                return static_cast<EnumType>(i);
            }
        }

        return EnumType::NumberOfEntries;
    }

    // given an enum type and an array of conversion strings, this function returns the string associated with the enum 
    template<typename EnumType, typename EnumStrings>
    std::string convertEnumToString(EnumType e, EnumStrings enumStrings)
    {
        return std::string(enumStrings[static_cast<size_t>(e)]);
    }

    // convert the EnumType to T.
    template<typename EnumType, typename T>
    T convertEnumTo(EnumType e)
    {
        static_assert(false, "conevrsionMapping not overloaded");
        return T();
    }
}}
