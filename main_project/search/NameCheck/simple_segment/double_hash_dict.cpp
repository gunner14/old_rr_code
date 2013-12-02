#include "double_hash_dict.h"
#include "segment_const.h"
#include <fstream>
#include <vector>
#include "utility.h"

using namespace std;
using namespace segment;
using namespace __gnu_cxx;

static const char SPECIAL_TOKEN = '@';  
static const string SPECIAL_TOKEN_STR = "@";  

static const string CONNTOR_TOKEN_STR = "-";  
static const string SPLIT_TOKEN_STR = "\t";  

static bool sortByStrLargerLength( const string& l_str , const string& r_str )
{
	return l_str.size() > r_str.size();
}

static bool _swapSepcialToken( StrAry& str_ary )
{
	if( str_ary.size() < 2 ) return false;

	StrAry::iterator itor = find( str_ary.begin() , str_ary.end() , SPECIAL_TOKEN_STR );
	int idx = itor - str_ary.begin();
	if( idx >= str_ary.size() - 1 ) return false;
	
	string tmp = str_ary[ str_ary.size() - 1 ];
	str_ary.back() = str_ary[ idx ];
	str_ary[ idx ] = tmp;
	
	return true;
}

DoubleHashDict::DoubleHashDict()
{}

DoubleHashDict::~DoubleHashDict()
{
	/*
	DoubleHashMap::iterator first_itor = _hash_db.begin();
	DoubleHashMap::iterator first_end_itor = _hash_db.end();
	
	while( first_itor != first_end_itor )
	{
		StrHashMap::iterator second_itor = (first_itor->second).begin();
		StrHashMap::iterator second_end_itor = (first_itor->second).end();
		
		while( second_itor != second_end_itor )
		{
			char *p = (char*) second_itor->first;
			//printf("delete %s [ %d ] %x\n",p,strlen(p),p);
			delete[] p;
			second_itor++;
		}

		first_itor++;
	}
	*/
}

bool DoubleHashDict::loadHashDict(const char* dict_file )
{
	ifstream infile( dict_file );
	if( !infile.is_open() )
		return false;

	string line;
	while( getline(infile,line) )
	{
		if( line.empty() )
			continue;
		
		vector<string> vec;
		splitTxt( line , SPLIT_TOKEN_STR ,vec );
		if( vec.size() < 3 )
			continue;

		string primary_key = vec[0];
		string second_key = vec[1];
		string value = vec[2];
		
		vector<string> value_vec;
		splitTxt( value ,CONNTOR_TOKEN_STR ,value_vec );

#ifdef __DEBUG_
	//printf("DoubleHashDict::loadHashDict parser primary_key:%s second_key:%s v:%d %s\n",primary_key.c_str(),second_key.c_str(),value_vec.size(),value.c_str());
#endif

	
		_insertNewKey( primary_key , second_key , value_vec );	
	}

	infile.close();
	return true;
}

bool DoubleHashDict::_insertNewKey(const string& primary_key,const string& second_key,StrAry& value_ary )
{
	if( primary_key.empty() || second_key.empty() )
		return false;
	
	if( _hash_db.find( primary_key.c_str() ) == _hash_db.end() )
	{
		char *p = new char[ primary_key.size() ];
		strcpy( p , primary_key.c_str() );
		
		StrHashMap new_hash_map;
		_hash_db[ p ] = new_hash_map;
	}

	StrHashMap& ref_hash = _hash_db[ primary_key.c_str() ];
	if( ref_hash.find( second_key.c_str() ) == ref_hash.end() )
	{
		char *p = new char[ second_key.size() ];
		strcpy( p , second_key.c_str() );
		ref_hash[ p ] = value_ary;
	}

	return true;
}

