#include <string>
#include <locale.h>
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

std::wstring com::renren::search::utility::normalize(const std::wstring &ws, uint32_t opt)
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
		res.erase(i, 1);
    }
	return res;
}

std::string com::renren::search::utility::normalize(const std::string &s, uint32_t opt)
{
	return ws2s(normalize(s2ws(s), opt));
}

