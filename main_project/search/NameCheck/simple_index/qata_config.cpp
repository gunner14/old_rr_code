#include "qata_config.h"
#include "qata_typechecker.h"
#include "utility.h"
#include <cstdio>
#include <vector>
#include <iostream>

using namespace std;

const string LINE_SYMBLE = "\n";
const string ASSIGN_SYMBLE = "=";
const string TRIM_SYMBLE = " \t";
const char COMMENT_SYMBLE = '#';
const char VAR_SYMBLE = '$';
const char VAR_END_SYMBLE = '.';
const char FILE_INCLUDE_SYMBLE = '@';

const char BRACKET_OPEN_SYMBLE = '[';
const char BRACKET_CLOSE_SYMBLE = ']';

const string TYPE_CHECK_ON_FLAG = "type check on";
const string TYPE_CHECK_OFF_FLAG = "type check off";

const string BLOCK_COMMENT_ON_FLAG = "block comment on";
const string BLOCK_COMMENT_OFF_FLAG = "block comment off";

const string FILE_TYPE_CHECK_FLAG = "f";
const string DIGIT_TYPE_CHECK_FLAG = "n";
const string STRING_TYPE_CHECK_FLAG = "s";
const string FLOAT_TYPE_CHECK_FLAG = "d";

QataConfig::QataConfig()
{
	_is_typecheck_on = false;
	_is_block_comment_on = false;
}

bool QataConfig::loadConfig(const char* filename)
{
	string filecontent;
	if( getFileContent( filename , filecontent ) == -1 ) return false;
	
	vector<string> lines_vec;
	splitTxt( filecontent , LINE_SYMBLE , lines_vec );

	if( lines_vec.empty() ) return false;

	for(int i = 0 ; i < lines_vec.size(); i++ )
	{
		if( lines_vec[i].empty() )
			continue;
		
		if( lines_vec[i][0] == COMMENT_SYMBLE )
			continue;
		
		if( lines_vec[i][0] == FILE_INCLUDE_SYMBLE && lines_vec[i].size() > 2)
		{
			string include_file_name = lines_vec[i].substr( 1 );
			_add_file_include( include_file_name.c_str() );
		}

		if( lines_vec[i][0] == BRACKET_OPEN_SYMBLE )
		{
			_set_options( lines_vec[i] );
			continue;
		}

		if( _is_block_comment_on )
			continue;
		
		vector<string> vec;
		splitTxt( lines_vec[i] , ASSIGN_SYMBLE , vec );

		if( vec.size() != 2 )
			continue;
		
		_addToMap( trimStr( vec[0] , TRIM_SYMBLE ) , trimStr( vec[1] , TRIM_SYMBLE ) );
	}

	return true;
}

bool QataConfig::_add_file_include( const char *file_name )
{
	if( strlen( file_name ) == 0 || !TypeChecker::isValidFileName( file_name ) )
		return false;

	return loadConfig( file_name );
}

bool QataConfig::_addToMap(const string& key,const string& value)
{
	if( key.empty() || value.empty() )
		return false;
	
	if( value.find( VAR_SYMBLE ) != string::npos )
	{
		string value_str;
		if( !_convert_var_str( value , value_str ) )
			return false;
		
		if( _is_typecheck_on ) 
		{
			string parse_str;
			if( !_type_checking( value_str , parse_str ) )
				return false;
			
			_config_db[ key ] = parse_str;
		}
		else
		{
			_config_db[ key ] = value_str;
		}
	}
	else
	{
		if( _is_typecheck_on ) 
		{
			string parse_str;
			if( !_type_checking( value , parse_str ) )
				return false;
			
			_config_db[ key ] = parse_str;
		}
		else
		{
			_config_db[ key ] = value;
		}
	}

	return true;
}

bool QataConfig::_convert_var_str(const string& src_str,string& ret_str)
{
	size_t start_idx = 0;
	size_t var_start_idx = 0;
	while(  start_idx < src_str.size() && ( var_start_idx = src_str.find_first_of( VAR_SYMBLE , start_idx ) ) != string::npos)
	{
		size_t var_end_idx = src_str.find_first_of( VAR_END_SYMBLE , var_start_idx );
		
		string key_name = src_str.substr( var_start_idx+1 , var_end_idx-var_start_idx-1 );
		if( key_name.empty() || !_isKeyExists( key_name ) ) return false;
		
		string key_value = _config_db[ key_name ];

		ret_str = ret_str + src_str.substr( start_idx , var_start_idx - start_idx ) + key_value;

		start_idx = var_end_idx == string::npos ? src_str.size() : var_end_idx + 1;
	}

	return true;
}

