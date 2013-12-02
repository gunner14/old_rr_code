#ifndef SORT_UTIL_H_
#define SORT_UTIL_H_

#include <vector>
#include <iostream>
#include <map>

#include "PhotoStateFilterAdapter.h"

namespace xce {
namespace socialgraph {

class SortUtil {
 public:
	SortUtil();
	static void FilterUserPhoto(std::vector<int>& idlist);
  static void RankByUserValue(std::vector<int>& idlist);

 private:

};

}
}

#endif