bool DoubleHashDict::preMatchMaxWord(const char* buf,size_t len,size_t& wd_len)
{
#ifdef __DEBUG_
printf("DoubleHashDict::preMatchMaxWord start:\n");
#endif
	if( len < BASE_WORD_SIZE || len > _MAX_WORD_LENGTH ) return false;

	char *primary_key = (char*) buf;
	char end_ch = primary_key[ GBK_CHAR_SIZE ];
	primary_key[ GBK_CHAR_SIZE ]  = 0;

#ifdef __DEBUG_
printf("DoubleHashDict::preMatchMaxWord primary_key: %s\n" ,primary_key);
#endif
	DoubleHashMap::iterator h_itor = _hash_db.find( primary_key );
	primary_key[ GBK_CHAR_SIZE ] = end_ch;
	if(  h_itor == _hash_db.end() ) return false;
	
	StrHashMap& ref_second_map = h_itor->second;
#ifdef __DEBUG_
printf("DoubleHashDict::preMatchMaxWord search primary_key sucess, get ref_second_map[ size = %d ]\n" ,ref_second_map.size());
	/*
	StrHashMap::iterator itor = ref_second_map.begin();
	StrHashMap::iterator end = ref_second_map.end();
	int si = 0;
	while( itor != end )
	{
		printf("\t\tvalue: %s\n" ,(itor->first) );
		itor++;
	}
	*/
#endif
	

	char *second_key = (char*)buf + GBK_CHAR_SIZE;
	end_ch = second_key[ GBK_CHAR_SIZE ];
	second_key[ GBK_CHAR_SIZE ] = 0;

#ifdef __DEBUG_
printf("DoubleHashDict::preMatchMaxWord second_key: buf [ %s ] len:[ %d ]\n", buf, len );
printf("DoubleHashDict::preMatchMaxWord second_key: %s [ %x ]\n", second_key, *second_key );
#endif
	
	StrHashMap::iterator s_itor = ref_second_map.find( second_key );
	second_key[ GBK_CHAR_SIZE ] = end_ch;
	if( s_itor == ref_second_map.end() ) return false;
	
	StrAry& ref_word_ary = s_itor->second;
	if( ref_word_ary.empty() ) return false;

#ifdef __DEBUG_
printf("DoubleHashDict::preMatchMaxWord search second_key sucess, get ref_second_ary[ size = %d ]\n" ,ref_word_ary.size());
#endif
	if( !_prematchMaxWord( buf , len  , ref_word_ary ,wd_len ) ) return false;

#ifdef __DEBUG_
printf("DoubleHashDict::preMatchMaxWord search max_word sucess, get wd_len[ %d ]\n" ,wd_len);
#endif
	return true;
}

bool DoubleHashDict::_prematchMaxWord(const char *buf , size_t len ,StrAry& value_ary,size_t& wd_len)
{

	char *str = (char*)buf + BASE_WORD_SIZE;
	size_t ary_size = value_ary.size();

	for( size_t idx = 0 ; idx < ary_size ; idx++ )
	{
		string &ref_str = value_ary[ idx ];
		size_t v_size = ref_str.size();
		char orig_terminal_ch = str[ v_size ];
		str[ v_size ] = 0;

		if( strcmp( str , ref_str.c_str() )  == 0 )
		{
			wd_len =  BASE_WORD_SIZE + v_size;
			str[ v_size ] = orig_terminal_ch;
			return true;
		}


		str[ v_size ] = orig_terminal_ch;
	}
	
	if( value_ary.back()[0] == SPECIAL_TOKEN ) 
	{
		wd_len = BASE_WORD_SIZE;
		return true;
	}
	
	return false;
}

int DoubleHashDict::bulidPreHashDictFromTxt(const char* txt_file )
{
	return _bulidHashDictFromTxt( txt_file , false );	
}

int DoubleHashDict::bulidProHashDictFromTxt(const char* txt_file )
{
	return _bulidHashDictFromTxt( txt_file , true );
}

int DoubleHashDict::_bulidHashDictFromTxt(const char* txt_file , bool is_pro )
{
	ifstream infile( txt_file );
	if( !infile.is_open() )
		return 0;

	string line;
	while( getline(infile,line) )
	{
		if( line.empty() || line.size() < BASE_WORD_SIZE )
			continue;	
		
		string primary_key;
		string second_key;
		if( is_pro )
		{
			primary_key = line.substr(  line.size() - GBK_CHAR_SIZE );
			second_key = line.substr( line.size() - BASE_WORD_SIZE , GBK_CHAR_SIZE );
		}
		else
		{
			primary_key = line.substr( 0 , GBK_CHAR_SIZE );
			second_key = line.substr( GBK_CHAR_SIZE , GBK_CHAR_SIZE );
		}
		
		char *p_primary_key = new char[ primary_key.size() ];
		char *p_second_key = new char[ second_key.size() ];
		if( !p_primary_key || !p_second_key ) continue;

		strcpy( p_primary_key , primary_key.c_str() );
		strcpy( p_second_key , second_key.c_str() );
		
		if( _hash_db.find( p_primary_key ) == _hash_db.end() )
		{
			StrHashMap new_str_map;
			_hash_db[ p_primary_key ] = new_str_map;
		}
	
		StrHashMap& ref_str_map = _hash_db[ p_primary_key ];
		if( ref_str_map.find( p_second_key ) == ref_str_map.end() )
		{
			StrAry new_str_ary;
			ref_str_map[ p_second_key ] = new_str_ary;
		}

		string value_str = SPECIAL_TOKEN_STR;

		if( line.size() > BASE_WORD_SIZE ) 
		{
			if( is_pro )
				value_str = line.substr(  0 , line.size() - BASE_WORD_SIZE );
			else
				value_str = line.substr( BASE_WORD_SIZE );
		}
#ifdef __DEBUG_
		printf("DoubleHashDict::bulidHashDictFromTxt p_primary_key = %s,p_second_key = %s , value = %s\n",p_primary_key,p_second_key,value_str.c_str());
#endif
		ref_str_map[ p_second_key ].push_back( value_str );
	}
	
	infile.close();

	_sortHashDict();
	return _hash_db.size();
}

