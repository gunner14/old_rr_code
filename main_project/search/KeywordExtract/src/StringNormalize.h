#ifndef __STRING_NORMALIZE_H__
#define __STRING_NORMALIZE_H__

#include <string>
#include <vector>
#include <map>
using namespace std;

namespace com{
namespace renren{
namespace search{
namespace utility{

enum NormalOPT{
    OPT_SPACE       = 1<<0, // 留下空格
    OPT_ALPHA       = 1<<1, // 留下字母
    OPT_NUMBER      = 1<<2, // 留下数字
    OPT_CHINESE     = 1<<3, // 留下汉字
	OPT_PUNCT       = 1<<4, // 留下常见标点符号
	OPT_FULLTOHALF  = 1<<5, // 字母;数字;常见标点的全角转半角
	OPT_TCCTOSCC    = 1<<6, // 繁体转简体
	OPT_UPPTOLOW    = 1<<7, // 所有大写转化为小写
	OPT_LOWTOUPP    = 1<<8  // 所有小写转化为大写
};

std::string ws2s(const std::wstring &ws);
std::wstring s2ws(const std::string &s);
std::wstring normalize(const std::wstring &ws, uint32_t opt, bool spaceFill = false);
std::string normalize(const std::string &s, uint32_t opt = OPT_ALPHA|OPT_NUMBER|OPT_CHINESE|OPT_UPPTOLOW, bool spaceFill = false);
std::map<string,int> split(const std::string &s,int minLen,int maxLen);
void splits(const std::string & s, char c, std::vector<string> & v);
void splitws(const std::wstring & s, char c, std::vector<wstring> & v);

}
}
}
}
#endif
