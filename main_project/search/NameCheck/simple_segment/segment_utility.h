#ifndef __N_SEGMENT_UTILITY_H__
#define __N_SEGMENT_UTILITY_H__

#include "segment_struct.h"

using namespace std;

namespace segment
{
	class SegmentUtility
	{
		public:
		static bool isSingleChar( char ch ); 
		static bool isTerminalChar( const char *str, size_t idx,size_t len );
		static bool isSeperatedChar( const char *str, size_t idx,size_t len );
		static bool isNumberChar( wordItem& wd );
		static bool isQuanChar( wordItem& wd );
		static bool isNumQuanChar( wordItem& wd );
		static bool isNoneCJKChar( wordItem& wd );

		static string getWordStr( wordItem& wd );

		static bool addRangeWordToArray( wordAry& ary , size_t start_idx , size_t end_idx ,wordAry& result);
		static bool addOneWord(const char* str,size_t offset,size_t length,wordAry& result );
		static bool getWordSizeFromRange( wordAry& ary , size_t start_idx , size_t end_idx ,size_t& word_count,size_t& total_len);
	};
}

#endif
