/**
 * common/errno.hh - Common errno definition
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

#ifndef COMMON_ERRNO_
#define COMMON_ERRNO_

namespace np
{
    enum class ErrFamily : int {
        COMMON_DATABASE = 0x1 << 3,
    };

    enum class Errno : int {
        /**
         * Generic success
         */
        E_SUCCESS = 0,

        /**
         * Database object is initialized with different path 
         */
        E_DBOBJ_DIFFPATH = 0x1 | (int)ErrFamily::COMMON_DATABASE,

        /**
         * Database object is initialized with different flags
         */
        E_DBOOJ_DIFFFLAG = 0x2 | (int)ErrFamily::COMMON_DATABASE,

        /**
         * Error or unacceptable dbFile path
         */
        E_DBOOJ_PATHERR = 0x3 | (int)ErrFamily::COMMON_DATABASE,

        /**
         * Generic Sqlite reported internal error
         */
        E_DBOOJ_SQLITERR = 0x4 | (int)ErrFamily::COMMON_DATABASE,

        /**
         * Database object is not initialized
         */
        E_DBOBJ_NOTINIT = 0x5 | (int)ErrFamily::COMMON_DATABASE
    };
}

#endif