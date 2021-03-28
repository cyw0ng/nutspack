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
#include <queue>
#include "common/errno/Errno.hh"

using np::Errno;

typedef int (*sqlite3Callback)(void *, int, char **, char **);
typedef void (*sqliteBindTextCallback)(void *);
namespace np::common
{
    // Bind different types as sqlite params
    // https://www.sqlite.org/c3ref/bind_blob.html
    // TBD: add more types if necessary
    enum class SqliteBindType : int
    {
        SQLITE3_BIND_NULL,
        SQLITE3_BIND_INT,
        SQLITE3_BIND_TEXT
    };

    // Relative position of sqlite_bind_* params
    class SqlitePara
    {
    public:
        SqlitePara(SqliteBindType type, void *p1, void *p2, void *p3) : bindType{type}, pBindParam1{p1}, pBindParam2{p2}, pBindParam3{p3} {};
        SqliteBindType bindType;

        void *pBindParam1;
        void *pBindParam2;
        void *pBindParam3;

        Errno BindToStmt(sqlite3_stmt **ppStmt, int postion, int &sqliteErr);
    };

    class SqliteDB
    {
    public:
        Errno Init(std::string dbFile, int flags, int &sqliteErr);
        Errno Destroy(int &sqliteErr);
        Errno Exec(std::string sql, sqlite3Callback callback, int &sqliteErr);
        Errno Exec(std::string sql, std::queue<SqlitePara *> paras, sqlite3Callback sqlite3Callback, int &sqliteErr);
        Errno Vacuum(int &sqliteErr);

    private:
        bool isInit{false};
        std::string dbFile;
        sqlite3 *dbHandle{nullptr};
        int flags{0};
    };
}

#endif