//
//  USSDK_DatabaseService.cpp
//
//  Created by drz on 2018/4/23.
//  Copyright © 2018年 drz. All rights reserved.
//

#include <stdio.h>
#include <iostream>

#include "USSDK_DatabaseService.h"
#include "../LogAgent/AdapterAPI/USSDK_LogAdapter.hpp"

#ifdef _APPLE
#define sqlite_open     sqlite5_open
#define sqlite_close    sqlite5_close
#define sqlite_exec     sqlite5_exec
#define sqlite_prepare  sqlite5_prepare
#define sqlite_step		sqlite5_step
#define sqlite_finalize sqlite5_finalize
#define sqlite_free		sqlite5_free
#define sqlite_errmsg   sqlite5_errmsg
#define sqlite_key      sqlite5_key
#define sqlite_rekey    sqlite5_rekey
#define sqlite_bind_text     sqlite5_bind_text
#define sqlite_column_count  sqlite5_column_count
#define sqlite_column_name   sqlite5_column_name
#define sqlite_column_int    sqlite5_column_int
#define sqlite_column_text   sqlite5_column_text
#else 
#define sqlite_open     sqlite3_open
#define sqlite_close	sqlite3_close
#define sqlite_exec		sqlite3_exec
#define sqlite_prepare  sqlite3_prepare
#define sqlite_step		sqlite3_step
#define sqlite_finalize sqlite3_finalize
#define sqlite_free		sqlite3_free
#define sqlite_errmsg   sqlite3_errmsg
#define sqlite_key      sqlite3_key
#define sqlite_rekey    sqlite3_rekey
#define sqlite_bind_text     sqlite3_bind_text
#define sqlite_column_count  sqlite3_column_count
#define sqlite_column_name   sqlite3_column_name
#define sqlite_column_int    sqlite3_column_int
#define sqlite_column_text   sqlite3_column_text
#endif

using namespace NS_USSDK_DatabaseService;
using namespace NS_USSDK_LogService;

USSDK_DatabaseService *USSDK_DatabaseService::shareInstance()
{
    static USSDK_DatabaseService instance;
    return &instance;
}

int USSDK_DatabaseService::open(string dbPath, sqlite_db* &db)
{
    return sqlite_open(dbPath.c_str(), &db);
}

int USSDK_DatabaseService::close(sqlite_db* &db)
{
	if (db)
	{
		int result = SQLITE_DEFAULT;
		result = sqlite_close(db);

		LOG_DEBUGGING("databaseAgent", "sqlite_close[%d]", result);
		db = NULL;
		return result;
	}
	else
	{
		LOG_DEBUGGING("databaseAgent", "db is null!");
	}
	return 0;
}

int USSDK_DatabaseService::execute(sqlite_db* &db, string sql, ...)
{
	if (db)
	{
		//do split
		va_list args;
		int length = sql.length();
		char *sqlBuf = (char *)malloc(3*length + 1);
		memset(sqlBuf, 0, 3*length + 1);
		va_start(args, sql);
		vsnprintf(sqlBuf, 3*length + 1, sql.c_str(), args);
		va_end(args);

		int result = SQLITE_DEFAULT;
		result = sqlite_exec(db, sqlBuf, 0, 0, 0);
		LOG_DEBUGGING("databaseAgent", "sqlite_exec[%d][%s]", result, getLastErrorMsg(db).c_str());
		
		free(sqlBuf);
		sqlBuf = NULL;
		return result;
	}
	else
	{
		LOG_DEBUGGING("databaseAgent", "db is null!");
	}
	return SQLITE_DEFAULT;
}

int USSDK_DatabaseService::query(sqlite_db* &db, DatabaseResultSet *resultSet, string sql, ...)
{
	if (db)
	{
		//do split
		va_list args;
		int length = sql.length();
		char *sqlBuf = (char *)malloc(3*length + 1);
		memset(sqlBuf, 0, 3*length + 1);
		va_start(args, sql);
		vsnprintf(sqlBuf, 3*length + 1, sql.c_str(), args);
		va_end(args);

		resultSet->sql = sql;
		resultSet->db = db;
		int result = SQLITE_DEFAULT;
		result = sqlite_prepare(db, sqlBuf, -1, &resultSet->statement, 0); 

		if (result != SQLITE_OK)
		{
			resultSet->isSuccess = false;
			LOG_DEBUGGING("databaseAgent", "sqlite_prepare[%d][%s]", result, getLastErrorMsg(db).c_str());
		}

		free(sqlBuf);
		sqlBuf = NULL;
		resultSet->isSuccess = true;
		//after that,remember to finalize.
		return result;
	}
	else
	{
		LOG_DEBUGGING("databaseAgent", "db is null!");
	}
	return SQLITE_DEFAULT;
}

