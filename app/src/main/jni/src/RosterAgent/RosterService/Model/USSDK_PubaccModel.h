//
//  USSDK_PubaccModel.h
//  USSDK-Xcode
//
//  Created by YMXX on 2018/3/17.
//  Copyright © 2018年 YMXX. All rights reserved.
//

#ifndef USSDK_PubaccModel_h
#define USSDK_PubaccModel_h

#include <stdio.h>
#include <string>
#include "USSDK_RosterModel.h"

using namespace std;
using namespace NS_USSDK_RosterService;

namespace NS_USSDK_RosterService
{
	typedef enum
	{
		PubaccAttribute_FullAttention = 1 << 0,
		PubaccAttribute_Private = 1 << 1,
		PubaccAttribute_SubscribeGroup = 1 << 2,

	}PubaccAttribute;

	class USSDK_Pubacc : public USSDK_Roster
	{
	public:
		string description;
		string enDescription;
		PubaccAttribute pubaccAttribute;

		string logoAddress;
		string subscribeCount;
		
		string menuEtag;
	};
}
#endif /* USSDK_PubaccModel_h */
