/**
@namespace kxutil2::text 文本处理.
@file text.h 文本基本的操作.
<pre>
1.类型转换和判断
2.比较
3.大小写转换
4.查找
5.替换
6.trim
7.分隔和拼接
8.位置安全偏移和纠正
9.格式化
10.其它
</pre>
*/

#ifndef	_TEXT_H
#define _TEXT_H
#include <boost/regex.hpp>
#include <string>
#include <vector>
#include <set>
#include <utility>
#include <sstream>
using std::string;
using std::vector;
using std::set;
using std::pair;
using std::istringstream;
using std::ostringstream;

namespace kxutil2{ namespace text{
/*-------------------类型转换,判断------------------------*/
/// 通用转换函数:字符串转换为任意类型(只要该类型支持operator<<)
template < typename T > 
extern T fromString (const string & s , int nRadix=10)  /// by onebird string转成 任意支持cout<<类型转换
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
extern string toString (const T & t ,short nRadix = 10 )  /// 任意支持cout<<类型 转换为string by onebird
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
///整型向字符串的转换.
extern string IntToStr (unsigned long iValue);

///浮点型向字符串转换.
extern string FloatToStr (float iValue);

///字符串向整型的转换.
extern unsigned long StrToInt (const string & strValue);
///字符串向浮点型的转换.
extern unsigned long StrToFloat (const string & strValue);

/// 转化中文数字为阿拉伯数字，限于非负整数，不能解析亿(数据溢出)
/// 接受以下中文数字：一 二 三 四 五 六 七 八 九 十 百 千 万 两 零 壹 贰 叁 肆 伍 陆 柒 捌 玖 拾 佰 仟 萬
extern bool ChineseNum2AlabNum(string& strChineseNum, size_t& nAlabNum);  

/// str是否完全是数字.
extern bool IsNumberic (string str);


/*-------------------比较------------------------*/
/// 比较str1和str2的前nLen字节是否相等,忽略大小写,by lvwei.
extern int StrCaseCmp(const char* pStr1, const char* pStr2, size_t nLen);
extern int StrCaseCmp(const string&str1,const string&str2);

/*-------------------大小写转换------------------*/
/// 简单的小写转换,不安全
extern void  ToLower(string &str); 
/// 简单的大写转换,不安全
extern void  ToUpper(string &str); 
/// 可指定范围的安全小写转换
extern bool SafeToLower( string  &  str,  size_t begPos ,  size_t endPos);
/// 安全的小写转换
extern void SafeToLower( string  &  str); 
/// 安全的大写转换
extern void SafeToUpper( string  &  str); 
/// 安全的小写转换,返回一个小写字符串
extern string SafeGetLower(const string & str); 
/// 安全的大写转换,返回一个大写字符串
extern string SafeGetUpper(const string & str); 

/*----------------------查找-------------------------*/
/// 查找strFind在strSrc中的位置，忽略大小写, by lvwei
extern size_t StrCaseFind(const string&strSrc,const string &strFind);
/// 右向查找strFind在strSrc中的位置，忽略大小写, by lvwei
extern size_t StrCaseRFind(const string&strSrc,const string &strFind);

/// 字符串的安全查找，返回的位置不会是一个汉字的尾字节
extern size_t SecureStringFind (const string & strStr, const string & strSub,size_t nBeginPos=0);

extern size_t SecureStringRfind (const string & strStr,const string & strSub,size_t nEndPos=string::npos);
  
extern bool TailMatch(const string &  little , const string & big);

/// 在strContent的nPos位置开始从左向右查找第nTurn个strSub，返回位置,by lvwei
extern size_t StrFind(const string &strContent,const string &strSub,int nTurn=1,size_t nOff=0);
/// 在strContent的nPos位置开始从右向左查找第nTurn个strSub，返回位置,by lvwei
extern size_t StrRfind(const string &strContent,const string &strSub,int nTurn=1,size_t nOff=string::npos);

/// 获取一个字符串中汉字的个数,by lvwei
extern unsigned long GetChineseCount(const string &strContent);
/// 获取一个英文字符的个数,by lvwei
extern unsigned long GetEnCharCount(const string &strContent);  
/// 获取一个数字的个数,by lvwei
extern unsigned long GetNumCharCount(const string &strContent); 
/// 从strContent中获取vecWordList中词汇的个数,by lvwei
extern int   GetWordCount(const string &strContent,const vector<string>&vecWordList);
/// 从strContent中获取vecWordList中词汇的个数,strAllMatch为所有匹配的词汇,调试方便　,by lvwei
extern int   GetWordCount(const string &strContent,const vector<string>&vecWordList,string &strAllMatch);
/// 从strContent中获取strWord个数,by lvwei
extern int   GetWordCount(const string &strContent,const string &strWord);

/// 得到一个字符串m~n间的内容(其中m,n为百分比),by lvwei
extern string  GetSubStr(const string &strContent,float m,float n);
/// 获取一个字符串strContent的迭代器itCur前|nCount|或后|nCount|个字符(注:nCount可以为正或负),by lvwei
extern string  GetSubStr(const string &strContent,const string::const_iterator &itCur,int nCount);  

/// 正则表达式安全匹配，防止匹配汉字时匹配上第一个汉字的后一字节+第二个汉字的前一字节,
/// 如用"约"能匹配"自己"中"自"的第二字节+"已"的第一字节,因为自己:D7 D4 BC BA　；约:D4 BC,by lvwei
extern bool SecurityRegexSearch(const string::const_iterator &itBegin,const string::const_iterator &itEnd,boost::smatch &what,const boost::regex &regexFilter,bool bIsSafe=true);
extern bool SecurityRegexSearch(const string &strContent,const boost::regex &regexFilter,bool bIsSafe=true);
extern bool SecurityRegexSearch(const string &strContent, boost::smatch & what,const boost::regex &regexFilter,bool bIsSafe=true);
/// 安全匹配,返回匹配数量,strMatch表示匹配结果的串拼接,by lvwei
extern int  SecurityRegexSearchCount(const string &strContent,const boost::regex &regexFilter,bool bIsSafe=true);
extern int  SecurityRegexSearchCount(const string &strContent,string &strAllMatch,const boost::regex &regexFilter,bool bIsSafe=true);

/// 若匹配上正则表达式，则得到正则表达式后边的字符串，否则返回原来的字符串
extern bool GetAfterRegex(const string &strStr ,string & strGet, boost::regex regexFilter);
/// 若匹配上正则表达式，则得到正则表达式前边的字符串，否则返回原来的字符串
extern bool GetBeforeRegex(const string &strStr ,string & strGet, boost::regex regexFilter);

/// 从字符串中查找数字:从nPos开始,numberStartPos为返回位置
extern float GetFirstFloatNumber(const string &strContent,size_t nPos=0,size_t *numberStartPos=NULL);
extern int GetFirstIntNumber(const string &strContent,size_t nPos=0,size_t *numberStartPos=NULL);
extern float GetLastFloatNumber(const string &strContent,size_t nPos=string::npos,size_t *numberStartPos=NULL);
extern int GetLastIntNumber(const string &strContent,size_t nPos=string::npos,size_t *numberStartPos=NULL);



/*------------------------替换------------------*/
/// 把str中的vecReplaced表中的词汇替换成rStr,nType 不为零则为安全替换
extern int  ReplaceStrStr(string & str ,const  string &  strReplaced,const string & rStr =" ",int nType=0);	
/// byonebird 将一系列字符串换成指定字符串
extern int  ReplaceStrVec(string & str ,const  vector<string>  &vecReplaced ,const string & rStr =" ");	
/// 把连续(\r|\n|\t| |　)+替换成rStr(默认转换成英文空格)
extern int  ReplaceBlanksToStr(string & str,const string &rStr=" ");   
/// 把str中的(\r|\n|\t)代替成rStr,改进by lvwei 2006-08-11 删除\r\n\t by onebird
extern int  ReplaceRNT(string & str ,const string & rStr =" ");	
/// 合并连续的(\r|\n|\t| |　)，如果只有空格，替换为空格，否则，替换为rStr, by yangjian 2007-12-7
extern int  ReplaceRNTBToStr(string &str, const string &rStr="\n");

/*-----------------------------trim-----------------*/
/// 增加第二个参数，0表示去掉英文和中文空格，1表示去掉英文空格，2表示去掉中文空格
extern void trim(string & str, int flag = 0); 
extern void trim(string & srcStr ,const string & remStr );  /// strim 特殊的字符串 前后都trim by onebird
extern void LTrimAll(string & str , const string & allStr);
extern void RTrimAll(string & str , const string & allStr);
extern void TrimAll(string & str , const string & allStr);

/*-----------------------------分隔和拼接----------------*/
/// segment a line by a seperator
extern void Segment(const string & strInput, char chSeperator,vector < string > &vecSegments);
extern void Segment(const string& strInput, vector<char> vecSeperator, vector<string>& vecSegments);
extern void Segment(const string & strInput,const string& strSeperator,vector<string>&vecSegments);
extern void SegmentIgnoreRepeat(const string& strInput, vector<char> vecSeperator, vector<string>& vecSegments);
extern void SecureSegment(const string& strInput, const string&  strSeperator, vector<string>& vecSegments);

/// 把vecSrc中所有字符串拼接起来，中间加空格,返回拼后的内容
extern string GetStrFromVec(const vector<string>&vecStr, const string& strJoin="");
extern string GetStrFromSet(const set<string>& setStr, const string& strJoin="");

/*-----------------------------位置安全偏移和纠正-----------------*/
/// 获取strContent中某一个迭代器itCur偏移nOff后位置后的结果结果,by lvwei
extern string::const_iterator StrIterOff(const string &strContent,const string::const_iterator &itCur,int nOff);
/// 为防止出现截断汉字的情况,对strContent从nCurPos位置开始向右计nDistance个字符
extern size_t SafeRightDistPos(const string &strContent, size_t nCurPos, size_t nDistance);
/// 为防止出现截断汉字的情况,对strContent从nCurPos位置开始向左计nDistance个字符
extern size_t SafeLeftDistPos(const string &strContent, size_t nCurPos, size_t nDistance);

/// 判断当前迭代器是否指向一个GBK汉字的尾字节，若是，将其调整到正确的位置。
extern bool AdjustItr (string::const_iterator itrStart,string::const_iterator & itrCurrent);

/// 判断当前位置是否指向GBK汉字的尾字节，若是，调整正确
extern bool AdjustChinesePos(const string &strText,int nRightPos, int &nCurPos);


/*----------------------------格式化--------------------*/
extern string GetStrWithoutDigitSpace( const string & str);

extern void CleanTxt(const string &strSrc, string &strDst,string &strErased);
extern void CleanTitleStr (string & strTitle); 
extern int _GetByPassToken(string &str,int &pos,int beginPos);	

extern void CleanRepeatedBlank( string & strText);

///  去除中文空格和英文空格　 by dongfei
extern void CleanBlank(string& strText);

/// 清除strText匹配正则表达式的所有文本
extern void CleanRegexTxt(string & strText, boost::regex regex);

/// 清除strText匹配正则表达式的所有文本,vecText返回被正则表达式分开的各个字符串，如果是清除的内容bool变量为true,否则为false
extern void CleanRegexTxt(string & strText, boost::regex regex, vector<pair<string,bool> > &vecText);

/// 调整itrStart,过虑掉分隔符以前的字符 .。存在中英文空格, ，\t : ： ; ；等则返回ture
extern bool FiltSpacer (string::const_iterator & itrStart,string::const_iterator & itrEnd);			  	

extern int ResetStrZeroChar(string & str , const char ch = ' ' );/// by onebird
/// 把一个字符串格式化成适合做为sql语句的字符串,如把'转换成\'
extern void NormalizeSqlContent(string &strContent);
/// 把一个字符串格式化成适合做为文件名的字符串
extern void NormalizeFileName(string &strFileName);
///  整理strContent, 大写变小写，'\t', '\n', '\r', '&nbsp'变空格，全角数字英文变半角
extern void NormalizeContent(string &strContent, bool bCutTooLong = true);  
extern int _clearUnicodeHtmlTag(string& strBody);

/*-------------------------------其他----------------------------------*/
/// 判断strCheckContent是否在strContent的html标签内:如f1234是在<a value="f1234">内
extern bool IsInHtmlTag(const string &strContent,const string &strCheckContent);
/// 老死链检测用到的一个函数
void ProcessRawPageForCheck(const string &strRawBody, string &strResult);

};};
#endif
