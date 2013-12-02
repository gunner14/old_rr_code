#ifndef __AB_COMMON_H__
#define __AB_COMMON_H__

#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include "AbStd.h"
#include "AbTypeDef.h"
 
class AbCommon{
public:
    static bool  IsExistProccess(char const* appName);
	static char* GetAppName(char* appPathName);
	static bool GetDateTime(string& datetime);
	static bool GetDateTime(time_t tt, string& datetime);
	static bool GetDateTime(const string& old, int second, string& strNew);
	static bool GetDateTimeNoFormat(time_t tt, string& datetime);
	static time_t GetTimeByY4MDHMS2(const string& date);
    static void TrimDatetimeToHour(time_t & ttDatetime);
    static void TrimDatetimeToDay(time_t & ttDatetime);
	static int	GetYear(time_t tt);
	static int	GetMonth(time_t tt);
	static int	GetDay(time_t tt);
	static int	GetHour(time_t tt);
	static int	GetMinute(time_t tt);
	static int	GetSecond(time_t tt);
    static void GetYMonDay(time_t tt, UINT32& uiYmd);
	static bool CreateNoExistDirectory(const string& path);
	static bool IsExistFile(const string& strFile);
	static bool IsDirectory(const string& strFile);
	static int  GetFileSize(const string& strFile);
    static off_t GetFileSize(const string& strFile, bool& bSuccess);
    static time_t GetFileModifyDatetime(const string& strFile);
	static bool CreateDirectory(const string& strFile);
	static bool MoveFile(const string& src, const string& dest, bool bzip);
	static bool DelFile(const string& strFile);
	static bool DelDirectory(const string& strPath);
	static bool ChangeDirectory(const string& strPath);
	static bool GetDirFile(const string& strPath, list<string>& files);
	static void GetLastDirectoryName(const string& dir, string& last);
	static void CopyString(char* dest, const char* src, size_t len);
    static void CopyString(char* desc, const char* src, size_t desc_len, size_t src_len);
	static void split(const string& src, list<string>& value);
	static void split(const string& src, list<string>& value, char ch);
	static void splitIgnoreSpace(const string& src, list<string>& value, char ch);
	static void split(const string& src, list< pair<string, string> >& value, char ch);
	static bool Pair(const string& src, pair<string, string>& value);
	static bool GetPair(const list< pair<string, string> >& values, const string& name, pair<string, string>& item);
	static bool ReadLine(FILE* fd, string& line);
	static bool ReadData(FILE* fd, string& data, int size);
	static bool IsInteger(const string& value);
	static bool IsDouble(const string& value);
	static void trimIncludeTab(string& value);
    static void trim(string& value, bool bIncludeTab = false);
    static void trim(char* value, bool bIncludeTab = false);
    static void rtrim(char* value);
	static bool ToWebDomain(char* szUrl);
	static void ReplaceAll(string& strSrc, const string& str1, const string& str2);
    static void ReplaceAll(char const* src, char* dst, size_t& dst_len, char const* from, char const* to);
	static bool LoadFile(string const& strFileName, string& strData);

	static size_t length(LONGLONG value);
	static size_t length(int value);
	static size_t length(unsigned int value);
	static size_t length(short value);
	static size_t length(unsigned short value);
	static size_t length(double value);
	static size_t length(char value);
	static size_t length(unsigned char value);
	static void toString(LONGLONG data, string& value);
	static void toString(int data, string& value);
	static void toString(unsigned int data, string& value);
	static void toString(short data, string& value);
	static void toString(unsigned short data, string& value);
	static void toString(double data, string& value);
	static void toString(char data, string& value);
	static void toString(unsigned char data, string& value);
    static void Utf8ToAscii(char* src, size_t src_len, char* desc, size_t& dest_len, bool bEscapeUri);
    static void BinnaryToHex(char  const* src, size_t src_len, char* desc, size_t& dest_len);
    static bool IsValidGbkStr(char const* src, size_t src_len);
    static void RemoveSpace(char * str, bool bIncludeEnglish=false);
    static void RemoveDigestSpace(char * str);
    static void MarkHtmlRed(char const* src, size_t src_len, char const* words, char* dest, size_t& dest_len);
    static void SameMemCopy(char* dest, char const* src, size_t len);
    static void ToLower(char* src);
    static void ToUpper(char* src);
    static bool IsGroup(UINT32 id, UINT16 unGroup, UINT16 unGroupIndex);

	static int strgbkcasecmp(char const* src, char const* dst);
	static int strngbkcasecmp(char const* src, char const* dst, size_t n);
	static void strgbktolower(char* src);
	static void GBKToLower(char* src);
	static bool HasGbk(char const* szKeyword);

	static void RemoveTailBracket(string& str);
	static bool EndWith(const string& src, const string& suffix);	
	static bool EndWith(const char* szSrc, const char* szSuffix);	
	static bool EndWithCase(const char* szSrc, const char* szSuffix);	

	static size_t hashgbkcase(char const* s1);
	static size_t hashstrcase(char const* s1);
    static size_t MemoryChangeCheck(char const* pszSrc, char const* pszDst, size_t& uiLen){
        size_t begin;
        size_t end;
        for (begin=0; begin<uiLen; begin++){
            if (pszSrc[begin]^pszDst[begin]) break;
        }
        for (end=uiLen; end>begin; end--){
            if (pszSrc[end-1]^pszDst[end-1]) break;
        }
        uiLen = end - begin;
        return begin;
    }

	static bool LoadBinaryFile(string const& strFileName, char* szFileBuf, int& iBufLen);
	static bool IsSingleChinese(const char* szSrc);
	//change 全角到半角，替换大写到小写
	static void SBCToDBC(char* src1);
	//remove punctuation,remain only 0-9,a-z,A-Z,gbk,remove multi space to one
	static void RemovePunctuation(char* src1);
    static UINT32 Inc(UINT32 num, UINT32 max, UINT32 inc);
    static UINT32 Dec(UINT32 num, UINT32 min, UINT32 dec);
};

#endif

