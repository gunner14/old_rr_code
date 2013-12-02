#ifndef __N_SEGMENT_H__
#define __N_SEGMENT_H__

#include <vector>
#include <string>
#include "segment_struct.h"

using namespace std;

namespace segment
{
	class DoubleHashDict;
	class NameEntityProber;
	class AddressEntityProber;

	class Segment
	{       
		public: 
			Segment( DoubleHashDict *p_fwd_dict , DoubleHashDict *p_bwd_dict,NameEntityProber *p_name_prober = NULL , AddressEntityProber *address_entity_prober = NULL , bool use_bidirectional = false );
			bool segmentWords( const char *str , size_t len ,wordAry& result );
			bool segmentTxt( const char *str , size_t len ,wordAry& result );
		private:
			size_t _getOneSentence( const char *str , size_t start_idx , size_t max_len );
			size_t _forwardMaxSegment( const char *str,size_t start_idx,size_t len ,size_t& wd_count,size_t& wd_total_len,wordAry& result);
			size_t _backwardMaxSegment( const char *str,size_t start_idx,size_t len ,size_t& wd_count,size_t& wd_total_len,wordAry& result);
			size_t _mergeResult( wordAry& fwd_result,size_t fwd_wd_count,size_t fwd_wd_len,wordAry& bwd_result,size_t bwd_wd_count,size_t bwd_wd_len,wordAry& result);
			bool _swapEmptyResult( wordAry& fwd_result,wordAry& bwd_result,wordAry& result );
			size_t _segmentOneSentence( const char *str,size_t start_idx,size_t len ,wordAry& result);
			size_t _pickSpecialWord( wordAry& raw_result ,wordAry& result );
			bool _pickQuanTimeWord( wordAry& ary ,size_t& start_idx ,wordAry& result);
			bool _pickNameEntityWord( wordAry& ary ,size_t& start_idx ,wordAry& result);
			bool _pickAddressEntityWord( wordAry& ary ,size_t& start_idx ,wordAry& result);
			bool _pickNoneCJKWord( wordAry& ary ,size_t& start_idx ,wordAry& result);
			bool _forwardAddAlphaWord(const char *str,size_t start_idx,size_t sent_end_idx ,wordAry& result,size_t& next_idx);
			bool _backwardAddAlphaWord(const char *str,size_t start_idx,size_t sent_end_idx,wordAry& result ,size_t& next_idx);
		private:
			DoubleHashDict *_ptr_fwd_dict;
			DoubleHashDict *_ptr_bwd_dict;
			NameEntityProber *_ptr_name_prober;
			AddressEntityProber *_ptr_address_prober;
			bool _use_bidirectional;
	};      	
}

#endif
