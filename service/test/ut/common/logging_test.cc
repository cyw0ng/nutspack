/**
 * ut/common/logging_test.cc - Common log interface unit tests
 * Copyright (c) 2021 cyw0ng<dev.cyw0ng@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **/
#include "gtest/gtest.h"
#include "common/logging/logging.hh"
#include <thread>
#include <future>
#include <random>
#include <sstream>
#include <string>
#include <ctime>
#include <fstream>
#include <regex>
#include <vector>
#include <thread>

using Logging = np::common::Logging;
using LogLevel = np::common::LogLevel;

void cleanLogFile()
{
    system("> /var/log/np.log");
}

void flushRsyslogd()
{
    system("pkill -HUP rsyslogd");
}

class TestForLoggingInterfaces : public ::testing::Test
{
    void SetUp() {
        srand(time(0));
        cleanLogFile();
    }

    void TearDown() {
        cleanLogFile();
    }
};

TEST_F(TestForLoggingInterfaces, Singleton)
{
    EXPECT_EQ(Logging::getInstance(), Logging::getInstance());

    auto future = std::async([]() -> Logging * {
        return Logging::getInstance();
    });
    EXPECT_EQ(Logging::getInstance(), future.get());
}

TEST_F(TestForLoggingInterfaces, Feature)
{
    cleanLogFile();
    std::ostringstream tagOStr;
    tagOStr << rand() << "-" << rand() << "-" << rand();
    std::string tagStr = tagOStr.str();

    std::regex tagStrRegex(".*" + tagStr);

    auto logging = Logging::getInstance();

    logging->Log(LogLevel::EMERG, "log EMERGENCY %s", tagStr.c_str());
    logging->Log(LogLevel::ALERT, "log ALERT %s", tagStr.c_str());
    logging->Log(LogLevel::CRIT, "log CRITICAL %s", tagStr.c_str());
    logging->Log(LogLevel::ERR, "log ERROR %s", tagStr.c_str());
    logging->Log(LogLevel::WARNING, "log WARNING %s", tagStr.c_str());
    logging->Log(LogLevel::NOTICE, "log NOTICE %s", tagStr.c_str());
    logging->Log(LogLevel::INFO, "log INFO %s", tagStr.c_str());
    logging->Log(LogLevel::DEBUG, "log DEBUG %s", tagStr.c_str());

    flushRsyslogd();
    std::ifstream logFile("/var/log/np.log");

    int lineCount = 0;
    std::string line;
    while (std::getline(logFile, line))
    {
        ASSERT_TRUE(std::regex_match(line, tagStrRegex));
        lineCount++;
    }

    ASSERT_EQ(lineCount, 8);
}

TEST_F(TestForLoggingInterfaces, MTStressing)
{
    cleanLogFile();

    std::vector<std::thread> threads;
    int scale = 12;

    std::regex logInfoRegex(".* log INFO.*");

    for (int i = 0; i < scale; i++)
    {
        threads.push_back(std::thread([scale]() {
            int *buf = new int[scale];
            for (int j = 0; j < scale; j++)
            {
                for (int k = 0; k < scale; k++)
                {
                    buf[k] = rand();
                }
                Logging::getInstance()->Log(LogLevel::INFO, "log INFO %s", (const char *)buf);
            }
            delete[] buf;
        }));
    }

    for (int i = 0; i < scale; i++)
    {
        threads[i].join();
    }

    flushRsyslogd();

    std::ifstream logFile("/var/log/np.log");

    int lineCount = 0;
    std::string line;
    while (std::getline(logFile, line))
    {
        ASSERT_TRUE(std::regex_match(line, logInfoRegex));
        lineCount++;
    }

    ASSERT_EQ(lineCount, scale * scale);
}