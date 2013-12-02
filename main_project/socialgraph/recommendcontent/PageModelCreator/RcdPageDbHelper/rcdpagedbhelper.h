/*
  Author: yize.tan
  Date : 2012-11-28
  Description : get the page & user friends information from the db
*/

#ifndef __RCD_PAGE_DB_HELPER__
#define __RCD_PAGE_DB_HELPER__

#include <iostream>
#include <vector>
#include <string>
#include <boost/lexical_cast.hpp>
#include "DbCxxPool/src/QueryRunner.h"

namespace xce {
namespace socialgraph {

class RcdPageDbHelper {
public:
  static void getUserPageFromDb(int user_id, std::set<int>& page_ids);
};

};
};

#endif
