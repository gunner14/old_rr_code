#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>    
#include <sys/ioctl.h>    
#include <sys/time.h>    
#include <netinet/in.h>
#include <net/if.h>     
#include <net/if_arp.h>    
#include <arpa/inet.h> 
#include <time.h>
#include <unistd.h>
#include <stdarg.h>
#include <iconv.h>
#include <openssl/md5.h>
#include "base3/network.h"

#include "util.h"

size_t StrCaseFind(const string& strSrc, const string& strSub,int nIndex)
{
	size_t nPos = string::npos;
	char *p=strcasestr(strSrc.c_str()+nIndex,strSub.c_str());
	if(p)
		nPos = p-strSrc.c_str();
	return nPos;
}
int StrCaseCmp(const string& str1,const string& str2, int nCount)
{
	if(nCount==0)
		return strcasecmp(str1.c_str(),str2.c_str());
	else
		return strncasecmp(str1.c_str(),str2.c_str(),nCount);
}
string ToLower(const string &str) 
{  
	string tmp=str;
	for(size_t i=0;i<tmp.size();i++){
		char& c=tmp[i];
		if(c>='A'&&c<='Z')
			c|=32;   
	}
	return tmp;
}   

string ToUpper(const string &str)   
{   
	string tmp=str;
	for(size_t i=0;i<tmp.size();i++){
		char& c=tmp[i];
		if(c>='a'&&c<='z')
			c&=0xdf;   
	}
	return tmp;
}
void Split(const string& strInput, const string& strSep, vector<string>& vecSeg)
{
	if(strInput.empty() || strSep.empty())
		return;
	vecSeg.clear();
	vecSeg.reserve(strInput.size()/2);
	size_t nStart = 0;
	size_t nPos = string::npos;
	do{
		nPos = strInput.find(strSep,nStart);
		vecSeg.push_back(strInput.substr(nStart,nPos-nStart));
		nStart = nPos + strSep.size();
	}
	while(nPos!=string::npos);
}

void Split(const string& strInput, const string &strSep, set<string>& setSeg)
{
	if(strInput.empty())
		return;
	setSeg.clear();
	size_t nStart = 0;
	size_t nPos = string::npos;
	do{
		nPos = strInput.find(strSep,nStart);
		setSeg.insert(strInput.substr(nStart,nPos-nStart));
		nStart = nPos + strSep.size();
	}
	while(nPos!=string::npos);
}

string Trim(const string& str, const string& chars)
{
	if(str.empty())
		return "";

	string strNew=str;
	size_t nBeginPos=0;
	size_t nEndPos=str.size();	
	for(size_t i=0; i<=str.size(); i++){
		if(strchr(chars.c_str(),str[i])==NULL || i==str.size()){
			nBeginPos=i;
			break;
		}
	}
	for(size_t i=str.size()-1; i>=nBeginPos; i--){
		if(strchr(chars.c_str(),str[i])==NULL){
			nEndPos=i+1;
			break;
		}
	}
	if(!(nBeginPos==0 && nEndPos==str.size()))
		strNew=str.substr(nBeginPos,nEndPos-nBeginPos);
	return strNew;
}

bool Mkdir(const string &strDirName) //  目录全路径
{
  vector < string > vecDir; 
  string strDirName2 = Trim(strDirName);    
  if(strDirName2.empty())
    return false;
  
  string strParentDir = strDirName2[0]=='/'?"/":"";
  Split(strDirName2, "/", vecDir);
  for (size_t i = 0; i < vecDir.size (); i++)
  { 
    string strDirName=vecDir[i];
    if(strDirName=="")
      continue;
    string strDir = strParentDir+strDirName;
#ifdef WIN32
    mkdir (strDir.c_str ());
#else 
    mkdir (strDir.c_str (), S_IRWXU | S_IRWXG | S_IRWXO);
#endif

    strParentDir+=vecDir[i]+ "/";
  }
  return true;
}

