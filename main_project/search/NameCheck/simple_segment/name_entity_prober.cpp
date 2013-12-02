#include "name_entity_prober.h"
#include "utility.h"
#include "segment_const.h"
#include "qata_config.h"
#include "config_const.h"
#include <string>
#include <vector>
#include <fstream>
using namespace std;
using namespace segment;

static const string TAB_STR = "\t";
static const float DEF_THRESHOLD = 1;

NameEntityProber::NameEntityProber()
{}

NameEntityProber::~NameEntityProber()
{
	_freeDb( _surname_db );
	_freeDb( _dual_first_name_db );
	_freeDb( _dual_second_name_db );
	_freeDb( _single_name_db );
}

bool NameEntityProber::init( const char *conf_file )
{
	QataConfig config;
//	cout<<"NameCheck :: 1"<<endl;
	if( !config.loadConfig( conf_file ) ) return false;
//	cout<<"NameCheck :: 2"<<endl;


	if( !config.isKeyExists( SURNAME_ENTITY_FILE_NAME ) ) return false;
//	cout<<"NameCheck :: 3"<<endl;
	if( !_loadDb( config[ SURNAME_ENTITY_FILE_NAME ].c_str()  , _surname_db ) )
		return false;

	if( !config.isKeyExists( DUFIRST_ENTITY_FILE_NAME ) ) return false;
	if( !_loadDb( config[ DUFIRST_ENTITY_FILE_NAME ].c_str() , _dual_first_name_db ) )
		return false;

	if( !config.isKeyExists( DUSECOND_ENTITY_FILE_NAME ) ) return false;
	if( !_loadDb( config[ DUSECOND_ENTITY_FILE_NAME ].c_str() , _dual_second_name_db ))
		return false;

	if( !config.isKeyExists( SINNAME_ENTITY_FILE_NAME ) ) return false;
	if( !_loadDb( config[ SINNAME_ENTITY_FILE_NAME ].c_str() , _single_name_db ) )
		return false;

	_dual_name_threshold = DEF_THRESHOLD;
	_sin_name_threshold = DEF_THRESHOLD;
	_sur_name_threshold = DEF_THRESHOLD;
	_name_ratio = DEF_THRESHOLD;
	
	if( config.isKeyExists( SURNAME_THRESHOLD_NAME ) )
		_sur_name_threshold = atof( config[ SURNAME_THRESHOLD_NAME ].c_str() );

	if( config.isKeyExists( SINNAME_THRESHOLD_NAME ) )
		_sin_name_threshold = atof( config[ SINNAME_THRESHOLD_NAME ].c_str() );

	if( config.isKeyExists( DUALNAME_THRESHOLD_NAME ) )
		_dual_name_threshold = atof( config[ DUALNAME_THRESHOLD_NAME ].c_str() );

	if( config.isKeyExists( NAME_RATIO_NAME ) )
		_name_ratio = atof( config[ NAME_RATIO_NAME ].c_str() );

	return true;
}

bool NameEntityProber::_loadDb( const char * db_file,HashFMap& db )
{
	if( !db_file || strlen( db_file ) == 0 ) return false;

	ifstream infile( db_file );
	if( !infile.is_open() ) return false;

	string line;
	while( getline( infile , line ) )
	{
		vector<string> vec;
		splitTxt( line , TAB_STR , vec );
		if( vec.size() != 2 ) continue;

		char *p = new char[ vec[0].size() + 1];
		strcpy( p ,  vec[0].c_str() );
		db[ p ] = atof( vec[1].c_str() );
	}

	return true;
}

void NameEntityProber::_freeDb( HashFMap& db )
{
	HashFMap::iterator itor = db.begin();
	HashFMap::iterator end = db.end();
	while( itor != end )
	{
		char *p = (char*) itor->first;
		if( p ) delete []p;
		itor++;
	}
}

