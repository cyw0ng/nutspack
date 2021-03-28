/**
 * common/database.cc - Common database interface
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

#include <limits.h>
#include <sqlite3.h>
#include <cstring>
#include <iostream>
#include "database.hh"
#include "common/errno/Errno.hh"
#include "common/logging/logging.hh"

using np::Errno;

namespace np::common
{
    /**
     * Init SqliteDB object based on dbFile and flags
     * 
     * @return {E_SUCCESS} if db is successfully created
     * @return {E_DBOBJ_DIFFPATH} if db object is init with another path
     * @return {E_DBOOJ_DIFFFLAG} if db object is init with different flags
     * @return {E_DBOOJ_SQLITERR} if sqlite3_open_v2 returns an error 
     */
    Errno SqliteDB::Init(std::string dbFile, int flags, int &sqliteErr)
    {
        logI("Init sqliteDB with [dbFile/flags/isInit] - [%s / %d / %d]", dbFile.c_str(), flags, isInit);
        if (isInit)
        {
            char savedPath[PATH_MAX];
            char tryPath[PATH_MAX];

            if (realpath(this->dbFile.c_str(), savedPath) && realpath(dbFile.c_str(), tryPath))
            {
                logI("Comparing sqliteDB file with [savedPath/tryPath/strncmp] - [%s / %s / %d]",
                     savedPath, tryPath, strncmp(savedPath, tryPath, PATH_MAX));
                if (strncmp(savedPath, tryPath, PATH_MAX))
                {
                    return Errno::E_DBOBJ_DIFFPATH;
                }

                if (flags != this->flags)
                {
                    return Errno::E_DBOOJ_DIFFFLAG;
                }

                return Errno::E_SUCCESS;
            }

            return Errno::E_DBOOJ_PATHERR;
        }

        sqliteErr = sqlite3_open_v2(dbFile.c_str(), &this->dbHandle, flags, NULL);
        if (sqliteErr == SQLITE_OK)
        {
            this->dbFile = dbFile;
            this->flags = flags;
            this->isInit = true;

            return Errno::E_SUCCESS;
        }
        else
        {
            return Errno::E_DBOOJ_SQLITERR;
        }
    }

    /**
     * Init SqliteDB object based on dbFile and flags
     * 
     * @return {E_SUCCESS} if db is successfully destroyed
     * @return {E_DBOOJ_SQLITERR} if sqlite3_close_v2 returns an error 
     */
    Errno SqliteDB::Destroy(int &sqliteErr)
    {
        if (isInit)
        {
            sqliteErr = sqlite3_close_v2(this->dbHandle);

            if (sqliteErr == 0)
            {
                this->isInit = false;
                this->flags = 0;
                this->dbHandle = nullptr;
                this->dbFile = "";

                return Errno::E_SUCCESS;
            }
            return Errno::E_DBOOJ_SQLITERR;
        }

        return Errno::E_DBOBJ_NOTINIT;
    }
}