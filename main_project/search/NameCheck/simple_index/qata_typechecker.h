#ifndef _QATA_TYPE_CHECKER_H_
#define _QATA_TYPE_CHECKER_H_

#include <map>
#include <string>

using namespace std;

class TypeChecker
{
	public:
		static bool isValidFileName(const string& strname);
		static bool isDigitStr(const string& str);
		static bool isFloatStr(const string& str);
		static bool isAlphaStr(const string& str);
		static bool isAlpNumStr(const string& str);
		static bool isAlpNumSymStr(const string& str);
		static bool isBoolStr(const string& str);

		template<typename Type>
		static bool isInRange(Type test,Type begin,Type end)
		{
			return ( test >= begin && test < end )?true:false;
		}
		
		template<typename Type>
		static bool isEqual(Type test,Type compare)
		{
			return ( test == compare )?true:false;
		}
};



#endif
