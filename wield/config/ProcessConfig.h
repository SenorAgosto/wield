#pragma once
#include <wield/config/ProcessPriorityEnum.h>
#include <wield/util/EnumUtils.h>

#include <yaml-cpp/yaml.h>
#include <string>

namespace wield { namespace config {

    struct ProcessConfig
    {
        std::string name;
        std::string configVersion;
        ProcessPriorityEnum priority;

        friend void operator>>(const YAML::Node& node, ProcessConfig& processConfig)
        {
            node["process"] >> processConfig.name;
            node["configVersion"] >> processConfig.configVersion;

            std::string priorityValue;
            node["priority"] >> priorityValue;
            processConfig.priority = util::convertStringToEnum<ProcessPriorityEnum>(priorityValue, ProcessPriorityStrings);
        }
    };
}}
