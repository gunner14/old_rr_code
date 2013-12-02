#include "qata_typechecker.h"
#include "utility.h"
#include <unistd.h>

using namespace std;

static const string BOOL_T_FLAG = "true";
static const string BOOL_F_FLAG = "false";

bool TypeChecker::isValidFileName(const string& fileName)
{
	if( fileName.empty() )
		return false;
	
	int file_open_flag = O_RDONLY;
	int fdin;
	if( ( fdin = open( fileName.c_str(),file_open_flag ) ) < 0 )
		return false;
	
	if( close( fdin ) < 0 )
		return false;

	return true;
}

bool TypeChecker::isDigitStr(const string& str)
{
	return is_number_str( str );	
}

bool TypeChecker::isFloatStr(const string& str)
{
	return is_float_str( str );
}

bool TypeChecker::isAlphaStr( const string& str)
{
	return is_alpha_str( str );
}

bool TypeChecker::isAlpNumStr(const string& str)
{
	return is_alnum_str( str );
}

bool TypeChecker::isAlpNumSymStr(const string& str)
{
	return is_alnumsym_str( str );
}

bool TypeChecker::isBoolStr(const string& str)
{
	string low_str = str;
	lower_case_str( low_str );
	
	return (BOOL_T_FLAG == low_str || BOOL_F_FLAG == low_str)?true:false; 
}

