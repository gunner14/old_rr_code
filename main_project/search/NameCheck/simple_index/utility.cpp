#include <vector>
#include <fstream>
#include <string>
#include <string.h>
#include <algorithm>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <sys/stat.h>
#include <sys/mman.h>
#include <syslog.h>
#include "utility.h"

using namespace std;

const size_t NORMAL_BUFF_SIZE = 1024;

bool print_msg(const string msg)
{
#ifdef __DEBUG_
	//cout<<msg<<endl;
	return true;
#endif
	return false;
}

unsigned int splitTxt(const string& text, const string& delim, vector<string>& array)
{
	//assert(text.size()>0);
	array.clear();
	if( text.empty() )
		return 0;

	size_t start = 0;
	size_t found = 0;
	found = text.find(delim,start);
	while (found != string::npos)
	{
		array.push_back(text.substr(start, found - start));
		start = found + delim.size();
		found = text.find(delim, start);
	}
	if (start != text.size())
	{
		array.push_back(text.substr(start, text.size() - start + 1));
	}
	return array.size();
} 

unsigned int joinTxt(const vector<string>& array,string& result)
{
	if(array.size()==0)
	{
		return 0;
	}
	result = array[0];
	for(size_t i=1;i<array.size();i++)
		result=result+array[i];
	return result.size();
}

unsigned int joinTxt(const vector<string>& array,const string& connetor,string& result)
{
	if(array.size()==0)
	{
		return 0;
	}
	result = array[0];
	for(size_t i=1;i<array.size();i++)
		result = result + connetor + array[i];

	return result.size();
}

string int2s(long long a)
{
	char buf[64];
	sprintf(buf, "%d", a);
	return (string)buf;
}

string float2s(float a)
{
	char buf[64];
	sprintf(buf, "%f", a);
	return (string)buf;
}

bool is_number_str(const char *str )
{
	int str_size = strlen( str );
	int ix = 0;
	
	while( ix < str_size )
	{
		if( !isdigit( (unsigned char)str[ix] ) )
			return false;
		ix++;
	}

	return true;
}

bool is_number_str(const string& str )
{
	return is_number_str( str.c_str() );
}

bool is_alpha_str(const char *str )
{
	int str_size = strlen( str );
	int ix = 0;
	
	while( ix < str_size )
	{
		if( !isalpha( (unsigned char)str[ix] ) )
			return false;
		ix++;
	}

	return true;
}

bool is_alpha_str(const string& str )
{
	return is_alpha_str( str.c_str() );
}

bool is_alnum_str(const char *str )
{
	if( !str || strlen( str ) == 0 )
		return false;
	
	int str_size = strlen( str );
	int ix = 0;
	
	while( ix < str_size )
	{
		if( !isalnum( (unsigned char)str[ix] ) )
			return false;
		ix++;
	}

	return true;
}

bool is_alnum_str(const string& str )
{
	if( str.empty() )
		return false;
	
	return is_alnum_str( str.c_str() );
}

bool is_alnumsym_str(const char *str )
{
	if( !str || strlen( str ) == 0 )
		return false;
	
	int str_size = strlen( str );
	int ix = 0;
	
	while( ix < str_size )
	{
		if( iscntrl( (unsigned char)str[ix] ) )
			return false;
		
		ix++;
	}

	return true;
}

bool is_alnumsym_str(const string& str )
{
	if( str.empty() )
		return false;
	
	return is_alnumsym_str( str.c_str() );
}

bool is_float_str(const char *str )
{
	if( strlen( str ) < 1 )
		return false;

	size_t idx = 0;
	if( str[idx] == '+' || str[idx] == '-')
		idx++;
	
	bool is_get_dot = false;
	for( ; idx < strlen( str ); idx++ )
	{
		if( str[idx] == '.')
		{
			if( is_get_dot )
				return false;

			is_get_dot = true;
		}
		else if( !isdigit( (unsigned char)str[ idx ]) )
		{
			return false;
		}
		
	}

	return true;
}

bool is_float_str(const string& str )
{
	if( str.empty() )
		return false;
	
	return is_float_str( str.c_str() );
}

static char _tolower( char& ch )
{
	char nch = ch;
	if( ch >= 0x41 && ch <= 0x5a ) nch = ch + 32;

	return nch;
}

int lower_case_str(char* s )
{
	for(int i = 0 ; i < strlen( s ) ;i++ )
		s[i] = _tolower( s[i]);
	return 1;
}

int lower_case_str(string& s)
{
	for(int i = 0 ; i < s.size() ;i++ )
		s[i] = _tolower( s[i]);
	return 1;
}