bool Mkdir2(const string &strFileName) //  文件全路径
{
  vector < string > vecDir; 
  string strFileName2 = Trim(strFileName);    
  if(strFileName2.empty())
    return false;
  
  string strParentDir = strFileName2[0]=='/'?"/":"";
  Split(strFileName2, "/", vecDir);
  for (size_t i = 0; i < vecDir.size() - 1; i++)
  { 
    string strFileName=vecDir[i];
    if(strFileName=="")
      continue;
    string strDir = strParentDir + strFileName;
#ifdef WIN32
    mkdir (strDir.c_str ());
#else 
    mkdir (strDir.c_str (), S_IRWXU | S_IRWXG | S_IRWXO);
#endif

    strParentDir+=vecDir[i]+ "/";
  }
  return true;
}
string LoadFile(const string &path,size_t nStartOffset)
{
	string strContent;
	FILE *f=fopen(path.c_str(),"rb");
	if(f==NULL){
		if(path!="")
			fprintf(stderr,"Open file error:%s\n",path.c_str());
		return "";
	}
	else{
		fseek(f,nStartOffset,SEEK_SET);
		char buf[65536];
		int readLen=0;
		while(true) {
			readLen=fread(buf,sizeof(char),65536,f);
			if(readLen<=0)
				break;
			strContent.append(buf,readLen);		
		}
		fclose(f);
		return strContent;
	}
}

bool SaveFile(const string &path,const string& content) {
  return SaveFile(path, content, NULL);
}

bool SaveFile(const string &path,const string& content, int *err) {
   FILE *f=fopen(path.c_str(),"wb");
   if (f==NULL) {
     if (err)
       *err = errno;
     return false;
   } else {
     size_t w = fwrite(content.c_str(),1,content.length(),f);
     fclose(f);
     if(w == content.length()) {
       return true;
     } else {
       if (err)
         *err = errno;
       return false;
     }
   }
}

bool AppendFile(const string &path,const string& content)
{
        FILE* f=fopen(path.c_str(),"ab");
        if(f==NULL){
		if(path!="")
			printf("Open file error: %s\n",path.c_str());
                return false;
	}
	else{
		fwrite(content.c_str(),1,content.length(),f);
		fclose(f);
        	return true;
	}
}
uint32_t ScanDir(const string& strQuery,vector<string>& vecFileName)
{
	vecFileName.clear();
	
	string strDir=".";
	string strFilePattern=strQuery;
	size_t nPos=strQuery.rfind("/");
	if(nPos!=string::npos){
		strDir=strQuery.substr(0,nPos);
		strFilePattern=strQuery.substr(nPos+1);
	}
	
	string strPrefix=strFilePattern;
	string strPostfix="";
	nPos=strFilePattern.find('*');
	if(nPos!=string::npos){
		strPrefix=strFilePattern.substr(0,nPos);
		strPostfix=strFilePattern.substr(nPos+1);
	}
	
	DIR *pDir=opendir(strDir.c_str());
	if(pDir==NULL)
		return 0;
	struct dirent *pDirent;
	while((pDirent=readdir(pDir))!=NULL){
		string strFileName=pDirent->d_name;
		if(strFileName.size()<strPrefix.size()+strPostfix.size())
			continue;
		if(strFileName.substr(0,strPrefix.size())==strPrefix
				&&strFileName.substr(strFileName.size()-strPostfix.size())==strPostfix)
			vecFileName.push_back(strDir+"/"+strFileName);
	}	
	closedir(pDir);
	return vecFileName.size();	
}

char Dec2HexChar(short int n) {
	if ( 0 <= n && n <= 9 ) {
		return char( short('0') + n );
	} else if ( 10 <= n && n <= 15 ) {
		return char( short('a') + n - 10 );
	} else {
		return char(0);
	}
}

short int HexChar2Dec(char c) {
	if ( '0'<=c && c<='9' ) {
		return short(c-'0');
	} else if ( 'a'<=c && c<='f' ) {
		return ( short(c-'a') + 10 );
	} else if ( 'A'<=c && c<='F' ) {
		return ( short(c-'A') + 10 );
	} else {
		return -1;
	}
}

string Str2Hex(const string& input)
{
	int size = input.length();
	char *buf = new char[2*size + 1];
	for(int i=0; i<size; i++){
		char c = input[i];
		short int a = c & 0xF0;
		short int b = c & 0x0F;
		a = a >> 4;
		buf[2*i] = Dec2HexChar(a);
		buf[2*i + 1] = Dec2HexChar(b);
	}
	buf[2*size] = 0;
	string s;
	s.append(buf);
	delete[] buf;
	return s;
}

string Str2Hex(unsigned char *input, int len)
{
	string result;
	for(int i=0; i<len; i++){
		char c = input[i];
		short int a = c & 0xF0;
		short int b = c & 0x0F;
		a = a >> 4;
		result.push_back(Dec2HexChar(a));
		result.push_back(Dec2HexChar(b));
	}
	return result;
}

