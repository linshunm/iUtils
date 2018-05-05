//
//  USSDK_RosterDBManager.cpp
//
//  Created by drz on 2018/4/24.
//  Copyright © 2018年 YMXX. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include "USSDK_RosterDBManager.h"
#include "../../../Foundation/USSDKFile/USSDK_FileManager.hpp"
#include "../../../Foundation/USSDKStringTool/USSDK_StringTool.hpp"

using namespace NS_USSDK_DatabaseService;
using namespace NS_USSDK_RosterService;
using namespace NS_USSDK_Foundation;

USSDK_RosterDBManager::USSDK_RosterDBManager(string aRootDirPath, string anUserURI)
{
	rootDirPath = aRootDirPath;
	string userId = getUserID(anUserURI);
	if (userURI.compare(userId))
	{
		userURI = userId;
		checkCurrentDir();
	}
	
	isDatabaseInit = false;
}

USSDK_RosterDBManager::USSDK_RosterDBManager()
{
	isDatabaseInit = false;
}

USSDK_RosterDBManager::~USSDK_RosterDBManager()
{
	isDatabaseInit = false;
}

void USSDK_RosterDBManager::databaseInit()
{
	if (!isDatabaseInit)
	{
		bool isDBExist = USSDK_FileManager::isFileExist(getDatabaseDBPath());
		int result = NS_USSDK_DatabaseService::USSDK_DatabaseService::open(getDatabaseDBPath(), rosterDB);

		if(IS_SQLITE_SUCCESS(result))
		{
			if(!isDBExist)
			{
				encryDatabase(DBKey);
			}
			else
			{
				NS_USSDK_DatabaseService::USSDK_DatabaseService::setKey(rosterDB, DBKey);
			}

			if(false == NS_USSDK_DatabaseService::USSDK_DatabaseService::isTableExist(rosterDB, rosterTableName))
			{
				NS_USSDK_DatabaseService::USSDK_DatabaseService::execute(rosterDB, createRosterInfoTable);
				NS_USSDK_DatabaseService::USSDK_DatabaseService::execute(rosterDB, createUniqueIndex);
			}
			
			isDatabaseInit = true;
		}
		else
		{
			NS_USSDK_DatabaseService::USSDK_DatabaseService::close(rosterDB);
		}
	}
}

void USSDK_RosterDBManager::databaseUnInit()
{
	if (isDatabaseInit)
	{
		NS_USSDK_DatabaseService::USSDK_DatabaseService::close(rosterDB);
		isDatabaseInit = false;
	}
}

string USSDK_RosterDBManager::getDatabaseDBPath()
{
	return rosterDBPath;
}

bool USSDK_RosterDBManager::checkCurrentDir()
{
	string anUserDir = rootDirPath + userURI;
	USSDK_FileManager::fixDirPath(anUserDir);

	string aSubDirOfDB = anUserDir + pathSeparator + subDirOfDB;
	USSDK_FileManager::fixDirPath(aSubDirOfDB);
	
	rosterDBPath = aSubDirOfDB + pathSeparator + rosterDBName;
	return true;
}

string USSDK_RosterDBManager::getUserID(string aUserURI)
{
	return USSDK_StringTool::lastCompentUseSeparator(aUserURI, ":");
}

bool USSDK_RosterDBManager::encryDatabase(string key)
{
	if (!IS_SQLITE_SUCCESS(NS_USSDK_DatabaseService::USSDK_DatabaseService::setKey(rosterDB, key)))
	{
		return false;
	}

	string sql = setPragmaAutoVacuum;
	if (!IS_SQLITE_SUCCESS(NS_USSDK_DatabaseService::USSDK_DatabaseService::execute(rosterDB, sql)))
	{
		return false;
	}
    
    return true;
}

//////////////////////////////////////////////////////////////////////////
//roster operation start
bool USSDK_RosterDBManager::isExistRoster(string userURI)
{
	bool isExist = false;
	string sql = findRosterRecordIDSQL;
	isExist = NS_USSDK_DatabaseService::USSDK_DatabaseService::isExistRecord(rosterDB, sql, userURI.c_str());

	return isExist;
}

bool USSDK_RosterDBManager::insertRosterInfo(USSDK_SingleContact &info)
{
	string sql = insertRosterInfoSQL;
	int result = NS_USSDK_DatabaseService::USSDK_DatabaseService::execute(rosterDB, sql, info.URI.c_str(), info.name.c_str(), info.pinyinName.c_str(), info.logo.c_str(), info.sign.c_str(), info.userCapability, info.rosterRelationShip);

	return IS_SQLITE_SUCCESS(result);
}

bool USSDK_RosterDBManager::updateRosterInfo(USSDK_SingleContact &info)
{
	string sql = updateRosterInfoSQL;
	int result = NS_USSDK_DatabaseService::USSDK_DatabaseService::execute(rosterDB, sql, info.name.c_str(), info.pinyinName.c_str(), info.logo.c_str(), info.sign.c_str(), info.userCapability, info.rosterRelationShip, info.URI.c_str());

	return IS_SQLITE_SUCCESS(result);
}

bool USSDK_RosterDBManager::deleteRosterInfo(string userURI)
{
	string sql = deleteRosterInfoSQL;
	int result = NS_USSDK_DatabaseService::USSDK_DatabaseService::execute(rosterDB, sql, userURI.c_str());

	return IS_SQLITE_SUCCESS(result);
}