void DoubleHashDict::_sortHashDict()
{
	DoubleHashMap::iterator itor = _hash_db.begin();
	DoubleHashMap::iterator end = _hash_db.end();
	
	while( itor != end )
	{
		StrHashMap& ref_str_map = itor->second;

		StrHashMap::iterator s_itor = ref_str_map.begin();
		StrHashMap::iterator s_end = ref_str_map.end();
		while( s_itor != s_end )
		{
			StrAry& ref_str_ary = s_itor->second;
			sort( ref_str_ary.begin(),ref_str_ary.end(),sortByStrLargerLength);
			_swapSepcialToken( ref_str_ary );

			s_itor++;
		}
		
		itor++;
	}
}

bool DoubleHashDict::saveHashDict( const char* save_file )
{
	ofstream outfile( save_file );
	if( !outfile.is_open() ) return false;

	DoubleHashMap::iterator itor = _hash_db.begin();
	DoubleHashMap::iterator end = _hash_db.end();

	while( itor != end )
	{
		StrHashMap& ref_str_map = itor->second;

		StrHashMap::iterator s_itor = ref_str_map.begin();
		StrHashMap::iterator s_end = ref_str_map.end();
		while( s_itor != s_end )
		{
			outfile<<itor->first<<SPLIT_TOKEN_STR<<s_itor->first;
			StrAry& ref_str_ary = s_itor->second;
			if( !ref_str_ary.empty() )
			{
				outfile<<SPLIT_TOKEN_STR<<ref_str_ary[0];
				for( size_t idx = 1 ; idx < ref_str_ary.size() ; idx++ )
					outfile<<CONNTOR_TOKEN_STR<<ref_str_ary[ idx ];
			}
		
			outfile<<endl;	
			s_itor++;
		}
		
		itor++;
	}
	
	outfile.close();
	return true;
}

bool DoubleHashDict::proMatchMaxWord(const char* buf,size_t len,size_t& wd_len)
{
#ifdef __DEBUG_
printf("DoubleHashDict::proMatchMaxWord start:\n");
#endif
	if( len < BASE_WORD_SIZE || len > _MAX_WORD_LENGTH ) return false;

	char *primary_key = (char*) buf + len - GBK_CHAR_SIZE;
	char end_ch = primary_key[ GBK_CHAR_SIZE ];
	primary_key[ GBK_CHAR_SIZE ]  = 0;

#ifdef __DEBUG_
printf("DoubleHashDict::proMatchMaxWord primary_key: %s\n" ,primary_key);
#endif
	DoubleHashMap::iterator h_itor = _hash_db.find( primary_key );
	primary_key[ GBK_CHAR_SIZE ] = end_ch;
	if(  h_itor == _hash_db.end() ) return false;
	
	StrHashMap& ref_second_map = h_itor->second;
#ifdef __DEBUG_
printf("DoubleHashDict::proMatchMaxWord search primary_key sucess, get ref_second_map[size = %d ]\n" ,ref_second_map.size());
#endif

	char *second_key = (char*)buf + len - BASE_WORD_SIZE;
	end_ch = second_key[ GBK_CHAR_SIZE ];
	second_key[ GBK_CHAR_SIZE ] = 0;

#ifdef __DEBUG_
printf("DoubleHashDict::proMatchMaxWord second_key: %s\n" ,second_key);
#endif
	
	StrHashMap::iterator s_itor = ref_second_map.find( second_key );
	second_key[ GBK_CHAR_SIZE ] = end_ch;
	if( s_itor == ref_second_map.end() ) return false;
	
	StrAry& ref_word_ary = s_itor->second;
	if( ref_word_ary.empty() ) return false;

#ifdef __DEBUG_
printf("DoubleHashDict::proMatchMaxWord search second_key sucess, get ref_second_ary[size = %d ]\n" ,ref_word_ary.size());
#endif
	if( !_promatchMaxWord( buf , len  , ref_word_ary ,wd_len ) ) return false;

#ifdef __DEBUG_
printf("DoubleHashDict::proMatchMaxWord search max_word sucess, get wd_len[ %d ]\n" ,wd_len);
#endif
	return true;
}

bool DoubleHashDict::_promatchMaxWord(const char *buf , size_t len ,StrAry& value_ary,size_t& wd_len)
{
	size_t end_offset =  len - BASE_WORD_SIZE;
	char *base_str = (char*)buf + end_offset;
	size_t ary_size = value_ary.size();

	char term_ch = buf[ end_offset ];
	*base_str = 0;
	for( size_t idx = 0 ; idx < ary_size ; idx++ )
	{
		string& ref_str = value_ary[ idx ];
		size_t v_size = ref_str.size();
		char *str = base_str - v_size;
		if( strcmp( str , ref_str.c_str() )  == 0 )
		{
			wd_len =  BASE_WORD_SIZE + v_size;
			*base_str = term_ch;
			return true;
		}

	}
	
	*base_str = term_ch;
	if( value_ary.back()[0] == SPECIAL_TOKEN ) 
	{
		wd_len = BASE_WORD_SIZE;
		return true;
	}
	
	return false;
}

