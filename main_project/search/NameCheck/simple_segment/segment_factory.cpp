#include "segment_factory.h"
#include "segment.h"
#include "double_hash_dict.h"
#include "name_entity_prober.h"
#include "address_entity_prober.h"
#include "qata_config.h"
#include "config_const.h"

using namespace std;
using namespace segment;

static const string TRUE = "true";

size_t SegmentFactory::_ref_count = 0;
NameEntityProber *SegmentFactory::_p_name_prober = NULL;
AddressEntityProber *SegmentFactory::_p_address_prober = NULL;
Segment *SegmentFactory::_p_segtor = NULL;
DoubleHashDict *SegmentFactory::_p_fwd_dict = NULL;
DoubleHashDict *SegmentFactory::_p_bwd_dict = NULL;

void SegmentFactory::destoryInstance()
{
#ifdef __DEBUG_
printf("SegmentFactory::destoryInstance() start current ref_count = %d\n",_ref_count);
#endif
	if( _ref_count--  == 0 ) _releaseResource();
}

void SegmentFactory::_nullResource()
{
	_p_segtor = NULL;
	_p_fwd_dict = NULL;
	_p_bwd_dict = NULL;
	_p_name_prober = NULL;
	_p_address_prober = NULL;
}

void SegmentFactory::_releaseResource()
{
#ifdef __DEBUG_
printf("SegmentFactory::_releaseResource() free resources\n");
#endif
	if( _p_segtor )
		delete _p_segtor;

	if(  _p_fwd_dict )
		delete _p_fwd_dict;

	if(  _p_bwd_dict )
		delete _p_bwd_dict;

	if( _p_name_prober )
		delete _p_name_prober;

	if( _p_address_prober )
		delete _p_address_prober;

	_nullResource();
}

Segment*  SegmentFactory::getInstance( const char *conf_file  )
{
#ifdef __DEBUG_
printf("SegmentFactory::getInstance() start current ref_count = %d\n",_ref_count);
#endif
	if( _ref_count == 0  && !_init( conf_file ) ) return NULL;
	
	_ref_count++;
	return _p_segtor;
}

bool SegmentFactory::_init( const char *conf_file )
{
	if( !conf_file || strlen( conf_file ) == 0 )
	{
#if __DEBUG_
		printf("SegmentFactory::init error empty config file name\n");
#endif
		return false; 
	}

	QataConfig config;
	if( !config.loadConfig( conf_file ) )	
	{	
#if __DEBUG_
		printf("SegmentFactory::init config file name:[%s]\n",conf_file );
#endif
		return false;
	}
	
	try
	{
#if __DEBUG_
time_t start_time = clock();
#endif
		_p_fwd_dict = _loadDict( config[ FWD_PHRASE_FILE_NAME ] );
		if( !_p_fwd_dict )
			throw "SegmentFactory::init init _p_fwd_dict fail";

		_p_bwd_dict = _loadDict( config[ BWD_PHRASE_FILE_NAME ] );
		if( !_p_bwd_dict )
			throw "SegmentFactory::init init _p_bwd_dict fail";

		_p_name_prober = NULL;
		if( config.isKeyExists( ENABLE_NAME_ENTITY_RECOGNITION_NAME ) && config[ ENABLE_NAME_ENTITY_RECOGNITION_NAME ] == TRUE )
		{
			_p_name_prober = new NameEntityProber();
			if( !_p_name_prober || !_p_name_prober->init( conf_file ) )
				throw "SegmentFactory::init init _p_name_prober fail";
		}

		_p_address_prober = NULL;
		if( config.isKeyExists( ENABLE_ADDRESS_ENTITY_RECOGNITION_NAME ) && config[ ENABLE_ADDRESS_ENTITY_RECOGNITION_NAME ] == TRUE )
		{
			_p_address_prober = new AddressEntityProber();
			if( !_p_address_prober || !_p_address_prober->init( conf_file ) )
				throw "SegmentFactory::init init _p_address_prober fail";
		}

		bool use_bidirectional = false;
		if( config.isKeyExists( ENABLE_BIDIRECTIONAL_NAME ) && config[ ENABLE_BIDIRECTIONAL_NAME ] == TRUE )
			use_bidirectional = true;
		
		
		_p_segtor = new Segment( _p_fwd_dict, _p_bwd_dict, _p_name_prober, _p_address_prober, use_bidirectional );
		if( !_p_segtor )
			throw "SegmentFactory::init new _p_segtor fail";
#if __DEBUG_
time_t end_time = clock();
printf("SegmentFactory::init over! Used: [ %d ] us\n",end_time - start_time );
#endif

	}
	catch(string msg)
	{
#if __DEBUG_
		printf("SegmentFactory::init error as[ %s ]\n",msg.c_str() );
#endif
		_releaseResource();
		return false;
	}

	return true;
}

DoubleHashDict* SegmentFactory::_loadDict(const string& dict_name )
{
	if( dict_name.empty()  ) return NULL;

	DoubleHashDict *p_dict = new DoubleHashDict();
	if( !p_dict || !p_dict->loadHashDict( dict_name.c_str() ) ) return NULL;

	return p_dict;
}


