#ifndef _QATA_CONFIG_H_
#define _QATA_CONFIG_H_

#include <map>
#include <string>
#include <iostream>

using namespace std;

typedef map<string,string> StrMap;
typedef map<string,string>::iterator StrMapIter;
typedef pair<string,string> StrPair;

class QataConfig
{
	public:
		QataConfig();
		bool loadConfig(const char *filename);
		int Size();
	    	bool isKeyExists(const string& key);
		string operator [](const string& key);
		StrPair operator [](int idx);
	private:
	    bool _isKeyExists(const string& key);
		string _getValue(const string& key);
		bool _addToMap(const string& key,const string& value);
		bool _convert_var_str(const string& src_str,string& ret_str);

		bool _type_checking(const string& src_str,string& ret_str);
		bool _parse_check_flag(const string& src_str,string& ret_str,string& flag);
		bool _parse_bracket(const string& src_str,string& ret_str,size_t& start_idx);

		bool _set_options(const string& str );
		bool _add_file_include( const char *file_name );
	
	private:
		StrMap _config_db;
		bool _is_typecheck_on;
		bool _is_block_comment_on;
};

static void dumpQataConfig(QataConfig& config) 
{
	for(int i = 0 ; i < config.Size();i++)
	{
		StrPair  item = config[i];
	//	cout<<"dumpQataConfig for "<<i<<" item:: key:"<<item.first<<"\tvalue:"<<item.second<<endl;
	}
}


#endif
