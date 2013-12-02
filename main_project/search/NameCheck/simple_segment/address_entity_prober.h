#ifndef __N_ADDRESS_ENTITY_PROBER_H__
#define __N_ADDRESS_ENTITY_PROBER_H__

#include "segment_struct.h"

using namespace std;
using namespace segment;

namespace segment
{
	class AddressTB;

	class AddressEntityProber
	{       
		public: 
			AddressEntityProber();
			~AddressEntityProber();
			bool init( const char* conf_file );

			bool probeAddressEntity( wordAry& ary,size_t& idx,wordAry& ret );
		private:
			AddressTB *_ptr_state_tb;
	};      	
}

#endif