string Hex2Str(const string& input)
{
	int size = input.length();
	if(size%2 != 0)
		return "";
	int length = size / 2;
	char *buf = new char[length + 1];
	for(int i=0; i<length; i++){
		char c1 = input[2*i];
		char c2 = input[2*i + 1];
		short int a = HexChar2Dec(c1);
		short int b = HexChar2Dec(c2);
		a = a << 4;
		char c = (char)(a+b);
		if(0 == c)
			return "";
		buf[i] = c;
	}
	buf[length] = 0;
	string s;
	s.append(buf);
	delete[] buf;
	return s;
}


string EncodeUrl(const string &strUrl)
{
	const static unsigned char urlchr_table[256] =
	{
		1,  1,  1,  1,   1,  1,  1,  1,   /* N1L SOH STX ETX  EOT ENQ ACK BEL */
		1,  1,  1,  1,   1,  1,  1,  1,   /* BS  HT  LF  VT   FF  C1  SO  SI  */
		1,  1,  1,  1,   1,  1,  1,  1,   /* DLE DC1 DC2 DC3  DC4 NAK SYN ETB */
		1,  1,  1,  1,   1,  1,  1,  1,   /* CAN EM  S1B ESC  FS  GS  1S  1S  */
		1,  0,  1,  1,   0,  1,  1,  0,   /* SP  !   "   #    $   %   &   '   */
		0,  0,  0,  1,   0,  0,  0,  1,   /* (   )   *   +    ,   -   .   /   */
		0,  0,  0,  0,   0,  0,  0,  0,   /* 0   1   2   3    4   5   6   7   */
		0,  0,  1,  1,   1,  1,  1,  1,   /* 8   9   :   ;    <   =   >   ?   */
		1,  0,  0,  0,   0,  0,  0,  0,   /* @   A   B   C    D   E   F   G   */
		0,  0,  0,  0,   0,  0,  0,  0,   /* H   I   J   K    L   M   N   O   */
		0,  0,  0,  0,   0,  0,  0,  0,   /* P   Q   R   S    T   1   V   W   */
		0,  0,  0,  1,   1,  1,  1,  0,   /* X   Y   Z   [    \   ]   ^   _   */
		1,  0,  0,  0,   0,  0,  0,  0,   /* `   a   b   c    d   e   f   g   */
		0,  0,  0,  0,   0,  0,  0,  0,   /* h   i   j   k    l   m   n   o   */
		0,  0,  0,  0,   0,  0,  0,  0,   /* p   q   r   s    t   u   v   w   */
		0,  0,  0,  1,   1,  1,  1,  1,   /* x   y   z   {    |   }   ~   DEL */

		1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,
		1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,
		1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,
		1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,

		1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,
		1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,
		1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,
		1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,
	};
	string strResult;
	for (size_t i=0;i<strUrl.size(); i++ ) {
		unsigned char c = strUrl[i];
		if(urlchr_table[c]==1){	
			strResult += '%';		
			strResult += Dec2HexChar(c >> 4);
			strResult += Dec2HexChar(c & 0xf) ;
		}
		else{
			strResult+=c;
		}
	}
	return strResult;
}

/**
 * url decoder borrowed from php
 * @see url.c in php source package
 */
static int url_htoi(const char* s)
{
  int c = ((const unsigned char *)s)[0];
  if (isupper(c))
      c = tolower(c);
  int value = (c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10) * 16;

  c = ((const unsigned char *)s)[1];
  if (isupper(c))
      c = tolower(c);
  value += c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10;

  return value;
}

string DecodeUrl(const std::string &strUrl) 
{
	const char* src = strUrl.c_str();
	int len = strUrl.size();
	string result;
	result.reserve(len);

	for (int i = 0; i < len; ++i)
	{
	  if (src[i] == '+')
	  {
          result.push_back(' ');
	  }
	  else if (src[i] == '%' && i + 2 < len && isxdigit(src[i + 1]) && isxdigit(src[i + 2]))
	  {
	    result.push_back(url_htoi(src + i + 1));
	    i += 2;
	  }
	  else
	  {
	    result.push_back(src[i]);
	  }
	}
	return result;
}