int lower_case_the_word( vector<string>& word_ary )
{
	for( int i = 0 ; i < word_ary.size();i++)
	{
		lower_case_str( word_ary[i] );
	}
	return 1;
}

string trimStr(const string& srcStr,const string& trimStr)
{
	return trimLeftStr( trimRightStr( srcStr ,trimStr) ,trimStr );
}

string trimLeftStr(const string& srcStr,const string& trimStr)
{
	size_t idx = srcStr.find_first_not_of( trimStr );
	if( idx == string::npos )
		return srcStr;
	
	return srcStr.substr( idx );
}

string trimRightStr(const string& srcStr,const string& trimStr)
{
	size_t idx = srcStr.find_last_not_of( trimStr );
	if( idx == string::npos )
		return srcStr;
	
	return srcStr.substr( 0 , idx + 1 );
}

bool trimBuf(char *buf,char ch)
{
	if( !buf || strlen( buf ) == 0 || strlen( buf ) >= NORMAL_BUFF_SIZE*10 - 1 )
		return false;

	//char* tmp_buf = new char[ strlen(buf) ];
	char tmp_buf[  NORMAL_BUFF_SIZE*10 ];
	size_t idx = 0;
	size_t jdx = 0;

	while( idx < strlen( buf ) ) 
	{
		if( buf[idx] != ch ) 
			tmp_buf[ jdx++ ] = buf[ idx ];

		idx++;  
	}

	tmp_buf[ jdx ] = 0;
	strcpy( buf , tmp_buf );
	buf[ jdx ] = 0;
	//delete tmp_buf;
	return true;
}

bool replaceBuf(char *buf,char search_char,char replace_char)
{
	if( !buf || strlen( buf ) == 0 || strlen( buf ) >= NORMAL_BUFF_SIZE*10 - 1 )
		return false;

	//char* tmp_buf = new char[ strlen(buf) ];
	char tmp_buf[ NORMAL_BUFF_SIZE*10 ];
	size_t idx = 0;
	size_t jdx = 0;

	while( idx < strlen( buf ) ) 
	{
		if( buf[idx] != search_char ) 
			tmp_buf[ jdx++ ] = buf[ idx ];
		else
			tmp_buf[ jdx++ ] = replace_char;
		
		idx++;  
	}

	tmp_buf[ jdx ] = 0;
	strcpy( buf , tmp_buf );
	buf[ jdx ] = 0;
	//delete tmp_buf;
	return true;
}

string removeStr( const string& srcStr,char ch )
{
	string ret_str;
	for( size_t idx = 0 ; idx < srcStr.size();idx++)
	{
		if( ch == srcStr[idx] )
			continue;

		ret_str = ret_str + srcStr[idx];
	}

	return ret_str;
}

bool xmlToUrlContent(const char *txt,string& ret_url,string& ret_content)
{
	if( strlen(txt) < 1 )
		return false;

	string url_tag = "<url:";
	string content_tag = "<content:";
	string end_tag = ">";

	//1 get url:
	char* p_start_url = strstr(txt,url_tag.c_str());
	if( p_start_url == NULL )
		return false;

	p_start_url = strstr(p_start_url,":");
	char *p_end_url = strstr(p_start_url,end_tag.c_str());
	if( p_end_url == NULL )
		return false;

	size_t url_size = p_end_url - p_start_url - 1 ;
	string url(p_start_url+1, url_size);

	//2 get content:
	char* p_start_content = strstr(p_end_url,content_tag.c_str());
	if( p_start_content == NULL )
		return false;

	p_start_content = strstr(p_start_content,":");

	size_t content_size =  strlen( txt ) - (p_start_content - txt ) - 2;
	string content(p_start_content+1,content_size );

	ret_url = url;
	ret_content = content;

	return true;
}

bool xmlToUrlTitleContent(const char *txt,string& ret_url,string& ret_title,string& ret_content)
{
	//1. get url and content
	if( !xmlToUrlContent( txt , ret_url, ret_content))
		return false;

	//2. get title
	string title_tag = "title";
	if( !extractXmlTag( txt , title_tag,ret_title))
		return false;

	return true;
}

