//
//  DatabaseAgentHeader.h
//
//  Created by drz on 2018/4/23.
//  Copyright © 2018年 drz. All rights reserved.
//

#ifndef DatabaseAgntProc_hpp
#define DatabaseAgntProc_hpp

#ifdef _APPLE
#include "sqlite5.h"
#else 
#include "sqlite3.h"
#endif

#include "USSDK_DatabaseModel.h"
#include "USSDK_DatabaseService.h"

#endif /* DatabaseAgentHeader */
