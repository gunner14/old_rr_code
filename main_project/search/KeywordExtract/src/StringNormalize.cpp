#include <string>
#include <locale.h>
#include <iostream>
using namespace std;
#include "StringNormalize.h"
using namespace com::renren::search::utility;

const int normal_punct_count = 32;
const char normal_punct[] = {
    '!', '\"', '#', '$', '%',
	'&', '\'', '(', ')', '*',
	'+', ',',  '-', '.', '/',
	':', ';',  '<', '=', '>',
	'?', '@',  '[', '\\', ']',
	'^', '_',  '`', '{', '|',  
	'}', '~'
};

std::string com::renren::search::utility::ws2s(const std::wstring &ws)
{
    setlocale(LC_ALL, "zh_CN.utf8");
    const wchar_t* _Source = ws.c_str();
    size_t _Dsize = 3 * ws.size() + 1;
    char *_Dest = new char[_Dsize];
    memset(_Dest,0,_Dsize);
    wcstombs(_Dest,_Source,_Dsize);
    std::string result = _Dest;
    delete []_Dest;
    return result;
}

std::wstring com::renren::search::utility::s2ws(const std::string &s)
{
    setlocale(LC_ALL, "zh_CN.utf8");
    const char* _Source = s.c_str();
    size_t _Dsize = s.size() + 1;
    wchar_t *_Dest = new wchar_t[_Dsize];
    wmemset(_Dest, 0, _Dsize);
    mbstowcs(_Dest,_Source,_Dsize);
    std::wstring result = _Dest;
    delete []_Dest;
    return result;
}

std::wstring com::renren::search::utility::normalize(const std::wstring &ws, uint32_t opt, bool spaceFill)
{

    std::wstring res = ws;
	for (int i=res.length()-1; i>=0; --i){
        if (opt & OPT_FULLTOHALF) {
			// 字母;数字;字符 全角转半角，功能预留, 暂未实现
		}
		
		if (opt & OPT_TCCTOSCC){
			// 繁体转简体 功能预留，暂未实现
		}
		
		if (opt & OPT_UPPTOLOW) {
			// 大写转小写
			if (res[i] >= 'A' && res[i] <= 'Z') res[i] = res[i] - 'A' + 'a';
		} else
		if (opt & OPT_LOWTOUPP) {
			// 小写转大写
			if (res[i] >= 'a' && res[i] <= 'z') res[i] = res[i] - 'a' + 'A';
		}

		if (opt & OPT_SPACE) {
			if (isspace(res[i])) continue;
		}

		if (opt & OPT_ALPHA) {
			if (isalpha(res[i])) continue;
		}

		if (opt & OPT_NUMBER) {
			if (isdigit(res[i])) continue;
		}

		if (opt & OPT_CHINESE) {
			if (res[i] >= 0x4E00 && res[i] <= 0x9FA5) continue;
		}
		
		if (opt & OPT_PUNCT) {
			bool isPunct = false;
			for (int j=0; j<normal_punct_count; ++j)
				if (res[i] == normal_punct[j]) {
					isPunct = true;
					break;
				}
			if (isPunct) continue;
		}
        if (spaceFill == false){
            res.erase(i, 1);
        }
        else{
            res[i] = ' ';
        }
    }
	return res;
}

std::string com::renren::search::utility::normalize(const std::string &s, uint32_t opt, bool spaceFill)
{
	return ws2s(normalize(s2ws(s), opt, spaceFill));
}

void com::renren::search::utility::splits(const std::string& s, char c, std::vector<std::string>& v)
{
    std::string::size_type i = 0;
    std::string::size_type j = s.find(c); 
    while(j != std::string::npos)
    { 
        v.push_back(s.substr(i, j-i));
        i = ++j;
        j = s.find(c, j);
    }//find start from position j
	v.push_back(s.substr(i));
}

void com::renren::search::utility::splitws(const std::wstring& s, char c, std::vector<std::wstring>& v)
{
    std::wstring::size_type i = 0;
    std::wstring::size_type j = s.find(c); 
    while(j != std::wstring::npos)
    { 
        v.push_back(s.substr(i, j-i));
        i = ++j;
        j = s.find(c, j);
    }//find start from position j
	v.push_back(s.substr(i));
}

std::map<string,int> com::renren::search::utility::split(const std::string &s,int minLen,int maxLen)
{
    std::wstring ws = s2ws(s);
    std::wstring wsn = normalize(ws, OPT_CHINESE | OPT_NUMBER | OPT_ALPHA, true);
    std::vector<wstring> vec;
    std::map<string,int> mapResult;
    splitws(wsn, ' ', vec);
    for(size_t i = 0;i < vec.size();i++)
    {
        int curLen = vec[i].length();
        if(curLen <= 1)
        {
            continue;
        }
        int maxthres = curLen > maxLen ? maxLen : curLen;
        for (int j = 0;j <= curLen - minLen;j++)
        {
            for (int k = minLen;k <= maxthres;k++)
            {
                if (j + k > curLen)
                    break;
                string curItem = ws2s(vec[i].substr(j,k));
                std::map<string,int>::iterator iter = mapResult.find(curItem);
                if(iter == mapResult.end())
                {
                    mapResult.insert(std::make_pair(curItem,1));
                }
                else
                {
                    iter->second += 1;
                }
            }
        }
    }
    return mapResult;
}

