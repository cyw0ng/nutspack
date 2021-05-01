/**
 * common/foundation.cc - Sevice Framework Foundation
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

#include "Foundation.hh"
#include "common/errno/Errno.hh"

#include <memory>

namespace np::common
{
    /**
     * Get MsgBroker Singleton
     * 
     * @return {MsgBroker *} MsgBroker instance
     */
    std::shared_ptr<MsgBroker *> MsgBroker::getInstance()
    {
        std::lock_guard<std::mutex> lock(mtx);
        if (msgBroker == nullptr)
        {
            msgBroker = new MsgBroker();
        }
        return std::make_shared<MsgBroker *>(msgBroker);
    }

    /**
     * Init MsgBroker framework
     * 
     * @return {E_SRV_ALREADYINIT} MsgBroker has already init
     */
    Errno MsgBroker::Init()
    {
        std::lock_guard<std::mutex> lock(mtx);
        if (isInit)
        {
            return Errno::E_SRV_ALREADYINIT;
        }
    }
}