string EncodePrefixStr(const string& input, const string& key)
{
	if(input.length() != key.length())
		return "";
	int size = input.length();
	char *buf = new char[size + 1];
	for(int i=0; i<size; i++){
		short int a = HexChar2Dec(input[i]);
		short int b = HexChar2Dec(key[i]);
		short int c = a ^ b;
		buf[i] = Dec2HexChar(c);
	}
	buf[size] = 0;
	string s;
	s.append(buf);
	delete[] buf;
	return s;
}

uint32_t GetIP(const string &interface)
{
	static uint32_t ip = 0;
	if(ip != 0)
		return ip;
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock == -1)
		return 0;
	struct sockaddr_in sin;
	struct ifreq ifr;
	strncpy(ifr.ifr_name, interface.c_str(), IFNAMSIZ);
	ifr.ifr_name[IFNAMSIZ - 1] = 0;
	if(ioctl(sock, SIOCGIFADDR, &ifr) < 0)
		return 0;
	memcpy(&sin, &ifr.ifr_addr, sizeof(sin));
	ip = ntohl(sin.sin_addr.s_addr);
	return ip;
}

string GetTimeStr(time_t t)
{
  if (t == 0)
	  time(&t);
	struct tm *p = localtime(&t);
	char buf[20];
	snprintf(buf, sizeof(buf), "%04d%02d%02d/%02d%02d", (p->tm_year + 1900), (1 + p->tm_mon), p->tm_mday, p->tm_hour, p->tm_min / 5 * 5);
	return string(buf);
}

string GetUniqueStr()
{
	static const uint32_t UINT32_MAX = (uint32_t)(-1);
	static uint32_t count = 0;
  uint32_t ip = base::Network::local_iip();
	//uint32_t ip = GetIP("eth0");
	if(0 == ip)
  	return "";
	int id = getpid();
	if(id <= 0)
		return "";
	count++;
	id = id & 0x0000FFFF;
	ip = ip & 0x0000FFFF;
	uint64_t result = id;
	result = result << 48;
	uint64_t lcount = count;
	lcount = lcount << 16;
	result += lcount;
	result += ip;
	string s;
	ostringstream oss;
	oss << std::setfill('0') << std::setw(16) << std::hex << result;
	if(UINT32_MAX == count)
		count = 0;
	return oss.str();
}


string GetRandomStr(const int& size)
{
	char *buf = new char[size + 1];
	for(int i=0; i<size; ++i)
	{
		int n = GetRandomNumber(0, 62);
		char c;
		if(n < 10)
			c = char(short('0') + n);
		else if(n < 36)
			c = char(short('A') + n - 10);
		else if(n < 62)
			c = char(short('a') + n - 36);
		else
			c = char(0);
		buf[i] = c;
	}
	buf[size] = 0;
	string s;
	s.append(buf);
	delete[] buf;
	return s;
}

string GetRandomNumber(const int& size)
{
	static const int MAX_SIZE = 4;
	if(size <= 0)
		return NULL;
	if(size > MAX_SIZE)
	{
		int index = 0;
		string result;
		while(index < size)
		{
			int length = (size - index) > MAX_SIZE ? MAX_SIZE : (size - index);
			string s = GetRandomStr(length);
			result += s;
			index += MAX_SIZE;
		}
		return result;

	}else{
		int from = 1;
		for(int i=0; i<(size-1); i++)
			from = from * 10;
		int end = from*10 - 1;
		int result = GetRandomNumber(from, end);
		return ToString(result);
	}
}

int GetRandomNumber(const int& from, const int& to)
{
	static int isInitialized = false;
	if(!isInitialized)
	{
		srand(int(time(NULL)));
		isInitialized = true;
	}
	return from + rand()%(to - from);
}	

/****************时间日期函数******************/
string TmToStr(const tm& tmTime)
{
	char buf[20];
	snprintf(buf,sizeof(buf),"%04d-%02d-%02d %02d:%02d:%02d",tmTime.tm_year+1900,tmTime.tm_mon+1,tmTime.tm_mday,
		tmTime.tm_hour,tmTime.tm_min,tmTime.tm_sec);
	return buf;
}

