/**
 * fileSrv/FileSrv.cc - FileSrv Implementation
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
#include "common/srv/Srv.hh"
#include <string>

namespace np
{
    class FileSrv : np::Srv
    {
    public:
        bool init();
        std::string getSrvName() { return this->srvName; }
        int getSrvVersion() { return this->srvVersion; }

    private:
        bool isInit{false};
        std::string srvName{"FIleSrv"};
        int srvVersion{1};
        bool initDB();
    };
}
