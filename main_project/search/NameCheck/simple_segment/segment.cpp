#include "segment.h"
#include "name_entity_prober.h"
#include "address_entity_prober.h"
#include "segment_const.h"
#include "segment_utility.h"
#include "word_const.h"
#include "double_hash_dict.h"
#include <string>
#include <algorithm>

using namespace std;
using namespace segment;

Segment::Segment( DoubleHashDict *p_fwd_dict , DoubleHashDict *p_bwd_dict , NameEntityProber *p_name_prober , AddressEntityProber *p_address_prober, bool use_bidirectional ):_ptr_fwd_dict( p_fwd_dict ),_ptr_bwd_dict( p_bwd_dict ),_ptr_name_prober( p_name_prober),_ptr_address_prober( p_address_prober),_use_bidirectional( use_bidirectional )
{}

bool Segment::segmentTxt( const char *str , size_t length ,wordAry& result )
{
	if( !str || length == 0 ) return false;

	_segmentOneSentence( str , 0 , length , result );
#ifdef __DEBUG_
	printf("Segment::segmentTxt get [%d] words start at [%d]  len = %d \n", result.size(), 0 , length);
#endif

	return result.empty()?false:true;
}

bool Segment::segmentWords( const char *str , size_t length ,wordAry& result )
{
	if( !str || length == 0 ) return false;
	
	size_t senten_start_idx = 0;
	size_t senten_end_idx = 0;
	size_t len = 0;
	size_t sent_count = 0;
	while( len < length )
	{
		senten_end_idx = _getOneSentence( str , senten_start_idx , length );
		int senten_len = senten_end_idx - senten_start_idx;
		if( senten_end_idx == 0 || senten_len <= 0 ) break;

		wordAry senten_wds;
		_segmentOneSentence( str ,senten_start_idx , senten_len , senten_wds );
		result.insert( result.end() , senten_wds.begin() , senten_wds.end() );
#ifdef __DEBUG_
		printf("Segment::segmentWords get [%d] sentence start at [%d]  len = %d seg_size = %d\n",sent_count++,senten_start_idx,senten_len,senten_wds.size());
#endif

		senten_start_idx = senten_end_idx;
		len += senten_len; 
	}
	
	return result.empty()?false:true;
}

size_t Segment::_getOneSentence( const char *str , size_t start_idx, size_t max_len )
{
	size_t idx = start_idx;
	while( idx < max_len )
	{
		if( SegmentUtility::isSingleChar( str[idx] ) )
		{
			idx++;
			continue;
		}

		if( SegmentUtility::isTerminalChar( str , idx , max_len ) ) 
		{
#ifdef __DEBUG_
			printf("Segment::_getOneSentence get terminal char at[ %d - %d ]\n",start_idx ,idx + GBK_CHAR_SIZE );
#endif
			return idx + GBK_CHAR_SIZE;
		}

		idx += GBK_CHAR_SIZE;
	}

	return max_len;
}

size_t Segment::_segmentOneSentence( const char *str,size_t start_idx ,size_t len ,wordAry& result)
{
#ifdef __DEBUG_
printf("Segment::_segmentOneSentence start: start_idx = %d ,len= %d\n",start_idx , len );
#endif
	wordAry fwd_result;
	size_t fwd_wd_count = 0;
	size_t fwd_wd_len = 0;
	_forwardMaxSegment( str , start_idx ,len , fwd_wd_count , fwd_wd_len ,fwd_result );

	wordAry bwd_result;	
	size_t bwd_wd_count = 0;
	size_t bwd_wd_len = 0;
	
	if( _use_bidirectional )
		_backwardMaxSegment( str , start_idx, len , bwd_wd_count , bwd_wd_len ,bwd_result );

	wordAry raw_seg;
	_mergeResult( fwd_result , fwd_wd_count , fwd_wd_len ,bwd_result , bwd_wd_count , fwd_wd_len ,raw_seg );

	return _pickSpecialWord( raw_seg , result );
}