bool StrToTm(const string& strTime, tm& tmTime)
{
	sscanf(strTime.c_str(),"%d-%d-%d %d:%d:%d",&tmTime.tm_year, &tmTime.tm_mon, &tmTime.tm_mday ,
		&tmTime.tm_hour , &tmTime.tm_min, &tmTime.tm_sec );		
	if(!(tmTime.tm_year>=0&&tmTime.tm_year<=3000) || !(tmTime.tm_mon>=1&&tmTime.tm_mon<=12) || !(tmTime.tm_mday>=1&&tmTime.tm_mday<=31) || !(tmTime.tm_hour>=0&&tmTime.tm_hour<=59) || !(tmTime.tm_min>=0&&tmTime.tm_min<=59) || !(tmTime.tm_sec>=0&&tmTime.tm_sec<=59))
		return false;
	tmTime.tm_year -= 1900;
	tmTime.tm_mon -=  1;
	return true;
}


void TimeToTm(const time_t &tTime,tm &tmTime)
{
#ifdef WIN32
	tmTime=*localtime(&tTime);
#else
	localtime_r(&tTime,&tmTime);
	bool bGMTZone=false;
	#ifdef  __USE_BSD
	if(strcmp(tmTime.tm_zone,"GMT")==0)
		bGMTZone=true;		
	#else
	if(strcmp(tmTime.__tm_zone,"GMT")==0)
		bGMTZone=true;		
	#endif
	//if(bGMTZone)
	  //  AppendFile("./SysAlarm.log","系统严重错误:时区被改成GMT!\n");
#endif		
}

time_t TmToTime(tm &tmTime)
{
	time_t tTime=mktime(&tmTime);
	return tTime;
}

string TimeToStr(const time_t &tTime)
{
	struct tm tmTime;
	TimeToTm(tTime,tmTime);		
	return TmToStr(tmTime);
}

time_t StrToTime(const string &strTime)
{
	tm tmTime;
	if(StrToTm(strTime,tmTime))
		return TmToTime(tmTime);
	else
		return 0;
}
uint64_t GetMilliSecond()
{
	timeval tv;
	gettimeofday(&tv,0);
	return ((uint64_t)(tv.tv_sec))*1000+tv.tv_usec/1000;
}

int CompareDate(time_t tTime1st, time_t tTime2nd)
{
	struct tm tmFirst = *localtime(&tTime1st);
	tmFirst.tm_hour = 0;
	tmFirst.tm_min = 0;
	tmFirst.tm_sec = 0;

	struct tm tmSecond = *localtime(&tTime2nd);
	tmSecond.tm_hour = 0;
	tmSecond.tm_min = 0;
	tmSecond.tm_sec = 0;

	int diff = mktime(&tmFirst) - mktime(&tmSecond);
	return diff/86400;
}

/*bool Log(const string& strLog)
{
	string strNow = TimeToStr(time(0));
	string strLogFile="./log/upload.log."+strNow.substr(0,10);
	return AppendFile(strLogFile, "["+strNow.substr(11,8)+"] - " + strLog +"\r\n");
}*/

bool Log(const char* fmt, ...)
{
   va_list args;
   va_start(args,fmt);
   static char buf[8096];	//单进程,所以可以这么做
   int rev = vsnprintf(buf,sizeof(buf),fmt,args);
   va_end(args);
 
   string strNow = TimeToStr(time(0));
   string strLogFile="./log/new.log."+strNow.substr(0,10);
   string pid = ToString<int>(getpid());
   return AppendFile(strLogFile, "["+strNow.substr(11,8)+"] - " + buf + " " + pid +"\r\n");
}
string GetHeader(const string &strHeaders,const string&strHeaderName)
{	
	string strHeaderValue;
	vector<string> vecHeaderValue;
	GetHeader(strHeaders, strHeaderName, vecHeaderValue, false);
	if(!vecHeaderValue.empty())
		strHeaderValue = vecHeaderValue[0];
	return strHeaderValue;
}

void GetHeader(const string &strHeaders, const string&strHeaderName, vector<string>& vecHeaderValue, bool GetAll)
{
	vecHeaderValue.clear();
	string strHeaders2 = ToLower(strHeaders);//统一成小写进行查找 dongfei 20070708
	string strHeaderName2 = ToLower(strHeaderName)+": ";
	size_t nBeginPos=string::npos,pos=0;
	while ((nBeginPos=strHeaders2.find(strHeaderName2.c_str(),pos))!=string::npos)
	{
		nBeginPos += strHeaderName2.size();
		size_t nEndPos = strHeaders2.find("\r\n",nBeginPos);
		if(nEndPos==string::npos)
			continue;
		//获取header某属性值，如不获取所有的，则停止获取		
		string strHeaderValue = strHeaders.substr(nBeginPos,nEndPos-nBeginPos);			
		vecHeaderValue.push_back(strHeaderValue);
		pos = nEndPos;
		if(!GetAll)
			break;
	}
}
string GetHttpPath(const string&strUrl)
{
	string strPath;
	size_t nBeginPos = strUrl.find("/",7);
	if(nBeginPos!=string::npos){
		size_t nEndPos = strUrl.find("?" , nBeginPos);
		strPath = strUrl.substr(nBeginPos, nEndPos-nBeginPos);
	}
	return strPath;
}

