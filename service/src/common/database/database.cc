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
     * @return {E_DBOBJ_SQLITERR} if sqlite3_open_v2 returns an error
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
            return Errno::E_DBOBJ_SQLITERR;
        }
    }

    /**
     * Init SqliteDB object based on dbFile and flags
     * 
     * @return {E_SUCCESS} if db is successfully destroyed
     * @return {E_DBOBJ_SQLITERR} if sqlite3_close_v2 returns an error 
     * @return {E_DBOBJ_NOTINIT} if target db obj is not init
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
            return Errno::E_DBOBJ_SQLITERR;
        }

        return Errno::E_DBOBJ_NOTINIT;
    }

    /**
     * Execute simple SQL statements with callback
     * 
     * @return {E_SUCCESS} if statement is successfully executed
     * @return {E_DBOBJ_SQLITERR} if sqlite3_exec returns an error
     * @return {E_DBOBJ_NOTINIT} if target db obj is not init
     */
    Errno SqliteDB::Exec(std::string sql, sqlite3Callback callback, int &sqliteErr)
    {
        if (isInit)
        {
            char *zErrMsg = nullptr;

            sqliteErr = sqlite3_exec(this->dbHandle, sql.c_str(), callback, 0, &zErrMsg);

            if (sqliteErr == SQLITE_OK)
            {
                return Errno::E_SUCCESS;
            }
            else
            {
                logE("sqlite execute error with: %s", zErrMsg);
                sqlite3_free(zErrMsg);

                return Errno::E_DBOBJ_SQLITERR;
            }
        }

        return Errno::E_DBOBJ_NOTINIT;
    }

    /**
     * Execute parameterized SQL statements with callback
     * 
     * @return {E_SUCCESS} if statement is successfully executed
     * @return {E_DBOBJ_SQLITERR} if sqlite3_prepare_v2 returns an error
     * @return {E_DBOBJ_NOTINIT} if target db obj is not init
     */
    Errno SqliteDB::Exec(std::string sql, std::queue<SqlitePara *> paras, sqlite3Callback sqlite3Callback, int &sqliteErr)
    {
        if (isInit)
        {
            sqlite3_stmt *pStmt;

            sqliteErr = sqlite3_prepare_v2(this->dbHandle, sql.c_str(), -1, &pStmt, nullptr);

            if (sqliteErr == SQLITE_OK)
            {
                int currentPos = 1;
                while (paras.size() != 0)
                {
                    SqlitePara *para = paras.front();

                    (void)para->BindToStmt(&pStmt, currentPos, sqliteErr);
                    paras.pop();
                    if (sqliteErr != SQLITE_OK)
                    {
                        return Errno::E_DBOBJ_SQLITERR;
                    }

                    currentPos++;
                }
                const char *generatedSql = sqlite3_expanded_sql(pStmt);
                std::string generatedSqlString{generatedSql};
                return Exec(generatedSqlString, sqlite3Callback, sqliteErr);
            }
            return Errno::E_DBOBJ_SQLITERR;
        }

        return Errno::E_DBOBJ_NOTINIT;
    }

    /**
     * Execute VACUUM statement on current db handle
     * 
     * @return {E_SUCCESS} if statement is successfully executed
     * @return {E_DBOBJ_SQLITERR} if sqlite3_exec returns an error
     * @return {E_DBOBJ_NOTINIT} if target db obj is not init
     */
    Errno SqliteDB::Vacuum(int &sqliteErr)
    {
        return this->Exec("VACUUM;", nullptr, sqliteErr);
    }

    /**
     * Bind parameter to sqlite3 stmt
     * 
     * @return {E_SUCCESS} if statement is successfully executed
     * @return {E_DBOBJ_SQLITERR} if sqlite3_exec returns an error
     * @return {E_DBBIND_TYPENOTSUPPORT} if such sqlite bind type is not support
     * 
     * TBD: too ugly, refactor later
     */
    Errno SqlitePara::BindToStmt(sqlite3_stmt **ppStmt, int postion, int &sqliteErr)
    {
        switch (this->bindType)
        {
        case SqliteBindType::SQLITE3_BIND_INT:
            sqliteErr = sqlite3_bind_int(*ppStmt, postion, *(int *)pBindParam1);
            break;
        case SqliteBindType::SQLITE3_BIND_NULL:
            sqliteErr = sqlite3_bind_null(*ppStmt, postion);
            break;
        case SqliteBindType::SQLITE3_BIND_TEXT:
            if (pBindParam3 == nullptr)
            {
                sqliteErr = sqlite3_bind_text(*ppStmt, postion, *(const char **)pBindParam1, *(int *)pBindParam2, nullptr);
            }
            else
            {
                sqliteErr = sqlite3_bind_text(*ppStmt, postion, *(const char **)pBindParam1, *(int *)pBindParam2, *(sqliteBindTextCallback *)pBindParam3);
            }
            break;
        default:
            return Errno::E_DBBIND_TYPENOTSUPPORT;
        }

        if (sqliteErr == SQLITE_OK)
        {
            return Errno::E_SUCCESS;
        }

        return Errno::E_DBOBJ_SQLITERR;
    }
}