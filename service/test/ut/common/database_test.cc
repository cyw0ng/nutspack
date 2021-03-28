/**
 * ut/common/database_test.cc - Common database interface unit tests
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

#include "gtest/gtest.h"
#include <ctime>
#include <string>
#include <iostream>
#include <queue>
#include <sqlite3.h>
#include "common/database/database.hh"

using SqliteDB = np::common::SqliteDB;
using SqlitePara = np::common::SqlitePara;
using SqliteBindType = np::common::SqliteBindType;
using Errno = np::Errno;

bool g_foundRecord = false;

class TestForDatabaseInterfaces : public ::testing::Test
{
public:
    std::string testRoot;

private:
    void SetUp()
    {
        srand(time(NULL));
        testRoot = "/tmp/testroot-" + std::to_string(rand()) + "/";
        std::cout << "Using testroot: " + testRoot << std::endl;

        system(("mkdir " + testRoot).c_str());
    }

    void TearDown()
    {
        system(("rm -rf " + testRoot).c_str());
    }
};

/**
 * TestForDatabaseInterfaces.CommonOpenClose
 * 
 * @test sqliteDB common open close operations
 */
TEST_F(TestForDatabaseInterfaces, CommonOpenClose)
{
    int sqliteErr = 0;
    SqliteDB sqliteDB;
    Errno npErr;

    // Try to close the db object before init
    EXPECT_EQ(sqliteDB.Destroy(sqliteErr), Errno::E_DBOBJ_NOTINIT);

    // Try to execute sql query before init
    EXPECT_EQ(sqliteDB.Vacuum(sqliteErr), Errno::E_DBOBJ_NOTINIT);

    // Try to open a db object
    EXPECT_EQ(sqliteDB.Init(testRoot + "test.db", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, sqliteErr), Errno::E_SUCCESS);
    EXPECT_EQ(sqliteDB.Vacuum(sqliteErr), Errno::E_SUCCESS);
    EXPECT_EQ(sqliteErr, SQLITE_OK);

    // Try to open the same db object
    EXPECT_EQ(sqliteDB.Init(testRoot + "/../../" + testRoot + "test.db", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, sqliteErr), Errno::E_SUCCESS);

    // Try to open the db onject with different path
    EXPECT_EQ(sqliteDB.Init(testRoot + "test.db.notexist", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, sqliteErr), Errno::E_DBOOJ_PATHERR);

    // Try to open the db object with different flags
    EXPECT_EQ(sqliteDB.Init(testRoot + "test.db", SQLITE_OPEN_READWRITE, sqliteErr), Errno::E_DBOOJ_DIFFFLAG);

    // Try to close the db object multiple times
    EXPECT_EQ(sqliteDB.Destroy(sqliteErr), Errno::E_SUCCESS);
    EXPECT_EQ(sqliteDB.Destroy(sqliteErr), Errno::E_DBOBJ_NOTINIT);

    // Try to execute sql query after init
    EXPECT_EQ(sqliteDB.Vacuum(sqliteErr), Errno::E_DBOBJ_NOTINIT);
}

/**
 * TestForDatabaseInterfaces.BasicQuery
 * 
 * @test sqliteDB basic query tests
 */