bool USSDK_DatabaseService::isExistRecord(sqlite_db* &db, string sql, ...)
{
	if (db)
	{
		va_list args;
		int length = sql.length();
		char *sqlBuf = (char *)malloc(3*length + 1);
		memset(sqlBuf, 0, 3*length + 1);
		va_start(args, sql);
		vsnprintf(sqlBuf, 3*length + 1, sql.c_str(), args);
		va_end(args);

		long lID = -1;
		int result = 0;
		sqlite_stmt *pStmt = NULL;

		result = sqlite_prepare(db, sqlBuf, -1, &pStmt, 0); 

		if (result != SQLITE_OK)
		{
			LOG_DEBUGGING("databaseAgent", "sqlite_prepare[%d][%s]", result, getLastErrorMsg(db).c_str());
		}

		result = sqlite_step(pStmt);

		if(SQLITE_ROW == result)
		{
			lID = sqlite_column_int(pStmt, 0);	
		}
		sqlite_finalize(pStmt);
		sqlite_free(NULL);

		free(sqlBuf);
		sqlBuf = NULL;

		return lID > 0? true:false;
	}
	else
	{
		LOG_DEBUGGING("databaseAgent", "db is null!");
	}
	return false;
}

int USSDK_DatabaseService::setKey(sqlite_db* &db, string key)
{
	if (db)
	{
		int result = SQLITE_DEFAULT;

		result = sqlite_key(db, key.c_str(), key.length());

		LOG_DEBUGGING("databaseAgent", "sqlite_key[%d][%s]", result, getLastErrorMsg(db).c_str());
		return result;
	}
	else
	{
		LOG_DEBUGGING("databaseAgent", "db is null!");
	}
	return SQLITE_DEFAULT;
}

int USSDK_DatabaseService::reKey(sqlite_db* &db, string newKey)
{
	if (db)
	{
		int result = sqlite_rekey(db, newKey.c_str(), newKey.length());

		LOG_DEBUGGING("databaseAgent", "sqlite_rekey[%d][%s]", result, getLastErrorMsg(db).c_str());
		return result;
	}
	else
	{
		LOG_DEBUGGING("databaseAgent", "db is null!");
	}
	return SQLITE_DEFAULT;
}

bool USSDK_DatabaseService::isTableExist(sqlite_db* &db, string tableName)
{
	bool bExist = false;
	int ret = 0;

	sqlite_stmt* stmt = NULL;
	const char * KSQLSELECT = "SELECT * FROM sqlite_master WHERE type='table' AND name=?;";

	if (NULL == db)
	{
		LOG_DEBUGGING("databaseAgent", "db is null!");
		return false;
	}

	if (0 == tableName.length())
	{
		LOG_DEBUGGING("databaseAgent", "tableName's length is 0!");
		return false;
	}

	do 
	{
		ret = sqlite_prepare(db, KSQLSELECT, -1, &stmt, 0);
		if(SQLITE_OK != ret) break;

		sqlite_bind_text(stmt, 1, tableName.c_str(), -1, SQLITE_STATIC);
		ret = sqlite_step(stmt);
		if (ret == SQLITE_DONE)
		{
			bExist = false;
		}
		else if (ret == SQLITE_ROW)
		{
			bExist = true;
		}
	} while (0);

	sqlite_finalize(stmt);
	return bExist;
}	

string USSDK_DatabaseService::getLastErrorMsg(sqlite_db* &db)
{
	if(db)
	{
		return sqlite_errmsg(db);
	}
	else
	{
		LOG_DEBUGGING("databaseAgent", "db is null!");
	}
	return "";
}

int USSDK_DatabaseService::step(sqlite_stmt *stmt)
{
	if(stmt)
	{
		return sqlite_step(stmt);
	}
	return SQLITE_DEFAULT;
}

int USSDK_DatabaseService::finalize(sqlite_stmt *stmt)
{
	if(stmt)
	{
		return sqlite_finalize(stmt);
	}
	return SQLITE_DEFAULT;
}

int USSDK_DatabaseService::getColumnCount(sqlite_stmt *stmt)
{
	if(stmt)
	{
		return sqlite_column_count(stmt);
	}
	return SQLITE_DEFAULT;
}

string USSDK_DatabaseService::getColumnName(sqlite_stmt *stmt, int index)
{
	if(stmt)
	{
		return sqlite_column_name(stmt, index);
	}
	return "";
}

int USSDK_DatabaseService::getColumnInt(sqlite_stmt *stmt, int index)
{
	if(stmt)
	{
		return sqlite_column_int(stmt, index);
	}
	return SQLITE_DEFAULT;
}

string USSDK_DatabaseService::getColumnText(sqlite_stmt *stmt, int index)
{
	if(stmt)
	{
		return (char *)sqlite_column_text(stmt, index);
	}
	return "";
}

int USSDK_DatabaseService::updateVersion(sqlite_db* &db)
{
	return 0;
}