#ifndef __Q_DOUBLE_HASH_DICT_H__
#define __Q_DOUBLE_HASH_DICT_H__

#include "container.h"

using namespace std;
using namespace segment;

namespace segment
{
	class DoubleHashDict
	{
		public:
			DoubleHashDict();
			~DoubleHashDict();
			
			int bulidPreHashDictFromTxt( const char* txt_file );
			int bulidProHashDictFromTxt( const char* txt_file );
			bool saveHashDict( const char* save_file );
	
			bool loadHashDict( const char* dict_file );
			bool preMatchMaxWord(const char* buf,size_t buf_len,size_t& wd_len);
			bool proMatchMaxWord(const char* buf,size_t buf_len,size_t& wd_len);
			size_t size(){ return _hash_db.size(); }
		private:
			int _bulidHashDictFromTxt( const char* txt_file , bool is_pro);
			bool _insertNewKey(const string& primary_key,const string& second_key,StrAry& value_ary );
			bool _prematchMaxWord(const char *str ,size_t len ,StrAry& value_ary,size_t& wd_len);
			bool _promatchMaxWord(const char *str ,size_t len ,StrAry& value_ary,size_t& wd_len);
			void _sortHashDict();
		private:
			DoubleHashMap _hash_db; 
	};

}

#endif
