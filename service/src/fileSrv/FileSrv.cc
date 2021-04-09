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
#include "FileSrv.hh"
#include "common/errno/Errno.hh"

using np::Errno;

namespace np
{
    /**
     * Init fileSrv
     * 
     * @return {E_SUCCESS} if fileSrv is successfully init
     * @return {E_SRV_ALREADYINIT} if fileSrv is already init
     */
    Errno FileSrv::Init() {
        if (this->isInit) {
            return Errno::E_SRV_ALREADYINIT;
        }
    }

    /**
     * Init database for fileSrv
     * 
     * @return {E_SUCCESS} if fileSrv database schema created
     */
    Errno FileSrv::initDB() {
        
    }
}