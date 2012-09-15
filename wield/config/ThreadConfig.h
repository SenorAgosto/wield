#pragma once
#include <wield/config/ThreadPriorityEnum.h>
#include <wield/util/EnumUtils.h>

namespace wield { namespace config {

    template<typename StageGroupEnum>
    struct ThreadConfig
    {
        int32_t threadNum;
        ThreadPriorityEnum priority;
        std::vector<StageGroupEnum> stageGroups;

        void clear(void)
        {
            stageGroups.clear();
        }

        friend void operator>>(const YAML::Node& node, ThreadConfig& threadConfig)
        {
            node["thread"] >> threadConfig.threadNum;

            std::string priorityValue;
            node["priority"] >> priorityValue;
            threadConfig.priority = wield::util::convertStringToEnum<ThreadPriorityEnum>(priorityValue, ThreadPriorityStrings);

            std::vector<StageGroupEnum>& stageGroups = threadConfig.stageGroups;
            const YAML::Node& groups = node["stageGroups"];
            for(unsigned i = 0; i < groups.size(); ++i)
            {
                std::string stageGroupValue;
                groups[i] >> stageGroupValue;

                StageGroupEnum stageGroup = wield::util::convertStringToEnum<StageGroupEnum>(stageGroupValue, StageGroupStrings);
                if( StageGroupEnum::NumberOfEntries == stageGroup)
                {
                    std::clog << wield::logging::ErrorMessage("failed to convert '" + stageGroupValue + "' to enum value.");
                }
                else
                {
                    stageGroups.push_back(stageGroup);
                }
            }
        }
    };
}}
