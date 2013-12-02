#ifndef __Q_SEGMENT_FAC_H__
#define __Q_SEGMENT_FAC_H__

#include <string>
#include <vector>

using namespace std;

namespace segment
{
	class NameEntityProber;
	class AddressEntityProber;
	class Segment;
	class DoubleHashDict;

	class SegmentFactory
	{
		public:
			static Segment* getInstance( const char *config_file );
			static void destoryInstance();
		private:
			static bool _init( const char *config_file );
			static DoubleHashDict* _loadDict(const string& dict_name );
			static void _releaseResource();
			static void _nullResource();
		private:
			static size_t _ref_count;

			static NameEntityProber *_p_name_prober;
			static AddressEntityProber *_p_address_prober;

			static Segment *_p_segtor;
			static DoubleHashDict* _p_fwd_dict;
			static DoubleHashDict* _p_bwd_dict;
	};
}

#endif