bool NameEntityProber::probeNameEntity( wordAry& ary,size_t& idx,wordAry& ret )
{
	float surname_score = 0;
    if(ary.size() == 1)
    {
        if( !_getSurnameScore( ary[0],surname_score))
        {
            return false;
        }
        if (surname_score < _sur_name_threshold)
        {
            return false;
        }
        ret.push_back(ary[0]);
        return true;
    }
	//cout<<"1.index is :"<<idx<<endl;
	if( idx >= ary.size() - 1 ) return false;
	wordItem& first_wd_item = ary[ idx ];
	wordItem& second_wd_item = ary[ idx+1 ];

	//cout<<"_sur_name_threshold::"<<_sur_name_threshold<<endl;
	if( !_getSurnameScore( first_wd_item , surname_score ) ){
	//	cout<<"getSurnameScore is false!\n"<<endl;
		return false;
	}

	if( surname_score < _sur_name_threshold ){
	//	cout<<"surname_score::"<<surname_score<<endl;
		return false;
	}
	float sin_name_score = 0;
	_isPossibleSinName( second_wd_item , sin_name_score );

	float dual_name_score = 0;
	bool is_dual_name = false;
	if( idx < ary.size() - 2 )
	{
		wordItem& third_wd_item = ary[ idx+2 ];
		_isPossibleDualName( second_wd_item , third_wd_item ,dual_name_score );
	}
	//cout<<endl;
	return _mergeResult( sin_name_score , dual_name_score , ary , idx , ret );	
}

bool NameEntityProber::_mergeResult( float sin_name_score ,float dual_name_score,wordAry& ary , size_t& idx ,wordAry& ret )
{
	if( sin_name_score == 0 && dual_name_score == 0 ) return false;
	
	wordItem name_item;
	name_item.offset = ary[idx].offset;
	name_item.ptr_wd = ary[idx].ptr_wd;
	
//	cout<<"before push the idx is "<<idx<<endl;
	if( sin_name_score > dual_name_score * _name_ratio )
	{
		name_item.length = GBK_CHAR_SIZE*2;
		idx += 2;
	}
	else
	{
		name_item.length = GBK_CHAR_SIZE*3;
		idx += 3;
	}
#ifdef __DEBUG_
__printWord("NameEntityProber::_mergeResult get NameEntity",name_item );
printf("NameEntityProber::_mergeResult: sin_name_score = %f , dual_name_score = %f\n",sin_name_score,dual_name_score * _name_ratio );
#endif
	ret.push_back( name_item );
	//cout<<name_item.ptr_wd<<endl;
//	cout<<"after push the idx is "<<idx<<endl;
	return true;
}

bool NameEntityProber::_isPossibleSinName( wordItem& second_name , float& s_score)
{
	float second_score = 0;
	if( !_getSinnameScore( second_name , second_score ) ) return false;

#ifdef __DEBUG_
printf("NameEntityProber::_isPossibleSinName get sin_name_score = %f\n", second_score );
#endif
	if( _sin_name_threshold > second_score )
	{
		s_score = 0;
		return false;
	}

	s_score = second_score;
	return true;
}

bool NameEntityProber::_isPossibleDualName( wordItem& second_name , wordItem& third_name ,float& s_score)
{
	float third_score = 0;
	if( !_getDualSecondScore( third_name , third_score ) ) return false;

	float second_score = 0;
	if( !_getDualFirstScore( second_name , second_score ) ) return false;

	float agv_score = ( third_score + second_score ) / 2;

#ifdef __DEBUG_
printf("NameEntityProber::_isPossibleDualName get dual_name_score = %f\n" ,agv_score);
#endif
	if( _dual_name_threshold > agv_score )
	{
		s_score = 0;
		return false;
	}

	s_score = max( second_score , third_score);
	return true;
}

bool NameEntityProber::_getSurnameScore( wordItem& wd , float& score)
{
	return _getScore( _surname_db , wd , score );
}

bool NameEntityProber::_getSinnameScore( wordItem& wd , float& score)
{
	return _getScore( _single_name_db , wd , score );
}

bool NameEntityProber::_getDualFirstScore( wordItem& wd , float& score)
{
	return _getScore( _dual_first_name_db , wd , score );
}

bool NameEntityProber::_getDualSecondScore( wordItem& wd , float& score)
{
	return _getScore( _dual_second_name_db , wd , score );
}

bool  NameEntityProber::_getScore( HashFMap& db ,wordItem& wd , float& score )
{
	if( wd.length != GBK_CHAR_SIZE ){
	//	cout<<"wd.length!= 2!!!!!!"<<endl;
		return false;
	}
	char *p = (char*) wd.ptr_wd;
	char end_ch = p[ GBK_CHAR_SIZE ];
	p[ GBK_CHAR_SIZE ] = 0;

	HashFMap::iterator itor = db.find( p );
	p[ GBK_CHAR_SIZE ] = end_ch;
	if( itor == db.end() ){
	//	cout<<"There is no wd:: "<<p<<" ::,which can find in  db\n";
		return false;
	}
	score = itor->second;
#ifdef __DEBUG_
printf("NameEntityProber::_getScore get: %s score=> %f\n",itor->first,score);
#endif
	return true;	
}
