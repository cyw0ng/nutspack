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
#include <sqlite3.h>
#include "common/database/database.hh"

using SqliteDB = np::common::SqliteDB;
using Errno = np::Errno;

class TestForDatabaseInterfaces : public ::testing::Test
{
public:
    std::string testRoot;

private:
    void SetUp() {
        srand(time(NULL));
        testRoot = "/tmp/testroot-" + std::to_string(rand());
        std::cout << "Using testroot: " + testRoot << std::endl;

        system(("mkdir " + testRoot).c_str());
    }

    void TearDown() {
        // system(("rm -rf " + testRoot).c_str());
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

    // Try to open a db object
    EXPECT_EQ(sqliteDB.Init(testRoot + "test.db", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, sqliteErr), Errno::E_SUCCESS);

    // Try to open the same db object
    EXPECT_EQ(sqliteDB.Init(testRoot + "/../../" + testRoot + "test.db", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, sqliteErr), Errno::E_SUCCESS);

    // Try to open the db onject with different path
    EXPECT_EQ(sqliteDB.Init(testRoot + "test.db.notexist", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, sqliteErr), Errno::E_DBOOJ_PATHERR);

    // Try to open the db object with different flags
    EXPECT_EQ(sqliteDB.Init(testRoot + "test.db", SQLITE_OPEN_READWRITE, sqliteErr), Errno::E_DBOOJ_DIFFFLAG);

    // Try to close the db object multiple times
    EXPECT_EQ(sqliteDB.Destroy(sqliteErr), Errno::E_SUCCESS);
    EXPECT_EQ(sqliteDB.Destroy(sqliteErr), Errno::E_DBOBJ_NOTINIT);
}
