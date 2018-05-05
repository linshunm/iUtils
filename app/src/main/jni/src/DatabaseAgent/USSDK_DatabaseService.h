//
//  USSDK_DatabaseService.h
//
//  Created by drz on 2018/4/23.
//  Copyright © 2018年 drz. All rights reserved.
//

#ifndef USSDK_DatabaseService_h
#define USSDK_DatabaseService_h

#include <stdio.h>
#include <string>
#include <map>

#include "USSDK_DatabaseModel.h"

#ifdef _WIN32
#include "../../stdafx.h"
#endif

using namespace std;
using namespace NS_USSDK_DatabaseService;

namespace NS_USSDK_DatabaseService
{
#define IS_SQLITE_SUCCESS(a) (SQLITE_OK == a?true:false)
#define IS_SQLITE_DONE(a) (SQLITE_ROW == a?false:true)
#define SQLITE_DEFAULT -1   /* SQL default status */

    class USSDK_DatabaseService
    {
    public:
        static USSDK_DatabaseService *shareInstance();

	public:
		//open
		static int open(string dbPath, sqlite_db* &db);
		//close
		static int close(sqlite_db* &db);
		//execute
		static int execute(sqlite_db* &db, string sql, ...);
		//execute trans resultset
		static int query(sqlite_db* &db, DatabaseResultSet *resultSet, string sql, ...);
		//execute isexist record
		static bool isExistRecord(sqlite_db* &db, string sql, ...);
		//key
		static int setKey(sqlite_db* &db, string key);
		//rekey 
		static int reKey(sqlite_db* &db, string newKey);
		//isTableExist
		static bool isTableExist(sqlite_db* &db, string tableName);
		//getLastErrorMsg
		static string getLastErrorMsg(sqlite_db* &db);
		//update
		static int updateVersion(sqlite_db* &db);

		//for query from external 
		static int step(sqlite_stmt *stmt);
		static int finalize(sqlite_stmt *stmt);
		static int getColumnCount(sqlite_stmt *stmt);
		static string getColumnName(sqlite_stmt *stmt, int index);
		static int getColumnInt(sqlite_stmt *stmt, int index);
		static string getColumnText(sqlite_stmt *stmt, int index);
    };
}

#endif /* USSDK_DatabaseService_h */