bool extractXmlTag(const char *txt,const string& tag,string& ret)
{
	//1. extract the content from given tag
	if( txt == NULL || tag.empty() )
		return false;

	string the_tag = "<"+tag+":";
	string end_tag = ">";

	char* p_start_tag = strstr(txt,the_tag.c_str());
	if( p_start_tag == NULL )
		return false;

	p_start_tag = strstr(p_start_tag,":");
	char *p_end_tag = strstr(p_start_tag,end_tag.c_str());
	if( p_end_tag == NULL )
		return false;

	size_t tag_size = p_end_tag - p_start_tag - 1 ;
	string tag_content(p_start_tag+1, tag_size);

	ret = tag_content;

	return true;
}

int getFileSize(const char *filename )
{
	int fdin;
	struct stat statbuf;

	if( (fdin = open( filename, O_RDONLY )) < 0 )
	{
		return -1;
	}   

	if( fstat( fdin , &statbuf) < 0 )
	{
		return -1;
	}   

	int size = statbuf.st_size ;

	close( fdin );

	return size;
}


void* MMapFile(const char *filename,int file_open_flag ,int mem_prot_flag,int mem_share_flag)
{
	int fdin;
	struct stat statbuf;

	if( (fdin = open( filename,file_open_flag )) < 0 )
	{
		print_msg("MMapFile open file fail!");
		return NULL;
	}   

	if( fstat( fdin , &statbuf) < 0 )
	{
		print_msg("MMapFile get file state fail!");
		return NULL;
	}   

	//printf("[%s] file size = %d\n",filename,statbuf.st_size);
	void *ptr_mem = mmap(NULL,statbuf.st_size,mem_prot_flag ,mem_share_flag,fdin,0); 
	if( ptr_mem == MAP_FAILED )
	{
		print_msg("MMapFile map file to mem!");
		return NULL;
	}   

	return ptr_mem;
}

bool UnMMapFile(const char *filename ,void *ptr_mem)
{
	int fdin;
	struct stat statbuf;

	if( (fdin = open( filename ,O_RDONLY )) < 0 )
	{
		print_msg("UnMMapFile open file to mem!");
		return false;
	}

	if( fstat( fdin , &statbuf) < 0 )
	{   
		print_msg("UnMMapFile get file state fail!");
		return false;
	}

	int ret = munmap(ptr_mem,statbuf.st_size );
	if( ret == 1 )
	{
		print_msg("UnMMapFile un-mem fail!");
		return false;
	}

	return true;
}

int convertToBanjiao(uint16_t* ptr_wbuf, int len )
{
	int i;
	uint16_t    ch;
	for( i = 0; i < len; i ++ )
	{
		ch = ptr_wbuf[i];
		if( ( ch >> 8 ) == 0xff )
		{
			ch &= 0xff;
			ch += 0x20;
		}
		if( ch >= 'A' && ch <= 'Z' )
		{
			ch += 0x20;
		}
		ptr_wbuf[i] = ch; 
	}
	return 1;
}
int getFileContent(const string& file,string& contentStr)
{
	/*
	ifstream infile( file.c_str() );
	if( !infile.is_open())
		return -1;

	filebuf *pbuf;
	pbuf = infile.rdbuf();
	long size=pbuf->pubseekoff (0,ios::end,ios::in);
	pbuf->pubseekpos (0,ios::in);

	// allocate memory to contain file data
	char *buffer=new char[size+1];
	pbuf->sgetn (buffer,size);

	contentStr = buffer;
	delete[] buffer;
	infile.close();
	*/

	FILE *pfile = NULL; 
	pfile = fopen( file.c_str() , "r");
	if( !pfile  )
		return -1;

	char buf[ NORMAL_BUFF_SIZE ];
	while( fgets( buf,NORMAL_BUFF_SIZE,pfile ) )
	{
		if( strlen(buf) == 0 )
			continue;
		 
		buf[ strlen(buf) - 1 ] = '\n';
		string line = buf;
		contentStr += line;
	}

	fclose( pfile );
	return 1;
}

int getRandomNumber( )
{
        srand ( unsigned ( time (NULL) ) );
        return rand();
}

int getRandomNumber(int start_range,int end_range)
{
        if( start_range >= end_range )
                return 0;

        int rand_idx = ( getRandomNumber() % ( end_range - start_range)) + start_range ;

        return rand_idx;
}

void Logger::logDebug(const string& ident,const string& msg)
{
	openlog(ident.c_str(),LOG_PID | LOG_PERROR ,LOG_USER);
	_logMsg(LOG_DEBUG,msg);
}

void Logger::logError(const string& ident,const string& msg)
{
	openlog(ident.c_str(), LOG_PID|LOG_PERROR , LOG_USER);
	_logMsg(LOG_ERR,msg);
}

void Logger::_logMsg(int priority,const string& msg)
{
	syslog(priority,msg.c_str());
}

