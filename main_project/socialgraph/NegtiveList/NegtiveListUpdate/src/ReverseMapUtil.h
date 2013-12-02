#ifndef _REVERSE_MAP_UTIL_H_
#define _REVERSE_MAP_UTIL_H_

#include <vector>
#include <map>
#include "socialgraph/socialgraphutil/clogging.h" // for clog

using namespace xce::socialgraph;
namespace reversemap {

//将map的key-value反置
class ReverseMapUtil {
public:
	static void ReverseMap(const std::map<int, std::vector<int> >& resource, std::map<int, std::vector<int> >& destination);
};

}

#endif
