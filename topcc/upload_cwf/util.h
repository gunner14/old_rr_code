#ifndef _UTIL_H_
#define _UTIL_H_

#include <string>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <iomanip>
#include <utility>
using std::string;
using std::vector;
using std::map;
using std::set;
using std::pair;
using std::istringstream;
using std::ostringstream;

/*****************字符串操作*****************/

/// 通用转换函数:字符串转换为任意类型(只要该类型支持operator<<)
template < typename T > 
extern T FromString (const string & s , int nRadix=10)  
{
	istringstream iss (s);
	T t;
	if(16 == nRadix)
		iss >>std::hex>> t;
	else if(8 == nRadix)
		iss >>std::oct>> t;
	else
		iss >> t;

	return t;
}

/// 通用转换函数:任意类型(只要该类型支持operator>>)转换为字符串
template < typename T > 
extern string ToString (const T & t ,short nRadix = 10 ) 
{
	ostringstream oss;
	if(16 == nRadix)
		oss << std::hex<<t;
	else if(8 == nRadix)
		oss << std::oct <<t;
	else
		oss << t;
	return oss.str();
}

extern size_t StrCaseFind(const string& strSrc, const string& strSub,int nIndex=0);

extern int StrCaseCmp(const string&str1,const string&str2,int nCount=0);

extern string ToLower(const string &str); 

extern string ToUpper(const string &str);  

extern void Split(const string& strInput, const string& strSep, vector<string>& vecSeg);

extern void Split(const string& strInput, const string& strSep, set<string>& setSeg);

extern string Trim(const string & str, const string& chars=" \t\r\n"); 

/*****************文件操作*****************/

//创建目录,支持递归,如/dir1/dir2/dir3
bool Mkdir(const string &strDirName);

extern string LoadFile(const string &path,size_t nStartOffset=0);

extern bool SaveFile(const string &path,const string& content);

extern bool SaveFile(const string &path, const void* content, int length);

extern bool AppendFile(const string &path,const string& content);

extern uint32_t ScanDir(const string& strQuery,vector<string>& vecFileName);

/*****************编码解码操作*****************/

extern char Dec2HexChar(short int n);

extern short int HexChar2Dec(char c);

/*将字符串转成相应的16进制串*/
extern string Str2Hex(const string& input);
extern string Str2Hex(unsigned char *p, int len);

/*将16进制串转成相应的字符串，输入串长度必须是2的整数倍(每两个对应一个输出字符)*/
string Hex2Str(const string& input);


///把一个strSep分隔的字符串strContent转化成一个pair
template<typename T>
bool Str2Pair(const string& strContent, pair<T,T>& p, const string& strSeg=",")
{
	vector<string> vecSeg;
	Split(strContent,strSeg,vecSeg);
	if(vecSeg.size()<2)
		return false;
	p.first=FromString<T>(vecSeg[0]);
	p.second=FromString<T>(vecSeg[1]);
	return true;
}

/*****************网络相关*****************************/

///编码
extern string EncodeUrl(const string &strURL);
///解码
extern string DecodeUrl(const string &strURL);
///获取一个header
string GetHeader(const string &strHeaders,const string&strHeaderName);
void GetHeader(const string &strHeaders,const string&strHeaderName, vector<string>& vecHeaderValue, bool bGetAll=true);
///获取url中的path
string GetHttpPath(const string&strUrl);


/*****************随机文件名生成操作依赖函数*************************/
/*根据key对输入值进行编码,输入值是不同大小图片的8字节唯一字符串,key则是"arge","main","head","tiny"对应的16进制输出的8字节字符串*/
extern string EncodePrefixStr(const string& input, const string& key);

uint32_t GetIP(const string &interface);

string GetUniqueStr();

string GetTimeStr();

string GetRandomNumber(const int& size);

extern string GetRandomStr(const int& size);

extern int GetRandomNumber(const int& from, const int& to);

/****************时间日期函数***************/
///tm到str的转换
extern string TmToStr(const tm& tmTime);
///str到tm的转换:字符串不合法,则返回false
extern bool StrToTm(const string& strTime, tm& tmTime);

///time_t到tm的转换
extern void TimeToTm(const time_t &tTime,tm &tmTime);
///tm到time_t的转换
extern time_t TmToTime(struct tm &tmTime);

///time_t向str的转换
extern string TimeToStr(const time_t &tTime);
///str向time_t的转换:字符串不合法,则返回0
extern time_t StrToTime(const string &strTime);

///获取当前时间,
extern uint64_t GetMilliSecond();

///按照local日期比较是否是同一天，是则返回0，tTime1st表示的日期小则返回-1，否则返回1， by xuzheng
extern int CompareDate(time_t tTime1st, time_t tTime2nd);


/**********记日志***********/
//extern bool Log(const string& strLog);
//类似printf似的记日志
extern bool Log(const char* fmt, ...);

/*处理文件名,获取真实文件名,主要针对IE上传用户*/
extern string GetFileName(const string& strFileName);

/*将GBK编码的字符串转换为utf-8编码的字符串,目前用户控件上传的文件名*/
extern string GBKToUTF8(const string& src);

/*计算输入string的md5*/
extern string StrMd5(const string& data);
#endif
