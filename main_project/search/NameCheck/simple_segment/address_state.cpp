#include "address_state.h"
#include "utility.h"
#include <fstream>

using namespace std;
using namespace segment;

static const string TB_STR = "\t";
static const string CON_STR = "|";
static const string NONSTATE_STR = "NONE";
static const int NONSTATE_V = -1;

AddressTB::AddressTB()
{}

AddressTB::~AddressTB()
{
	HashIMap::iterator itor = _state_id_db.begin();
        HashIMap::iterator end = _state_id_db.end();
        while( itor != end )
        {       
                char *p = (char*) itor->first;
                if( p ) delete []p;
                itor++; 
        }  
}

bool AddressTB::initAddressTB( const char* state_tb_file )
{
	if( !state_tb_file || strlen( state_tb_file ) == 0 ) return false;

	if( !_initStateTB( state_tb_file ) || !_initTransferTB( state_tb_file ) )
		return false;

	return true;
}

bool AddressTB::_initStateTB( const char* state_tb_file )
{
	ifstream infile( state_tb_file );
	if( !infile.is_open() ) return false;

	string line;
	size_t row_count = 0;
	getline( infile , line );

	vector<string> svec;
	splitTxt(  line , TB_STR , svec);
	for( size_t idx = 0 ; idx < svec.size() ;idx++ )
	{
#if __DEBUG_
		printf("AddressTB::_initStateTB add new state %s at: %d\n",svec[idx].c_str(),idx);
#endif
		char *p_ch = new char[ svec[idx].size() + 1];
		strcpy( p_ch , svec[idx].c_str() );
		_state_id_db[ p_ch ] = idx;
	}
	
	infile.close();	
	return true;
}

bool AddressTB::_initTransferTB( const char* state_tb_file )
{
	ifstream infile( state_tb_file );
	if( !infile.is_open() ) return false;

	string line;
	size_t row_count = 1;

	//ignore the first row
	getline( infile , line );

	while( getline( infile , line ) && row_count < MAX_ROW_NUM )
	{
		if( line.empty() ) continue;
		vector<string> vec;
		splitTxt( line , TB_STR , vec );

		if( vec.size() >= MAX_COLUM_NUM ) continue;

		for( size_t idx = 0 ; idx < vec.size() ;idx++ )
		{
			if( vec[idx] == NONSTATE_STR ) 
			{
				_tranfser_tb[ row_count ][ idx ] = NONSTATE_V;
				continue;
			}
#ifdef __DEBUG_
			printf("AddressTB::_initTransferTB state at[ %d ] get_state_id:[ %d ] then transfer to [ %d ]\n",row_count,idx,atoi( vec[idx].c_str() ) );
#endif

			_tranfser_tb[ row_count ][ idx ] = atoi( vec[idx].c_str() );
		}

		row_count++;
	}		
	
	infile.close();
	return true;	
}

void AddressTB::initState()
{ 
	_cur_state = START_STATE_SYM;
}

int AddressTB::getCurrentState()
{ 
	return _cur_state;
}

bool AddressTB::isTerminalState()
{ 
	return END_STATE_SYM == _cur_state;
 }

bool AddressTB::advanceOneStep( const char* s )
{
	if( !s  || strlen( s ) == 0 ) return false;

	HashIMap::iterator itor =  _state_id_db.find( s );
	if( itor == _state_id_db.end() ) return false;

	size_t colum_idx = itor->second;
	size_t next_state = _tranfser_tb[ _cur_state ][ colum_idx ];

	if( next_state == NONSTATE_V ) return false;
#ifdef __DEBUG_
	printf("AddressTB::advanceOneStep cur_state: %d, str: %s , next_state: %d\n",_cur_state,s,next_state);
#endif

	_cur_state = next_state;
	return _cur_state == END_STATE_SYM;		
}

