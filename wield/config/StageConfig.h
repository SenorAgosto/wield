#pragma once
#include <wield/util/EnumUtils.h>

#include <yaml-cpp/yaml.h>
#include <iostream>
#include <string>

namespace wield { namespace config {

    template<typename StageEnum, typename StageGroupEnum>
    struct StageConfig
    {
        StageEnum stageName;
        StageGroupEnum stageGroup;
        size_t stagePriority;
        size_t batchCount;
        size_t emptyRetryCount;

        friend void operator>>(const YAML::Node& node, StageConfig& stageConfig)
        {
            std::string stageName;
            std::string groupName;
            std::string priority;

            node["stage"] >> stageName;
            stageConfig.stageName = util::convertStringToEnum<StageEnum, const char**>(stageName, StageEnumStrings);
            if(StageEnum::NumberOfEntries == stageConfig.stageName )
            {
                std::clog << wield::logging::ErrorMessage("failed to convert '" + stageName + "' to enum value.");
            }

            node["stageGroup"] >> groupName; 
            stageConfig.stageGroup = util::convertStringToEnum<StageGroupEnum, const char**>(groupName, StageGroupStrings);
            if(StageGroupEnum::NumberOfEntries == stageConfig.stageGroup)
            {
                std::clog << wield::logging::ErrorMessage("failed to convert '" + groupName + "' to enum value.");
            }

            node["stagePriority"] >> stageConfig.stagePriority;            
            node["batchCount"] >> stageConfig.batchCount;
            node["emptyRetryCount"] >> stageConfig.emptyRetryCount;
        }
    };
}}
