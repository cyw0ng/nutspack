/**
 * common/logging.hh - Common log interface
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

#ifndef COMMON_LOGGING_
#define COMMON_LOGGING_

#include <string>

#define logE(fmt, ...) do { \
    np::common::Logging::getInstance()->Log(np::common::LogLevel::ERR, fmt, __VA_ARGS__); \
} while(0); \

#define logD(fmt, ...) do { \
    np::common::Logging::getInstance()->Log(np::common::LogLevel::DEBUG, fmt, __VA_ARGS__); \
} while(0); \

#define logI(fmt, ...) do { \
    np::common::Logging::getInstance()->Log(np::common::LogLevel::INFO, fmt, __VA_ARGS__); \
} while(0); \

namespace np::common
{
    enum class LogLevel : int
    {
        EMERG,
        ALERT,
        CRIT,
        ERR,
        WARNING,
        NOTICE,
        INFO,
        DEBUG
    };

    class Logging
    {
    private:
        static Logging *instance;
        Logging();
        ~Logging();

    public:
        static Logging *getInstance();
        void Log(LogLevel, const char *, ...);
    };

    Logging *Logging::instance;
}

#endif