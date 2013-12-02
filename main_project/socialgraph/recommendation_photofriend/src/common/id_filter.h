#ifndef ID_FILTER_H_
#define ID_FILTER_H_

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <sstream>

#include "UtilCxx/src/Singleton.h"
#include "base/rwlock.h"

namespace xce {
namespace socialgraph {

#define WHITE_LIST_POSTFIX "whitelist"
#define BLACK_LIST_POSTFIX "blacklist"

typedef std::map<std::string, std::set<int> >	FilterContainer;

//--------------------------------------

class IdFilter : public MyUtil::Singleton<IdFilter> {
 public:
	/*
	@brief get file needed from dir
	@param dir		directory of file list
	*/
	void Initialize(const std::string& dir = "/data/xce/XSocialGraph/sg_data");

	/*
	@brief reload local memory of FilterContainer
	@param dir		directory of file list
	*/
	void Reload(const std::string& dir = "/data/xce/XSocialGraph/sg_data");

	/*
	@brief justify whether id exist in filename
	@param filename		file store with id 
	*/
	bool IsExist(const std::string& filename, int id) const;

	/*
	@brief get id set
	@param filename		file store with id 
	*/
	std::set<int> GetIdSet(const std::string& filename) const;

 private:
	bool LoadFile(const std::vector<std::string>& file_list);

	void LoadIdSet(const std::string& filename, std::set<int>& idset);

	mutable ReadWriteLock lock_;		//use rwlock to get better performance
	FilterContainer container_;
	std::string local_dir_;
};

}
}

#endif
