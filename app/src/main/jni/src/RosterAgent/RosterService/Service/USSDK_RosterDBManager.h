//
//  USSDK_RosterDBManager.h
//
//  Created by drz on 2018/4/24.
//  Copyright © 2018年 drz. All rights reserved.
//

#ifndef USSDK_RosterDBManager_h
#define USSDK_RosterDBManager_h

#include <stdio.h>
#include "../../../DatabaseAgent/DatabaseAgentHeader.h"
#include "../Model/USSDK_RosterModel.h"
#include "../Model/USSDK_SingleContactModel.h"
#include "../Model/USSDK_RosterListModel.h"
#include "../../../DatabaseAgent/USSDK_DatabaseModel.h"

namespace NS_USSDK_RosterService
{
	static std::string subDirOfDB = "RecordDB";
	static std::string rosterDBName = "Roster_encry.db";
	static std::string setPragmaAutoVacuum = "PRAGMA auto_vacuum = 1;";
	static std::string attachDatabase = "attach database '%s' as sourceDb key '';";
	//roster
	static std::string rosterTableName = "rosterinfo";
	static std::string createRosterInfoTable = "CREATE TABLE rosterinfo (id INTEGER PRIMARY KEY AUTOINCREMENT,uri VARCHAR,name VARCHAR,pyname VARCHAR,logo VARCHAR,sign VARCHAR,capability INTEGER,relationship INTEGER);";
	static std::string createUniqueIndex = "CREATE UNIQUE INDEX if not exists unique_index_rosterinfo_uri ON rosterinfo(uri);";
	static std::string insertRosterInfoSQL = "INSERT INTO rosterinfo(uri,name,pyname,logo,sign,capability,relationship) VALUES(\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',%d,%d);";
	static std::string replaceRosterInfoSQL = "REPLACE INTO rosterinfo(uri,name,pyname,logo,sign,capability,relationship) VALUES(\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',%d,%d);";
	static std::string updateRosterInfoSQL = "UPDATE rosterinfo SET name=\'%s\' pyname=\'%s\' logo=\'%s\' sign=\'%s\' capability=%d relationship=%d where uri=\'%s\';";
	static std::string deleteRosterInfoSQL = "DELETE from where uri=\'%s\';";
	static std::string findRosterRecordIDSQL = "SELECT id from rosterinfo where uri=\'%s\';";
	static std::string getAllRosterInfoSQL = "SELECT * from rosterinfo;";
	static std::string getRosterInfoByURISQL = "SELECT * from rosterinfo where uri=\'%s\';";
	//group
	static std::string groupTableName = "groupinfo";
	static std::string createGroupInfoTable = "CREATE TABLE groupinfo (id INTEGER PRIMARY KEY AUTOINCREMENT,uri VARCHAR,identifier VARCHAR,name VARCHAR,owneruri VARCHAR,ownername VARCHAR,members VARCHAR,topic VARCHAR,subject VARCHAR,pyname VARCHAR,logo VARCHAR,relationship INTEGER,attribute INTEGER,maxmember INTEGER);";
	static std::string createGroupInfoUniqueIndex = "CREATE UNIQUE INDEX if not exists unique_index_groupinfo_uri ON groupinfo(uri);";
	static std::string insertGroupInfoSQL = "INSERT INTO groupinfo(uri,identifier,name,owneruri,ownername,members,topic,subject,pyname,logo,relationship,attribute,maxmember) VALUES(\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',%d,%d,%d);";
	static std::string replaceGroupInfoSQL = "REPLACE INTO groupinfo(uri,identifier,name,owneruri,ownername,members,topic,subject,pyname,logo,relationship,attribute,maxmember) VALUES(\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',%d,%d,%d);";
	static std::string updateGroupInfoSQL = "UPDATE groupinfo SET identifier=\'%s\' name=\'%s\' owneruri=\'%s\' ownername=\'%s\' members=\'%s\' topic=\'%s\' subject=\'%s\' pyname=\'%s\' logo=\'%s\' relationship=%d attribute=%d maxmember=%d where uri=\'%s\';";
	static std::string deleteGroupInfoSQL = "DELETE from where uri=\'%s\';";
	static std::string findGroupRecordIDSQL = "SELECT id from groupinfo where uri=\'%s\';";
	static std::string getAllGroupInfoSQL = "SELECT * from groupinfo;";
	static std::string getGroupInfoByURISQL = "SELECT * from groupinfo where uri=\'%s\';";

	class USSDK_RosterDBManager
	{
	private:
		string rootDirPath;
		string userURI;
		string rosterDBPath;
		string DBKey;
		bool isDatabaseInit;
		sqlite3 *rosterDB;

	public:
		USSDK_RosterDBManager(string aRootDirPath, string anUserURI);
		USSDK_RosterDBManager();
		~USSDK_RosterDBManager();

	public:
		void databaseInit();
		void databaseUnInit();
		
		bool checkCurrentDir();
		string getDatabaseDBPath();
		string getUserID(string aUserURI);

		bool encryDatabase(string key);
		//roster
		bool isExistRoster(string userURI);
		bool insertRosterInfo(USSDK_SingleContact &info);
		bool updateRosterInfo(USSDK_SingleContact &info);
		bool deleteRosterInfo(string userURI);
		bool getAllRosterInfo(USSDK_RosterList<USSDK_SingleContact> &aRostersInfo);
		bool getRosterByURI(string userURI, USSDK_SingleContact &Info);
		//group
		bool isExistGroup(string groupURI);
		bool deleteGroupInfo(string groupURI);

		//test for android
		bool databaseFunctionTest(string path);
	};
}

#endif /* USSDK_RosterDBManager_h */