size_t Segment::_mergeResult( wordAry& fwd_result , size_t fwd_wd_count ,size_t fwd_wd_len ,wordAry& bwd_result , size_t bwd_wd_count , size_t bwd_wd_len ,wordAry& result )
{
	if( _swapEmptyResult( fwd_result , bwd_result , result ) ) 
		return result.size();
	
	float fwd_avg_wd_size = ( fwd_wd_count > 0 )?(  (float)fwd_wd_len/fwd_wd_count ):0;
	float bwd_avg_wd_size = ( bwd_wd_count > 0 )?(  (float)bwd_wd_len/bwd_wd_count ):0;
#ifdef __DEBUG_
printf("Segment::_mergeResult fwd_wd_count/fwd_ary_count[ avg_wd_size:%f ] = [ %d / %d ] , bwd_wd_count/bwd_ary_count[ avg_wd_size:%f ] = [ %d / %d ]\n", fwd_avg_wd_size ,fwd_wd_count ,fwd_result.size() , bwd_avg_wd_size ,bwd_wd_count,bwd_result.size() );
#endif
	
	if( fwd_result.size() == bwd_result.size() )
	{
		if( fwd_wd_count > bwd_wd_count )
			result.swap( fwd_result );
		else
			result.swap( bwd_result );
	}
	else if( fwd_avg_wd_size > bwd_avg_wd_size )
	{
			result.swap( fwd_result );
	}
	else
	{
			result.swap( bwd_result );
	}

	return result.size();
}

bool Segment::_swapEmptyResult( wordAry& fwd_result , wordAry& bwd_result , wordAry& result )
{
	if( !fwd_result.empty() && !bwd_result.empty() ) return false;

	if( fwd_result.empty() && !bwd_result.empty() )	
		result.swap( bwd_result );

	if( !fwd_result.empty() && bwd_result.empty() )	
		result.swap( fwd_result );

	return true;
}

size_t Segment::_forwardMaxSegment( const char *str,size_t start_idx,size_t len , size_t& wd_count,size_t& wd_total_len,wordAry& result)
{
#ifdef __DEBUG_
	printf("Segment::_forwardMaxSegment Start idx = %d ,len = %d, str: %s\n",start_idx,len, str);
#endif
	size_t sent_end_idx = start_idx + len;
	while( start_idx < sent_end_idx )
	{
		size_t next_idx = 0;
		if( _forwardAddAlphaWord( str, start_idx,sent_end_idx ,result,next_idx) )
		{
			start_idx = next_idx;
			continue;
		}

		int wd_end_idx = start_idx + GBK_CHAR_SIZE;
		size_t search_size = min( sent_end_idx  - start_idx , _MAX_WORD_LENGTH );

		size_t offset = 0;
		if( _ptr_fwd_dict->preMatchMaxWord( str + start_idx , search_size , offset ) )
		{
			wd_end_idx = start_idx + offset;
			wd_total_len += wd_end_idx - start_idx;
			wd_count++;
		}

		SegmentUtility::addOneWord( str , start_idx ,  wd_end_idx - start_idx , result );
		start_idx = wd_end_idx;
	}

	return result.empty()?false:true;
}

size_t Segment::_backwardMaxSegment( const char *str,size_t start_idx,size_t len ,size_t& wd_count,size_t& wd_total_len,wordAry& result)
{
#ifdef __DEBUG_
	printf("Segment::_backwardMaxSegment Start idx = %d ,len = %d\n",start_idx,len);
#endif

	int search_end_idx = start_idx + len - 1;
	while( search_end_idx >= (int)start_idx )
	{
		size_t next_idx = 0;
		if( _backwardAddAlphaWord( str,start_idx,search_end_idx,result ,next_idx))
		{
			search_end_idx = next_idx;
			continue;
		}

		size_t search_size = min( (size_t)search_end_idx + 1 , _MAX_WORD_LENGTH );
		int search_start_idx = search_end_idx - search_size + 1;

		size_t wd_len;
		if( !_ptr_bwd_dict->proMatchMaxWord( str + search_start_idx , search_size ,wd_len ))
			wd_len =  GBK_CHAR_SIZE;
		else
		{
			wd_total_len += wd_len;
			wd_count++;
		}

		SegmentUtility::addOneWord( str , search_end_idx - wd_len + 1 , wd_len ,result);
		search_end_idx -= wd_len;
	}

	if( result.empty() ) return false;

	reverse( result.begin() , result.end() );
	return true;
}

size_t Segment::_pickSpecialWord( wordAry& raw_result ,wordAry& result )
{
#ifdef __DEBUG_
	printf("Segment::_pickSpecialWord start with [ %d ] raw_segs\n",raw_result.size() );
#endif
	size_t idx = 0;
	size_t wd_size = raw_result.size();
	while( idx < wd_size )
	{

#ifdef __DEBUG_
	printf("Segment::_pickSpecialWord pick at [ %d ][ %p ]\n",idx , raw_result[idx].ptr_wd  );
	__printWord( "Segment::_pickSpecialWord wd",raw_result[idx] );
#endif
		if( _pickQuanTimeWord( raw_result , idx , result ) )
			continue;

		if( _pickNoneCJKWord( raw_result , idx , result ) )
			continue;

		if( _pickNameEntityWord( raw_result , idx , result ) )
			continue;

		if( _pickAddressEntityWord( raw_result , idx , result ) )
			continue;

		result.push_back( raw_result[ idx ] );
		idx++;
	}
	
	return result.size();
}

