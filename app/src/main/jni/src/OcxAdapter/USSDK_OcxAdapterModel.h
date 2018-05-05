//
//  USSDK_OcxAdapterModel.hpp
//  USSDK-Xcode
//
//  Created by YMXX on 2018/3/16.
//  Copyright © 2018年 YMXX. All rights reserved.
//

#ifndef USSDK_OcxAdapterModel_h
#define USSDK_OcxAdapterModel_h

#include <stdio.h>
#include <string>

using namespace std;

namespace NS_USSDK_OcxAdapter
{
		typedef struct
		{
			string chSoftPhone;
			string chPassword;
			string chSSIP;
			int lSSPort;
			string chSipServerIP;
			int lSipServerPort;
			string chVersion;
			short nLogonType;												
		}OcxLogonPara;

		typedef enum
		{
			TimerNo_1,
			TimerNo_2,
			TimerNo_3,
			TimerNo_4,
			TimerNo_5,
			TimerNo_6,
			TimerNo_7,
			TimerNo_8,
			TimerNo_9,
		}TimerNo;

		#define R01                        1000
		typedef enum
		{
			Timer1Event_1 = R01 ,
			Timer1Event_2,
			Timer1Event_3,
			Timer1Event_4,
			Timer1Event_5,
			Timer1Event_6,
			Timer1Event_7,
			Timer1Event_8,
			Timer1Event_9,
		}TimerNotifyEvent;
}
#endif /* USSDK_OcxAdapterModel_h */
