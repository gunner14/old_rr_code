/**
 *@file ImpersonalChecker.cpp
 *@author zehua.hong@renren-inc.com
 *@brief provided the impersonal name entity recognize.
 */
#include "ImpersonalChecker.h"
#include "StringNormalize.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace com::renren::search::utility;
using namespace std;

static const char SURNAME_DICT[] = "surname_dict";
static const char NICKNAME_DICT[] = "nickname_dict";

void ImpersonalChecker::SetWorkPath(const string &str){
	m_strWorkPath = str;
}

void ImpersonalChecker::Init(){
	string szSurnameDict = m_strWorkPath + "/" + SURNAME_DICT;
	string szNicknameDict = m_strWorkPath + "/" + NICKNAME_DICT;
	string line = "";
	
	ifstream surnameFileInput(szSurnameDict.c_str());
	if (!surnameFileInput.is_open()){
		cerr << "open " << szSurnameDict << " failed." << endl;
		return;
	}
	while (getline(surnameFileInput, line))
		m_surname.insert(line);
	
	ifstream nicknameFileInput(szNicknameDict.c_str());
	if (!nicknameFileInput.is_open()){
		cerr << "open " << szNicknameDict << " failed." << endl;
		return;
	}
	while (getline(nicknameFileInput, line))
		m_nickname.insert(line);
}

bool ImpersonalChecker::isPrefixSurname(const string &str){
	int len = str.length();
	if (len < 3) return false;
	string singleSurname = str.substr(0, 3);
	if (m_surname.find(singleSurname) != m_surname.end() && len >= 6 && len <= 9) return true;
	if (len < 6) return false;
	string doubleSurname = str.substr(0, 6);
	if (m_surname.find(doubleSurname) != m_surname.end() && len >= 6 && len <= 12) return true;
	return false;
}

bool ImpersonalChecker::isNickname(const string &str){
	return (m_nickname.find(str) != m_nickname.end());
}

bool ImpersonalChecker::isImpersonal(const string &str){
	if (str.length() > 15) return true;
	string normalizedStr = normalize(str, OPT_CHINESE | OPT_ALPHA);
	return (!isPrefixSurname(normalizedStr) && !isNickname(normalizedStr));
}
