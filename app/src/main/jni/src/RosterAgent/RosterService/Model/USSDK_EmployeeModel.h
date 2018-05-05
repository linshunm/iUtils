//
//  USSDK_EmployeeModel.h
//  USSDK-Xcode
//
//  Created by YMXX on 2018/3/17.
//  Copyright © 2018年 YMXX. All rights reserved.
//

#ifndef USSDK_EmployeeModel_h
#define USSDK_EmployeeModel_h

#include <stdio.h>
#include <string>
#include "USSDK_RosterModel.h"

using namespace std;
using namespace NS_USSDK_RosterService;

namespace NS_USSDK_RosterService
{
	//need according new server api
	class USSDK_Employee : public USSDK_Roster
	{
		string nickName;
		string address;
		string email;
		string head;

		string mobcountryno1;
		string mobile1;
		string mobcountryno2;
		string mobile2;

		string outlinecountryno1;
		string quhao1;
		string telphone1;

		string outlinecountryno2;
		string quhao2;
		string telphone2;

		string outlinecountryno3;
		string quhao3;
		string telphone3;

		string outlinecountryno4;
		string quhao4;
		string telphone4;

		string outlinecountryno5;
		string quhao5;
		string telphone5;

		string department;

		string compantURI;
		string companyName;
		string companyNamePinyin;

		bool isMOA;
	};
}
#endif /* USSDK_EmployeeModel_h */