string GetFileName(const string& strFileName)
{
	string s = DecodeUrl(strFileName);
	size_t index = s.find_last_of("\\");
	string result = s.substr(index+1, s.length()-index-1);
	//Log("FileName is:%s %d %s", s.c_str(), index, result.c_str());
	return result;
}

string GBKToUTF8(const string& src)
{
	size_t nInputLen = src.length();
	char* input = new char[nInputLen];
	src.copy(input, nInputLen);
	size_t nOutputLen = nInputLen*2;
	char *output = new char[nOutputLen];
	//因为iconv函数会改变传进去的pInut,pOutPut,使其指向修改后的位置，所以必须复制一份传进去
	char *pInput = input;
	char *pOutput = output;

	iconv_t cd = iconv_open("UTF-8", "GBK");
	if(cd == (iconv_t)-1)
		return src;

	if(iconv(cd, &pInput, &nInputLen, &pOutput, &nOutputLen) == -1)
		return src;

	iconv_close(cd);
	int nConverted = src.length()*2 - nOutputLen;
	string result(output, nConverted);
	delete[] input;
	delete[] output;
	return result;
}

string StrMd5(const string& src)
{
	const unsigned char *s = (const unsigned char*)src.c_str();
	unsigned char *md5 = MD5(s, src.length(), NULL);
	return Str2Hex(md5, MD5_DIGEST_LENGTH);
}

static bool IsStringUTF8(const char* str, unsigned length) {
  int i;
  unsigned nBytes = 0;  //UFT8可用1-6个字节编码,ASCII用一个字节
  unsigned char chr;
  bool bAllAscii = true;  //如果全部都是ASCII, 说明不是UTF-8
  for (i = 0; i < length; i++) {
    chr = *(str + i);
    if ((chr & 0x80) != 0)  // 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,但用一个字节存,最高位标记为0,o0xxxxxxx
      bAllAscii = false;
    if (nBytes == 0)  //如果不是ASCII码,应该是多字节符,计算字节数
        {
      if (chr >= 0x80) {
        if (chr >= 0xFC && chr <= 0xFD)
          nBytes = 6;
        else if (chr >= 0xF8)
          nBytes = 5;
        else if (chr >= 0xF0)
          nBytes = 4;
        else if (chr >= 0xE0)
          nBytes = 3;
        else if (chr >= 0xC0)
          nBytes = 2;
        else {
          return false;
        }
        nBytes--;
      }
    } else  //多字节符的非首字节,应为 10xxxxxx
    {
      if ((chr & 0xC0) != 0x80) {
        return false;
      }
      nBytes--;
    }
  }
  if (nBytes > 0)  //违返规则
      {
    return false;
  }
  if (bAllAscii)  //如果全部都是ASCII, 说明不是UTF-8
  {
    return false;
  }
  return true;
}

extern bool IsExifValueValid(const char* str, unsigned len) {
  // check if it is ascii
  bool allAscii = true;
  for (int i = 0; i < len; ++i) {
    if (!isascii(str[i])) {
      allAscii = false;
      break;
    }
  }
  if (allAscii) return true;
  return IsStringUTF8(str, len);
}
/*#include <iostream>
using namespace std;
int main(int argc, char **argv)
{
	Mkdir(argv[1]);
	cout<<Trim("\"\"","\" ")<<endl;
	cout<<atoi(" 9lvwei")<<endl;
	cout<<atoi(" lvwei")<<endl;
	cout<<atoi(" ")<<endl;
	cout<<atoi("")<<endl;
	cout<<FromString<int>(" 9lvwei")<<endl;
	cout<<FromString<int>(" lvwei")<<endl;
	cout<<FromString<int>(" ")<<endl;
	cout<<FromString<int>("")<<endl;
	SaveFile("/bigpool/fmn037/20090723/1225/pfef_large_LGWo_79ad000000022c3f.jpg","lvweiorie");
	return 0;
}*/
