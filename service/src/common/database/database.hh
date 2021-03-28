/**
 * common/database.hh - Common database interface
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

#ifndef COMMON_DATABASE_
#define COMMON_DATABASE_

#include <sqlite3.h>
#include <string>
#include "common/errno/Errno.hh"

using np::Errno;

typedef int (*sqlite3Callback)(void*,int,char**,char**);

namespace np::common
{
    class SqliteDB
    {
    public:
        Errno Init(std::string dbFile, int flags, int &sqliteErr);
        Errno Destroy(int &sqliteErr);
        Errno Exec(std::string sql, sqlite3Callback callback, int &sqliteErr);
        Errno Vacuum(int &sqliteErr);

    private:
        bool isInit {false};
        std::string dbFile;
        sqlite3 *dbHandle {nullptr};
        int flags {0};
    };
}

#endif