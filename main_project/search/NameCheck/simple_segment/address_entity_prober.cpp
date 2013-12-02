#include "address_entity_prober.h"
#include "address_state.h"
#include "utility.h"
#include "segment_const.h"
#include "segment_utility.h"
#include "qata_config.h"
#include "config_const.h"
#include <string>
#include <vector>
#include <fstream>

using namespace std;
using namespace segment;

static const size_t  MAX_PROBE_NUM = 10;

AddressEntityProber::AddressEntityProber()
{}

AddressEntityProber::~AddressEntityProber()
{
	if( !_ptr_state_tb ) delete _ptr_state_tb;
	_ptr_state_tb = NULL;
}

bool AddressEntityProber::init( const char *conf_file )
{
	QataConfig config;
	if( !config.loadConfig( conf_file ) ) return false;

	_ptr_state_tb = new AddressTB();
	if( !_ptr_state_tb ) return false;

	if( !config.isKeyExists( ADDRESS_TRANSFER_FILE_NAME ) ) return false;
	if( !_ptr_state_tb->initAddressTB( config[ ADDRESS_TRANSFER_FILE_NAME ].c_str() ) )
		return false;

	return true;
}

bool AddressEntityProber::probeAddressEntity( wordAry& ary,size_t& idx,wordAry& ret )
{
	if( ary.empty() || idx >= ary.size() ) return false;

	_ptr_state_tb->initState();
	
	size_t ck_idx = idx;
	size_t end_num = min( ary.size() - idx , MAX_PROBE_NUM );
	int cur_state = _ptr_state_tb->getCurrentState();
	int transfer_idx = ck_idx;

	for( ; ck_idx < end_num ; ck_idx++ )
	{
		if( ary[ ck_idx ].length < GBK_CHAR_SIZE ) continue;

		if( SegmentUtility::isNoneCJKChar( ary[ ck_idx ] ) ) return false;

		size_t s_offset = ary[ ck_idx ].length - GBK_CHAR_SIZE;
		char* wd = (char*)( ary[ ck_idx ].ptr_wd + s_offset );
		char ch = wd[ GBK_CHAR_SIZE ];
		wd[ GBK_CHAR_SIZE ] = 0;

		_ptr_state_tb->advanceOneStep( wd );
		
		 wd[ GBK_CHAR_SIZE ] = ch;

		if( _ptr_state_tb->isTerminalState() ) break;

		if( cur_state != _ptr_state_tb->getCurrentState() )
		{
			cur_state = _ptr_state_tb->getCurrentState();
			transfer_idx = ck_idx;		
		}
	}

	if( !_ptr_state_tb->isTerminalState() ) return false;
	
	size_t add_idx  = transfer_idx + 1;
	//if( transfer_idx > 0 && ary[ transfer_idx ].length < BASE_WORD_SIZE )
	//	add_idx = transfer_idx - 1;

	size_t l_idx = idx;
	while( l_idx < add_idx ) ret.push_back( ary[ l_idx++ ] );

	l_idx = add_idx; 
	size_t len = 0;
	while( l_idx <= ck_idx && l_idx < ary.size() ) len += ary[ l_idx++ ].length; 

	wordItem add_wd;
	add_wd.offset = ary[ add_idx ].offset;
	add_wd.length = len;
	add_wd.ptr_wd = ary[ add_idx ].ptr_wd;
	
	ret.push_back( add_wd );
	idx = ck_idx+1;
	//printf("MMMMMMMMM\n");
	return true;
}

