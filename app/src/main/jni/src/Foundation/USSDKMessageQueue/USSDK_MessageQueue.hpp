//
//  USSDK_MessageQueue.hpp
//  USSDK_IOS
//
//  Created by moa_mini2 on 2018/3/21.
//  Copyright © 2018年 YueMingXingXi. All rights reserved.
//

#ifndef USSDK_MessageQueue_hpp
#define USSDK_MessageQueue_hpp

#include <stdio.h>

#include <vector>
#include <string>

#include "../USSDKMutex/USSDK_Mutex.hpp"

using namespace std;
using namespace NS_USSDK_Foundation;

namespace NS_USSDK_Foundation
{
    template <typename TMessagePara>
    class USSDK_MessageQueue
    {
    private:
        USSDK_Mutex m_mutex;
        vector<TMessagePara> m_Queue;
        
    public:
        void add(TMessagePara &aMessagePara)
        {
            m_mutex.lock();
            m_Queue.push_back(aMessagePara);
            m_mutex.unlock();
        }

		void add(TMessagePara *aMessagePara)
		{
			m_mutex.lock();
			m_Queue.push_back(*aMessagePara);
			m_mutex.unlock();
		}
        
        void remove()
        {
            m_mutex.lock();
            m_Queue.erase(m_Queue.begin());
            m_mutex.unlock();
        }
        
        TMessagePara get()
        {
            m_mutex.lock();
            TMessagePara element = m_Queue[0];
            m_Queue.erase(m_Queue.begin());
            m_mutex.unlock();
            return element;
        }
        
        void Clear()
        {
            m_mutex.lock();
            m_Queue.clear();
            m_mutex.unlock();
        }
    };
}

#endif /* USSDK_MessageQueue_hpp */
