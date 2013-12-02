#ifndef __N_NAME_ENTITY_PROBER_H__
#define __N_NAME_ENTITY_PROBER_H__

#include "container.h"
#include "segment_struct.h"

using namespace std;
using namespace segment;

namespace segment
{
	class NameEntityProber
	{       
		public: 
			NameEntityProber();
			~NameEntityProber();
			bool init( const char* conf_file );

			bool probeNameEntity( wordAry& ary,size_t& idx,wordAry& ret );
		public:
			bool _loadDb( const char * db_file,HashFMap& db );
			void _freeDb( HashFMap& db );

			bool _getSurnameScore( wordItem& wd , float& score);
			bool _getSinnameScore( wordItem& wd , float& score);
			bool _getDualFirstScore( wordItem& wd , float& score);
			bool _getDualSecondScore( wordItem& wd , float& score);
			bool _getScore( HashFMap& db ,wordItem& wd , float& score);

			bool _isPossibleSinName( wordItem& second_name , float& s_score);
			bool _isPossibleDualName( wordItem& second_name, wordItem& third_name ,float& s_score );

			bool _mergeResult( float sin_name_score ,float dual_name_score,wordAry& ary , size_t& idx ,wordAry& ret );
		private:
			HashFMap _surname_db;
			HashFMap _dual_first_name_db;
			HashFMap _dual_second_name_db;
			HashFMap _single_name_db;

			float _dual_name_threshold;
			float _sin_name_threshold;
			float _sur_name_threshold;
			float _name_ratio;
	};      	
}

#endif
