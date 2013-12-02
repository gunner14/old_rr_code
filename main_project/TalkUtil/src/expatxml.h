#ifndef __EXPATXML_H__
#define __EXPATXML_H__
#include <string>
#include <list>
#include <utility>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include "TalkUtil/src/expat/expat.h"

namespace com {
namespace xiaonei {
namespace talk {
namespace util {
	class ExpatTool{
	public:
		bool IsValid(std::string xml);

	};
}

}
}
}
#endif
//__EXPATXML_H__
