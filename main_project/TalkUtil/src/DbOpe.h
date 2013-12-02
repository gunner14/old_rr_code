#ifndef __TALKUTIL_DBOPE__
#define __TALKUTIL_DBOPE__


namespace com {
namespace xiaonei {
namespace talk {
namespace util {

class DbOpe {
  static bool dosql(const char *inst, int wrflag, const char *route, const char *sql, mysqlpp::StoreQueryResult *res = NULL);
  static void DbTableHash(const char *pref, int count, long mod, int len, char *name);
 public:
  static bool SqlRead(const char *inst, const char *route, const char *sql, mysqlpp::StoreQueryResult &res);
  static bool SqlWrite(const char *inst, const char *route, const char *sql);
  static string GetDbTable(const char *pref, int count, long mod);
  static string GetDbRoute(const char *pref, int count, long mod);

};


}
}
}
}

#endif
