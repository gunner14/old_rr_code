#include "id_filter.h"

#include <dirent.h>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/program_options.hpp>

#include "UtilCxx/src/LogMonitor.h"
#include "UtilCxx/src/LogWrapper.h"

namespace xce {
namespace socialgraph {

void IdFilter::Initialize(const std::string& dir) {
	std::string local_dir = dir;
	if (!boost::ends_with(local_dir, "/")) {
		local_dir += "/";	
	}	
	DIR *dp = opendir(local_dir.c_str());			//open dir
	if (NULL == dp) return;

	local_dir_ = local_dir;				//set local_dir_

	std::vector<std::string> file_list;
	struct dirent *dirp;
	while ((dirp = readdir(dp)) != NULL) {			//read file all end with whitelist or blacklist postfix
		if (boost::ends_with(dirp->d_name, WHITE_LIST_POSTFIX) ||
				boost::ends_with(dirp->d_name, BLACK_LIST_POSTFIX)) {
			file_list.push_back(dirp->d_name);
		}
	}
	
	if (closedir(dp) < 0)
		MCE_WARN("can't close directory : " << local_dir);

	LoadFile(file_list);
}

void IdFilter::Reload(const std::string& dir) {
	Initialize(dir);						//call Initialize
}

bool IdFilter::LoadFile(const std::vector<std::string>& file_list) {
	FilterContainer container;

	for (std::vector<std::string>::const_iterator iter = file_list.begin();
			iter != file_list.end(); ++iter) {
		std::set<int> idset;
		LoadIdSet(*iter, idset);
		container.insert(make_pair<std::string, std::set<int> >(*iter, idset));
	}

	lock_.WriteLock();

	container_.swap(container);					//initialize local container

	lock_.WriteUnlock();

	return true;
}

bool IdFilter::IsExist(const std::string& filename, int id) const {
	lock_.ReadLock();
	
	FilterContainer::const_iterator fit = container_.find(filename);
	if (fit == container_.end()) {
		lock_.ReadUnlock();
		return false;
	}
	std::set<int>::const_iterator ffit = fit->second.find(id);
	if (ffit == fit->second.end()) {
		lock_.ReadUnlock();
		return false;
	}

	lock_.ReadUnlock();
	return true;
}

std::set<int> IdFilter::GetIdSet(const std::string& filename) const {
	lock_.ReadLock();
	FilterContainer::const_iterator fit = container_.find(filename);
	if (fit != container_.end()) {
		lock_.ReadUnlock();
		return fit->second;
	}

	lock_.ReadUnlock();
	return std::set<int>();
}

void IdFilter::LoadIdSet(const std::string& filename, std::set<int>& idset) {
	std::string current_file = local_dir_ + filename;

  boost::filesystem::path p(current_file);
  if (exists(p)) {							//whether file exist
    std::ifstream infile(current_file.c_str(), std::ifstream::in);
    std::string line = "";
    while (getline(infile, line)) {
      int id = 0;
      std::istringstream stream(line);
			try {
      	stream >> id;
      	if (0 != id) {
      	  idset.insert(id);
      	}
			} catch (...) {
				MCE_WARN("parser error current_file:" << current_file);
			}
    }
		infile.close();
  } else {
		MCE_WARN(current_file << " do not exists!!");
  }
}

}
}
