#ifndef TRIPOD_COMMON_H_
#define TRIPOD_COMMON_H_

#include <string>
#include <list>
#include <map>

#include "base/consistenthash.h"

namespace xce {
namespace tripod {

typedef std::list<std::string> StringListType;
typedef std::map<std::string, std::string> StringMapType;

typedef boost::shared_ptr<xce::Continuum> ContinuumPtr;

static const std::string kVersion = "#2.1.1";

const std::string GetVersion();


}} // xce::tripod

#endif

