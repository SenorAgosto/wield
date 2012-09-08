#include <UnitTest++/UnitTest++.h>

#include <wield/logging/Logging.h>
#include <wield/util/EnumUtils.h>
#include <wield/config/ProcessPriorityEnum.h>
#include <wield/config/ProcessConfig.h>
#include <wield/config/ThreadPriorityEnum.h>
#include <wield/config/ThreadConfig.h>
#include <wield/config/StageConfig.h>

#include <yaml-cpp/yaml.h>
#include <fstream>

#include "UnitTestAssert.h"
#include "StageGroups.h"
#include "Stages.h"

namespace {

    TEST(playingAroundWithYamlParser)
    {
        std::ifstream configFile("configs/test_config.yml");
        if(!configFile)
        {
            throw UnitTest::AssertException("Failed to open 'test_config.yml'", __FILE__, __LINE__);
        }

        YAML::Parser parser(configFile);
        YAML::Node document;

        // read the process configuration
        parser.GetNextDocument(document);
        ASSERT_EQUAL(1, document.size());
        wield::config::ProcessConfig processConfig;
        document[0] >> processConfig;

        CHECK_EQUAL("TestProcess", processConfig.name);
        CHECK_EQUAL("0.1", processConfig.configVersion);
        CHECK(wield::config::ProcessPriorityEnum::HighPriority == processConfig.priority);

        // read the thread configurations
        parser.GetNextDocument(document);
        ASSERT_EQUAL(2, document.size());
        wield::config::ThreadConfig<StageGroups> threadConfig;
        document[0] >> threadConfig;

        CHECK_EQUAL(0, threadConfig.threadNum);
        CHECK(wield::config::ThreadPriorityEnum::Highest == threadConfig.priority);
        ASSERT_EQUAL(2, threadConfig.stageGroups.size());
        CHECK(StageGroups::Group1 == threadConfig.stageGroups[0]);
        CHECK(StageGroups::Group2 == threadConfig.stageGroups[1]);

        threadConfig.clear();
        document[1] >> threadConfig;
        CHECK_EQUAL(1, threadConfig.threadNum);
        CHECK(wield::config::ThreadPriorityEnum::Normal == threadConfig.priority);
        ASSERT_EQUAL(1, threadConfig.stageGroups.size());
        CHECK(StageGroups::Group3 == threadConfig.stageGroups[0]);

        // read the stage configurations
        parser.GetNextDocument(document);
        ASSERT_EQUAL(2, document.size());
        wield::config::StageConfig<Stages, StageGroups> stageConfig;

        document[0] >> stageConfig;
        CHECK(Stages::Stage1 == stageConfig.stageName);
        CHECK(StageGroups::Group1 == stageConfig.stageGroup);
        CHECK_EQUAL(0, stageConfig.stagePriority);
        CHECK_EQUAL(100, stageConfig.batchCount);
        CHECK_EQUAL(10, stageConfig.emptyRetryCount);

        document[1] >> stageConfig;
        CHECK(Stages::Stage2 == stageConfig.stageName);
        CHECK(StageGroups::Group2 == stageConfig.stageGroup);
        CHECK_EQUAL(1, stageConfig.stagePriority);
        CHECK_EQUAL(150, stageConfig.batchCount);
        CHECK_EQUAL(15, stageConfig.emptyRetryCount);
    }
}
