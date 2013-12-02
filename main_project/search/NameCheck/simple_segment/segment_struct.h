#ifndef __N_SEGMENT_STRUCT_H__
#define __N_SEGMENT_STRUCT_H__

#include <string>
#include <vector>

using namespace std;

namespace segment
{
	static const size_t SMALL_BUF_SIZE = 32;
 
	class  wordItem
	{       
		public: 
			wordItem():length(0),offset(0),ptr_wd(NULL){}
			bool operator==( const wordItem& r_wd )
			{       
				return ( offset == r_wd.offset && length == r_wd.length );
			}       

		public: 
			size_t offset; 
			unsigned short length; 
			const char *ptr_wd;
	};      

	typedef vector<string> strAry; 
	typedef vector<wordItem> wordAry;
	
	static void __printWord( const string& info ,wordItem& wd )
	{
		if( wd.length >= SMALL_BUF_SIZE ) return;

		char buf[ SMALL_BUF_SIZE ];
		memcpy( buf , wd.ptr_wd ,  wd.length );
		buf[ wd.length ] = 0;
		printf("%s: offset = %d , len = %d , wd = %s\n",info.c_str(),wd.offset, wd.length , buf );
	}

}

#endif