bool USSDK_RosterDBManager::getAllRosterInfo(USSDK_RosterList<USSDK_SingleContact> &aRostersInfo)
{
	string sql = getAllRosterInfoSQL;
	DatabaseResultSet rs;
	NS_USSDK_DatabaseService::USSDK_DatabaseService::query(rosterDB, &rs, sql);
	
	while(rs.next())
	{
		USSDK_SingleContact queryResult;
		queryResult.URI = rs.textForColumn("uri");
		queryResult.name = rs.textForColumn("name");
		queryResult.pinyinName = rs.textForColumn("pyname");
		queryResult.logo = rs.textForColumn("logo");
		queryResult.sign = rs.textForColumn("sign");
		queryResult.userCapability = rs.intForColumn("capability");
		queryResult.rosterRelationShip = (RosterRelationShip)rs.intForColumn("relationship");
		aRostersInfo.add(queryResult.URI, queryResult);
	}
	
	rs.close();
	return rs.result();
}

bool USSDK_RosterDBManager::getRosterByURI(string userURI, USSDK_SingleContact &Info)
{
	string sql = getRosterInfoByURISQL;
	DatabaseResultSet rs;
	NS_USSDK_DatabaseService::USSDK_DatabaseService::query(rosterDB, &rs, sql, userURI.c_str());

	while(rs.next())
	{
		Info.URI = rs.textForColumn("uri");
		Info.name = rs.textForColumn("name");
		Info.pinyinName = rs.textForColumn("pyname");
		Info.logo = rs.textForColumn("logo");
		Info.sign = rs.textForColumn("sign");
		Info.userCapability = rs.intForColumn("capability");
		Info.rosterRelationShip = (RosterRelationShip)rs.intForColumn("relationship");
	}

	rs.close();
	return rs.result();
}

//roster operation end
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//group operation start
bool USSDK_RosterDBManager::isExistGroup(string groupURI)
{
	bool isExist = false;
	string sql = findGroupRecordIDSQL;
	isExist = NS_USSDK_DatabaseService::USSDK_DatabaseService::isExistRecord(rosterDB, sql, groupURI.c_str());

	return isExist;
}

bool USSDK_RosterDBManager::deleteGroupInfo(string groupURI)
{
	string sql = deleteGroupInfoSQL;
	int result = NS_USSDK_DatabaseService::USSDK_DatabaseService::execute(rosterDB, sql, groupURI.c_str());

	return IS_SQLITE_SUCCESS(result);
}
//group operation end
//////////////////////////////////////////////////////////////////////////

bool USSDK_RosterDBManager::databaseFunctionTest(string path)
{
	sqlite3 *testDB;
	string dbPath = path + rosterDBName;
	string testKey = "testkey";
	bool isDBExist = USSDK_FileManager::isFileExist(dbPath);
	int result = NS_USSDK_DatabaseService::USSDK_DatabaseService::open(dbPath, testDB);

	if(IS_SQLITE_SUCCESS(result))
	{
		if(!isDBExist)
		{
			NS_USSDK_DatabaseService::USSDK_DatabaseService::setKey(testDB, testKey);
			NS_USSDK_DatabaseService::USSDK_DatabaseService::execute(testDB, setPragmaAutoVacuum);
		}
		else
		{
			NS_USSDK_DatabaseService::USSDK_DatabaseService::setKey(testDB, testKey);
		}

		if(false == NS_USSDK_DatabaseService::USSDK_DatabaseService::isTableExist(testDB, rosterTableName))
		{
			NS_USSDK_DatabaseService::USSDK_DatabaseService::execute(testDB, createRosterInfoTable);
			NS_USSDK_DatabaseService::USSDK_DatabaseService::execute(testDB, createUniqueIndex);
		}
	}
	else
	{
		NS_USSDK_DatabaseService::USSDK_DatabaseService::close(testDB);
		return false;
	}

	//test
	//insert
	NS_USSDK_DatabaseService::USSDK_DatabaseService::execute(testDB, insertRosterInfoSQL, "testURI", "testName", "testPyName", "testLogo", "testSign", 0, 1);
	NS_USSDK_DatabaseService::USSDK_DatabaseService::execute(testDB, insertRosterInfoSQL, "testForUpdate", "testName", "testPyName", "testLogo", "testSign", 0, 1);
	NS_USSDK_DatabaseService::USSDK_DatabaseService::execute(testDB, insertRosterInfoSQL, "testFordelete", "testName", "testPyName", "testLogo", "testSign", 0, 1);
	//update
	NS_USSDK_DatabaseService::USSDK_DatabaseService::execute(testDB, updateRosterInfoSQL, "updateName", "updatePyName", "updateLogo", "updateSign", 0, 1, "testForUpdate");
	//delete
	NS_USSDK_DatabaseService::USSDK_DatabaseService::execute(testDB, deleteRosterInfoSQL, "testFordelete");
	//query
	USSDK_SingleContact info;
	bool isSuccess = getRosterByURI("testURI", info);
	NS_USSDK_DatabaseService::USSDK_DatabaseService::close(testDB);
	
	return isSuccess;
}