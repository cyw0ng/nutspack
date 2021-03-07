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

namespace np::common
{
    class Logging
    {
    private:
        static Logging *instance;
        Logging();
        ~Logging();

    public:
        static Logging *getInstance();
    };

    Logging *Logging::instance;
}

#endif