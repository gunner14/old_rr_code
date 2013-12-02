#include "segment_utility.h"
#include "segment_const.h"
#include "word_const.h"
#include "utility.h"
#include <string>
#include <algorithm>

using namespace std;
using namespace segment;


bool SegmentUtility::getWordSizeFromRange( wordAry& ary , size_t start_idx , size_t end_idx ,size_t& word_count,size_t& total_len )
{
	while( start_idx < end_idx )
	{
		wordItem wd = ary[ start_idx++];
		total_len += wd.length;
		if( wd.length >= BASE_WORD_SIZE ) word_count++;
	}
	
	return true;
}

bool SegmentUtility::addRangeWordToArray( wordAry& ary , size_t start_idx , size_t end_idx ,wordAry& result)
{
	if( start_idx >= end_idx ||  end_idx  > ary.size() ) return false;

	while( start_idx < end_idx ) result.push_back( ary[ start_idx++] );
	
	return true;
}

static bool __isalnum( unsigned char ch )
{
	if( ch >= 0x30 && ch <= 0x39 ) return true;

	if( ch >= 0x41 && ch <= 0x5a ) return true;

	if( ch >= 0x61 && ch <= 0x7a ) return true;

	return false;
}

static bool __isprint( unsigned char ch )
{
	if( ch >= 0x20 && ch <= 0x7e ) return true;

	return false;
}

bool SegmentUtility::isSingleChar( char ch )
{
	unsigned char uch = (unsigned char) ch;
	if( __isalnum( uch ) || __isprint( uch ) ) return true;

	return false;
}

/*
   bool SegmentUtility::isSingleChar( char ch )
   {
   if( isalnum( ch ) || isprint( ch ) ) return true;

   return false;
   }
   */

bool SegmentUtility::addOneWord(const char* str,size_t offset,size_t length,wordAry& result )
{
	wordItem wd_item;
	wd_item.length = length;
	wd_item.offset = offset;
	wd_item.ptr_wd = str + offset;

	result.push_back( wd_item );
#ifdef __DEBUG_
	__printWord("SegmentUtility::addOneWord get One Seg",wd_item );
#endif
	return true;
}

string SegmentUtility::getWordStr( wordItem& wd )
{
	char str_wd[ _MAX_WORD_LENGTH ];
	memcpy( str_wd , wd.ptr_wd , wd.length );
	str_wd[ wd.length ] = 0;

	return str_wd;
}

bool SegmentUtility::isNumberChar( wordItem& wd )
{
	char *p = (char*)wd.ptr_wd;
	size_t len = wd.length;
	char ch = p[ len ];
	p[ len ] = 0;
	bool result = ( is_number_str( p ) || NumeralDict.find( p ) != NumeralDict.end() );
#ifdef __DEBUG_
	printf("SegmentUtility::isNumberChar wd[%s] test ret:  %d\n", p ,result);
#endif
	p[ len ] = ch;

	return result;
}

bool SegmentUtility::isQuanChar( wordItem& wd )
{
	char *p = (char*)wd.ptr_wd;
	size_t len = wd.length;
	char ch = p[ len ];
	p[ len ] = 0;
	bool result = QuantityDict.find( p ) != QuantityDict.end()?true:false;
#ifdef __DEBUG_
	printf("SegmentUtility::isQuanChar test wd[%s] ret:  %d\n",p,result);
#endif
	p[ len ] = ch;

	return result;
}

bool SegmentUtility::isNumQuanChar( wordItem& wd )
{
	char *p = (char*)wd.ptr_wd;
	size_t len = wd.length;
	char ch = p[ len ];
	p[ len ] = 0;
	bool result = ( is_number_str( p ) || QuantityDict.find( p ) != QuantityDict.end()  || NumeralDict.find( p ) != NumeralDict.end() );
#ifdef __DEBUG_
	printf("SegmentUtility::isNumQuanChar test wd[%s] ret:  %d\n",p,result);
#endif
	p[ len ] = ch;

	return result;
}

bool SegmentUtility::isNoneCJKChar( wordItem& wd )
{
	if(  wd.length != GBK_CHAR_SIZE ) return false;

	char low_ch = wd.ptr_wd[0];
	char hig_ch = wd.ptr_wd[1];

	//if( low_ch < (char) 0xb0 && hig_ch < (char) 0xa1 ) return true;
	if( low_ch < (char) 0xb0 ) return true;

	return false;
}


bool SegmentUtility::isTerminalChar( const char *str, size_t idx,size_t len )
{
	if(  idx >= len - 1 ) return false;

	char low_ch = str[ idx ];
	char hig_ch = str[ idx + 1 ];

	//¡£
	if( ( low_ch == (char)0xa1 )  &&  ( hig_ch == (char)0xa3 ) )return true;

	if( low_ch == (char)0xa3 )
	{
		//£¬
		if(  hig_ch == (char)0xac  )return true;
		//£¡
		if( hig_ch == (char)0xa1  ) return true;
		//£¿
		if( hig_ch == (char)0xbf )  return true;
		//£º
		if( hig_ch == (char)0xb1  ) return true;
		//£»
		if( hig_ch == (char)0xbb  ) return true;
	}

	return false;
}

bool SegmentUtility::isSeperatedChar( const char *str, size_t idx,size_t len )
{
	if(  idx >= len - 1 ) return false;

	char low_ch = str[ idx ];
	char hig_ch = str[ idx + 1 ];

	if( low_ch == (char) 0xa1 )
	{
		//¡·
		if(  hig_ch == (char) 0xb7 ) return true;
		//¡¶
		if(  hig_ch == (char) 0xb6 ) return true;
		//¡°
		if(  hig_ch == (char) 0xb0 ) return true;
		//¡±
		if(  hig_ch == (char) 0xb1 ) return true;
		//¡¾
		if(  hig_ch == (char) 0xbe ) return true;
		//¡¿
		if(  hig_ch == (char) 0xbf ) return true;
	}

	if( low_ch == (char) 0xa3 )
	{
		//£¬
		if(  hig_ch == (char)0xac  ) return true;
		//£¨
		if(  hig_ch == (char)0xa8  ) return true;
		//£©
		if(  hig_ch == (char)0xa9  ) return true;
	}

	return false;
}