bool Segment::_pickNoneCJKWord( wordAry& ary ,size_t& start_idx ,wordAry& result)
{
	size_t idx = start_idx;
	size_t len = 0;
	while( idx < ary.size() && SegmentUtility::isNoneCJKChar( ary[idx]) ) 
	{
		len += ary[ idx ].length;
		idx++;
	}

	if( idx == start_idx ) return false;

	wordItem none_cjk_wd;
	none_cjk_wd.offset = ary[ start_idx ].offset;
	none_cjk_wd.length = len;
	none_cjk_wd.ptr_wd = ary[ start_idx ].ptr_wd;
	result.push_back( none_cjk_wd );
	
	start_idx = idx;
	return true;	
}

bool Segment::_pickQuanTimeWord( wordAry& ary ,size_t& start_idx ,wordAry& result)
{
	if( start_idx >= ary.size() - 1 ) return false;

	if( ( ary[ start_idx ].length >= BASE_WORD_SIZE  && !isdigit( ary[ start_idx ].ptr_wd[0] ) ) || ary[ start_idx + 1 ].length >= BASE_WORD_SIZE ) return false;
	
	size_t idx = start_idx;
	size_t len = 0;
	if( !SegmentUtility::isNumberChar( ary[idx] ) ) return false;

	len += ary[idx++ ].length;
	if( !SegmentUtility::isNumQuanChar( ary[idx] ) ) return false;

	len += ary[idx++].length;
	while( idx < ary.size() )
	{
		if( !SegmentUtility::isNumQuanChar( ary[idx] ) ) 
			break;
		len += ary[idx++].length;
	}	

	size_t ck_idx = start_idx;
	wordItem quan_wd;
	quan_wd.ptr_wd =  ary[ ck_idx ].ptr_wd;
	quan_wd.offset = ary[ ck_idx ].offset;

	quan_wd.length = len;
#ifdef __DEBUG_
	__printWord("Segment::_pickQuanTimeWord get QuanWord" , quan_wd );
#endif

	result.push_back( quan_wd );
	start_idx = idx;
	return true;
}

bool Segment::_pickNameEntityWord( wordAry& ary ,size_t& start_idx ,wordAry& result)
{
	if( !_ptr_name_prober ) return false;

	return _ptr_name_prober->probeNameEntity( ary , start_idx , result );
}

bool Segment::_pickAddressEntityWord( wordAry& ary ,size_t& start_idx ,wordAry& result)
{
	if( !_ptr_address_prober ) return false;

	return _ptr_address_prober->probeAddressEntity( ary , start_idx , result );
}

bool Segment::_forwardAddAlphaWord(const char *str,size_t start_idx,size_t sent_end_idx,wordAry& result,size_t& next_idx )
{
	size_t sin_char_idx = start_idx;
	while( sin_char_idx < sent_end_idx  && SegmentUtility::isSingleChar( str[ sin_char_idx ] ) ) 
	{
		if( !isspace( str[ sin_char_idx] ) )
		{
			sin_char_idx++;
			continue;
		}

		if( sin_char_idx > start_idx )			
			SegmentUtility::addOneWord( str , start_idx , sin_char_idx - start_idx , result );
		
		SegmentUtility::addOneWord( str ,sin_char_idx ,1 ,result );
		next_idx = sin_char_idx + 1;
		return true;
	}

	if( sin_char_idx > start_idx )
	{
		SegmentUtility::addOneWord( str , start_idx , sin_char_idx - start_idx , result );
		next_idx = sin_char_idx;
		return true;
	}
	
	return false;
}

bool Segment::_backwardAddAlphaWord(const char *str,size_t start_idx,size_t search_end_idx,wordAry& result,size_t& next_idx )
{
	int sin_char_idx = search_end_idx;
	while( sin_char_idx >= (int)start_idx && SegmentUtility::isSingleChar( str[ sin_char_idx ] ) )
	{       
		if( !isspace( str[ sin_char_idx] ) )
		{
			sin_char_idx--;
			continue;
		}

		if( sin_char_idx < search_end_idx )        
			SegmentUtility::addOneWord( str , sin_char_idx + 1 , search_end_idx - sin_char_idx  , result ); 
		
		SegmentUtility::addOneWord( str ,sin_char_idx ,1 ,result );
		next_idx = sin_char_idx - 1;
		return true;
	}            

	if( sin_char_idx < (int)search_end_idx )
	{       
		SegmentUtility::addOneWord( str , sin_char_idx+1  , search_end_idx - sin_char_idx , result );
		next_idx = sin_char_idx;
		return true;
	}      

	return false;
}
