#ifndef __N_ADDRESS_TB_H__
#define __N_ADDRESS_TB_H__

#include "container.h"
#include "segment_struct.h"
#include <map>
#include <string>

using namespace std;

static const size_t MAX_ROW_NUM = 10;
static const size_t MAX_COLUM_NUM = 10;

static const int START_STATE_SYM = 1;
static const int END_STATE_SYM = 2;

namespace segment
{
	class AddressTB
	{
		public:
			AddressTB();
			~AddressTB();

			bool initAddressTB( const char* state_tb_file );
			void initState();
			int getCurrentState();
			bool isTerminalState();
			bool advanceOneStep( const char* s );
		public:
			bool _initStateTB( const char* state_tb_file );
			bool _initTransferTB( const char* state_tb_file );
							
		private:
			int _tranfser_tb[ MAX_ROW_NUM][MAX_COLUM_NUM];
			HashIMap _state_id_db;
			int _cur_state;
	};
	
};


#endif
