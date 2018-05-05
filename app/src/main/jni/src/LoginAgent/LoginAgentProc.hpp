//
//  LoginAgentProc.hpp
//  USSDK-Xcode
//
//  Created by YMXX on 2018/3/16.
//  Copyright © 2018年 YMXX. All rights reserved.
//

#ifndef LoginAgentProc_hpp
#define LoginAgentProc_hpp

namespace NS_USSDK_LoginService
{
	void dealLoginAdapterReq(void *aReq);
	void loginAgentThreadInit();
	void loginAgentThreadUnInit();
}

#endif /* LoginAgentProc_hpp */
