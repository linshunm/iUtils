//
//  USSDK_DatabaseModel.cpp
//
//  Created by drz on 2018/4/23.
//  Copyright © 2018年 drz. All rights reserved.
//

#include "USSDK_DatabaseModel.h"
#include "USSDK_DatabaseService.h"
#include "../Foundation/USSDKStringTool/USSDK_StringTool.hpp"
#include <string>

using namespace std;
using namespace NS_USSDK_DatabaseService;
using namespace NS_USSDK_Foundation;

DatabaseResultSet::DatabaseResultSet()
{
	isSuccess = false;
	columnCount = 0;
	sql = "";
	db = NULL;
	statement = NULL;
}

DatabaseResultSet::~DatabaseResultSet()
{

}

bool DatabaseResultSet::result()
{
	return isSuccess;
}

bool DatabaseResultSet::next()
{
	if (statement)
	{
		if (!IS_SQLITE_DONE( NS_USSDK_DatabaseService::USSDK_DatabaseService::step(statement)))
		{
			return true;
		}
	}

	return false;
}

void DatabaseResultSet::close()
{
	if (statement)
	{
		NS_USSDK_DatabaseService::USSDK_DatabaseService::finalize(statement);
	}
}

int DatabaseResultSet::intForColumn(string columnName)
{
	if (columnIndexMap.size() == 0)
	{
		setUpColumnIndexMap();
	}

	USSDK_StringTool::makerLower(columnName);
	int index = columnIndexMap[columnName];
	
	return NS_USSDK_DatabaseService::USSDK_DatabaseService::getColumnInt(statement, index);
}

string DatabaseResultSet::textForColumn(string columnName)
{
	if (columnIndexMap.size() == 0)
	{
		setUpColumnIndexMap();
	}

	USSDK_StringTool::makerLower(columnName);
	int index = columnIndexMap[columnName];

	return NS_USSDK_DatabaseService::USSDK_DatabaseService::getColumnText(statement, index);
}

void DatabaseResultSet::setUpColumnIndexMap()
{
	if(statement)
	{
		int columnCount = NS_USSDK_DatabaseService::USSDK_DatabaseService::getColumnCount(statement);

		for(int i=0;i<columnCount;i++)
		{
			string columnName = NS_USSDK_DatabaseService::USSDK_DatabaseService::getColumnName(statement, i);
			columnIndexMap.insert(map<string, int>::value_type(columnName, i));
		}
	}	
}