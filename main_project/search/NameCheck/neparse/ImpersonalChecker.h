#ifndef __IMPERSONAL_CHECKER_H__
#define __IMPERSONAL_CHECKER_H__
/**
 *@file ImpersonalChecker.h
 *@author zehua.hong@renren-inc.com
 *@brief provided the impersonal name entity recognize.
 */

#include <set>
#include <string>
using namespace std;

class ImpersonalChecker{
	public:
	void Init();
	void SetWorkPath(const string &str);
	bool isImpersonal(const string &str);
	
	private:
	bool isPrefixSurname(const string &str);
	bool isNickname(const string &str);
	string m_strWorkPath;
	set<string> m_surname, m_nickname;
};

#endif