TEST_F(TestForDatabaseInterfaces, BasicQuery)
{
    int sqliteErr = 0;
    SqliteDB sqliteDB;
    Errno npErr;

    // Try to open a db object
    EXPECT_EQ(sqliteDB.Init(testRoot + "test.db", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, sqliteErr), Errno::E_SUCCESS);
    EXPECT_EQ(sqliteErr, SQLITE_OK);

    // Try yo create table
    const char *createTableStmt = "CREATE TABLE contacts ("
                                  "id INTEGER PRIMARY KEY,"
                                  "name TEXT NOT NULL,"
                                  "phone TEXT NOT NULL UNIQUE"
                                  ");";
    const char *dropTableStmt = "DROP TABLE contacts;";
    const char *insertStmt = "INSERT INTO contacts VALUES (0, 'John', '1732');";
    const char *selectStmt = "SELECT * FROM contacts;";
    const char *paraSelectStmt = "SELECT * FROM contacts WHERE id = ? AND name = ?";

    // Try to create a table with same name
    EXPECT_EQ(sqliteDB.Exec(createTableStmt, nullptr, sqliteErr), Errno::E_SUCCESS);
    EXPECT_EQ(sqliteDB.Exec(createTableStmt, nullptr, sqliteErr), Errno::E_DBOBJ_SQLITERR);
    EXPECT_EQ(sqliteErr, SQLITE_ERROR);

    // Try to drop and re-create a table
    EXPECT_EQ(sqliteDB.Exec(dropTableStmt, nullptr, sqliteErr), Errno::E_SUCCESS);
    EXPECT_EQ(sqliteDB.Exec(createTableStmt, nullptr, sqliteErr), Errno::E_SUCCESS);

    // Try to insert data
    EXPECT_EQ(sqliteDB.Exec(insertStmt, nullptr, sqliteErr), Errno::E_SUCCESS);
    EXPECT_EQ(sqliteDB.Exec(insertStmt, nullptr, sqliteErr), Errno::E_DBOBJ_SQLITERR);
    EXPECT_EQ(sqliteErr, SQLITE_CONSTRAINT);

    // Try to query for data inserted
    g_foundRecord = false;
    EXPECT_EQ(sqliteDB.Exec(
                  selectStmt, [](void *ctx, int argc, char **argv, char **columnName) -> int {
                      g_foundRecord = true;

                      EXPECT_EQ(argc, 3);
                      EXPECT_STREQ(columnName[0], "id");
                      EXPECT_STREQ(argv[0], "0");
                      EXPECT_STREQ(columnName[1], "name");
                      EXPECT_STREQ(argv[1], "John");
                      EXPECT_STREQ(columnName[2], "phone");
                      EXPECT_STREQ(argv[2], "1732");
                      return 0;
                  },
                  sqliteErr),
              Errno::E_SUCCESS);
    EXPECT_EQ(g_foundRecord, true);

    // Try to query with paras
    std::queue<SqlitePara *> paras;

    int targetQueryID = 0;
    SqlitePara *pPara1 = new SqlitePara(SqliteBindType::SQLITE3_BIND_INT, &targetQueryID, nullptr, nullptr);
    paras.push(pPara1);

    const char *targetQueryName = "John";
    int targetQueryNameMaxLength = -1;
    SqlitePara *pPara2 = new SqlitePara(SqliteBindType::SQLITE3_BIND_TEXT, &targetQueryName, &targetQueryNameMaxLength, nullptr);
    paras.push(pPara2);

    g_foundRecord = false;
    EXPECT_EQ(sqliteDB.Exec(
                  paraSelectStmt, paras,[](void *ctx, int argc, char **argv, char **columnName) -> int {
                      g_foundRecord = true;

                      EXPECT_EQ(argc, 3);
                      EXPECT_STREQ(columnName[0], "id");
                      EXPECT_STREQ(argv[0], "0");
                      EXPECT_STREQ(columnName[1], "name");
                      EXPECT_STREQ(argv[1], "John");
                      EXPECT_STREQ(columnName[2], "phone");
                      EXPECT_STREQ(argv[2], "1732");
                      return 0;
                  },
                  sqliteErr),
              Errno::E_SUCCESS);
    delete pPara1;
    delete pPara2;

    // Try to close the db object
    EXPECT_EQ(sqliteDB.Vacuum(sqliteErr), Errno::E_SUCCESS);
    EXPECT_EQ(sqliteDB.Destroy(sqliteErr), Errno::E_SUCCESS);
    EXPECT_EQ(sqliteDB.Exec(dropTableStmt, nullptr, sqliteErr), Errno::E_DBOBJ_NOTINIT);
}
