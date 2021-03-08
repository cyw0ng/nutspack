/**
 * common/logging.cc - Common log interface
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

#include "logging.hh"
#include <syslog.h>
#include <iostream>
#include <cstdarg>

namespace np::common
{
    Logging::Logging()
    {
        openlog(nullptr, LOG_NDELAY | LOG_USER, LOG_LOCAL1);

        syslog(LOG_INFO, "Logging interface started");
    }

    Logging::~Logging()
    {
        closelog();
    }

    Logging *Logging::getInstance()
    {
        if (instance == nullptr)
        {
            instance = new Logging();
        }

        return instance;
    }

    void Logging::Log(LogLevel level, const char *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        vsyslog(LOG_LOCAL1 | (int)level, fmt, args);
        va_end(args);
    }
}