//
//  USSDK_DatabaseModel.h
//
//  Created by drz on 2018/4/23.
//  Copyright © 2018年 drz. All rights reserved.
//

#ifndef USSDK_DatabaseModel_h
#define USSDK_DatabaseModel_h

#include <stdio.h>
#include <string>
#include <map>

#ifdef _APPLE
#include "sqlite5.h"
#define sqlite_db sqlite5
#define sqlite_stmt sqlite5_stmt
#else 
#include "sqlite3.h"
#define sqlite_db sqlite3
#define sqlite_stmt sqlite3_stmt
#endif

using namespace std;

namespace NS_USSDK_DatabaseService
{
	typedef enum
	{
		DatabaseType_Chat,
		DatabaseType_Conversation,
		DatabaseType_RosterInfo,
		DatabaseType_Other,
	}DatabaseType;

	typedef enum
	{
		DatabaseOperationType_Insert,
		DatabaseOperationType_delete,
		DatabaseOperationType_cover,
	}DatabaseOperationType;

	typedef enum
	{
		DataGetStatus_Success,
		DataGetStatus_Failed,
	}DatabaseGetStatus;

	//result set
	class DatabaseResultSet
	{
	public:
		DatabaseResultSet();
		~DatabaseResultSet();
		
	private:
		map<string, int> columnIndexMap;

	public:
		bool isSuccess;
		int columnCount;
		string sql;
		sqlite_db *db;
		sqlite_stmt *statement;
		bool result();
		bool next();
		void close();
		int intForColumn(string columnName);
		string textForColumn(string columnName);

	private:
		void setUpColumnIndexMap();
	};
}
#endif /* USSDK_DatabaseModel_h */
