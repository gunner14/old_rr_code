/******************************************************************************
Function:  SocialGraph mysqlpp database relevant operations
Developer: xiongjunwu
Email:     junwu.xiong@renren-inc.com
Date:      2012-03-21
*******************************************************************************/
#ifndef __MYSQLPP_UTIL_H_
#define __MYSQLPP_UTIL_H_

#include <sstream>
#include <string>

namespace MysqlppUtil {
//return sub table name or table pattern of database tables 
inline std::string SubTableName(const std::string& bizName, const int id) {
  ostringstream oss;
  oss << bizName << "_" << (id % 100);
  return oss.str();
}

};
#endif
