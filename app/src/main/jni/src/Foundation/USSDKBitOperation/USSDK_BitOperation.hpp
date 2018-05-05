//
//  USSDK_BitOperation.hpp
//  UMOA_SDK
//
//  Created by moa_mini2 on 2018/3/14.
//  Copyright © 2018年 YueMingXingXi. All rights reserved.
//

#ifndef USSDK_BitOperation_hpp
#define USSDK_BitOperation_hpp

#include <stdio.h>

namespace NS_USSDK_Foundation
{
	template<typename T>
	T BitAdd(const T& A, const T& B)
	{
		return A | B ;
	}

	template<typename T>
	T BitRemoveB(const T& A, const T& B)
	{
		return A & ~(B);
	}

	template<typename T>
	bool BitHasB(const T& A, const T& B)
	{
		return A & B;
	}
}

#endif /* USSDK_BitOperation_hpp */
