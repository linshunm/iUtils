//
//  USSDK_RosterListModel.h
//  USSDK-Xcode
//
//  Created by YMXX on 2018/3/17.
//  Copyright © 2018年 YMXX. All rights reserved.
//

#ifndef USSDK_RosterListModel_h
#define USSDK_RosterListModel_h

#include <stdio.h>
#include <string>
#include <map>
#include <vector>

using namespace std;

namespace NS_USSDK_RosterService
{
	template<typename T>
	class USSDK_RosterList
	{
    public:
        typedef map<string, T> ElementMapType;
        
	private:
		map<string, T> elementMap;

	public:
		void cover(string key, T &element)
		{
			elementMap[key] = element;
		}

		bool add(string key, T &element)
		{
			if (hasElement(key))
			{
				return false;
			}

			elementMap[key] = element;
			return true;
		}

		bool remove(string key)
		{
			if (!hasElement(key))
			{
				return true;
			}
			
			elementMap.erase(key);
			return true;
		}

		bool update(string key, T &element)
		{
			if (!hasElement(key))
			{
				return false;
			}

			elementMap[key] = element;
			return true;
		}

		bool getElement(string key, T& outBuffer)
		{
			if (hasElement(key))
			{
				outBuffer = elementMap[key];
				return true;
			}

			return false;
		}

		map<string, T> getAll()
		{
			return elementMap;
		}

		bool hasElement(string key)
		{
            typename ElementMapType::iterator it = elementMap.find(key);

			if (it != elementMap.end())
			{
				return true;
			}
			
			return false;
		}

		int countOfKey()
		{
			return elementMap.size();
		}

		void clear()
		{
			elementMap.erase(elementMap.begin(), elementMap.end());
		}

		void moveFrom(USSDK_RosterList<T> aSourceList, T & anElement, string aKey)
		{
			cover(aKey, anElement);
			aSourceList.remove(aKey);
		}

		void moveTo(USSDK_RosterList<T> aDestList, T & anElement, string aKey)
		{
			remove(aKey);
			aDestList.cover(aKey, anElement);
		}

		void getNotAtList(USSDK_RosterList<string> aList, vector<T> &out)
		{
			typename ElementMapType::iterator it = elementMap.begin();

			while(it != elementMap.end())
			{
				string key = it->first;
				if (!aList.hasElement(key))
				{
					out.push_back(it->second);
				}
				it ++;         
			}
		}
	};

}
#endif /* USSDK_RosterListModel_h */