string QataConfig::operator [](const string& key)
{
	return _getValue( key );
}

StrPair QataConfig::operator [](int idx )
{
	if( idx < 0 || idx >= Size() )
		return StrPair("","");
	
	StrMapIter itor = _config_db.begin();
	int i = 0;
	while( i++ < idx )itor++;

	return StrPair(itor->first,itor->second);
}

int QataConfig::Size()
{
	return _config_db.size();	
}

string QataConfig::_getValue( const string& key )
{
	if( !_isKeyExists( key ) ) 
		return "";

	return _config_db[ key ];
}

bool QataConfig::isKeyExists( const string& key )
{
	return _isKeyExists( key );
}

bool QataConfig::_isKeyExists( const string& key )
{
	if( _config_db.find( key ) == _config_db.end() )
		return false;

	return true;
}

bool QataConfig::_type_checking(const string& src_str,string& ret_str)
{
	try
	{
		string flag;
		if( !_parse_check_flag( src_str , ret_str , flag ) )
			throw "Parse Error: "+src_str;
		
		lower_case_str( flag );
		if( flag == FILE_TYPE_CHECK_FLAG )
		{
			if( !TypeChecker::isValidFileName( ret_str ) )
				throw "Check File Error:"+src_str;
			
			return true;
		}
		else if( flag == DIGIT_TYPE_CHECK_FLAG )
		{
			if( !TypeChecker::isDigitStr( ret_str ) )
				throw "Check Digit Error:"+src_str;
			
			return true;
		}
		else if( flag == STRING_TYPE_CHECK_FLAG )
		{
			if( !TypeChecker::isAlpNumSymStr( ret_str ) )
				throw "Check String Error:"+src_str;
			
			return true;
		}
		else if( flag == FLOAT_TYPE_CHECK_FLAG )
		{
			if( !TypeChecker::isFloatStr( ret_str ) )
				throw "Check Float Error:"+src_str;
			
			return true;
		}
		else
		{
			return false;
		}
			
	}
	catch( string msg )
	{
		printf("QataConfig::_type_checking Error: [%s]\n",msg.c_str());
		return false;
	}
	
	return false;
}

bool QataConfig::_set_options(const string& str )
{
	size_t flag_start_idx = 0;
	string flag_str;
		
	if( !_parse_bracket( str , flag_str , flag_start_idx ) )
		return false;
	
	lower_case_str( flag_str );
	if( !_is_block_comment_on && flag_str == TYPE_CHECK_ON_FLAG )
	{
		_is_typecheck_on = true;
	}
	else if( !_is_block_comment_on && flag_str == TYPE_CHECK_OFF_FLAG )
	{
		_is_typecheck_on = false;
	}
	else if( flag_str == BLOCK_COMMENT_ON_FLAG )
	{
		_is_block_comment_on = true;
	}
	else if( flag_str == BLOCK_COMMENT_OFF_FLAG )
	{
		_is_block_comment_on = false;
	}
	
	return true;
}

bool QataConfig::_parse_check_flag(const string& src_str,string& ret_str,string& flag)
{
	size_t flag_start_idx = 0;
	if( !_parse_bracket( src_str , flag , flag_start_idx ) )
		return false;

	int str_size = flag_start_idx ;
	if( str_size < 1 )
		return false;
	
	string tmp = src_str.substr( 0 , str_size );
	ret_str = trimStr( tmp , TRIM_SYMBLE );

	if( ret_str.empty() )
		return false;
	
	return true;
}

bool QataConfig::_parse_bracket(const string& src_str,string& ret_str,size_t& start_idx)
{
	size_t flag_start_idx = src_str.find_first_of( BRACKET_OPEN_SYMBLE );
	size_t flag_end_idx = src_str.find_last_of( BRACKET_CLOSE_SYMBLE );

	int flag_size = flag_end_idx - flag_start_idx - 1;

	if(	flag_size < 1 )
		return false;
	
	ret_str = src_str.substr( flag_start_idx + 1 , flag_size );
	if( ret_str.empty() )
		return false;

	start_idx = flag_start_idx;
	return true;
}
