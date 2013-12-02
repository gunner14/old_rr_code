#ifndef DATA_DICT_H_
#define DATA_DICT_H_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <map>

#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

#include "UtilCxx/src/Singleton.h"
#include "base/rwlock.h"

namespace xce {
namespace socialgraph {

/*
@brief DataDict storing small data needed in main process
*/
template <typename K, typename V>
class DataDict {
 public:
	DataDict() {}
	virtual ~DataDict() {}

	/*
	@brief initialize data map from target file
	@param file target file
	@param key_field key in target file
	@param value_field value in target file
	@param token segmentation in target file
	*/
	bool Initialize(const std::string& file, 
									int key_field, 
									int value_field, 
									const std::string token = " ") {
		std::ifstream infile(file.c_str(), std::ifstream::in);
		if (!infile.is_open()) {
			std::cerr << "Open file error!" << std::endl;
			return false;
		}

		std::string line = "";
		std::vector<std::string> item_list;

		while (getline(infile, line)) {
			boost::split(item_list, line, boost::is_any_of(token));
			if ((int)item_list.size() > key_field && 
					(int)item_list.size() > value_field) {
				InsertObject(item_list.at(key_field), item_list.at(value_field));			//insert object into object map
			}
			item_list.clear();
		}

		return true;
	}

	/*
	@brief Get data map
	*/
	std::map<K, V>* GetInstance() {
		return &object_map_;
	}

 private:
	void InsertObject(const std::string& key, const std::string& value) {
		K r_key;
		V r_value;
		try {
			r_key = boost::lexical_cast<K>(key);
			r_value = boost::lexical_cast<V>(value);
		} catch (boost::bad_lexical_cast& e) {
			std::cerr << "error:" << e.what() << std::endl;
		}

		object_map_.insert(std::make_pair<K, V>(r_key, r_value));
	}

	std::map<K, V> object_map_;
};

}}

#endif
