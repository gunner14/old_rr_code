#include "QueryRunner.h"


#include "DbOpe.h"

using namespace com::xiaonei::xce;
using namespace com::xiaonei::talk::util;


bool DbOpe::dosql(const char *inst, int wrflag, const char *route, const char *sql, mysqlpp::StoreQueryResult *res) {
  bool rv = false;
	try {
    Statement stsql;
    stsql << sql;

    if (CDbRServer == wrflag) {
      *res = QueryRunner(inst, wrflag, route).store(stsql);
      if (*res) rv = true;
    } else {
      QueryRunner(inst, wrflag, route).store(stsql);
      rv = true;
    }



    MCE_INFO("DbOpe::dosql --> inst:" << inst <<
             " wrflag:" << wrflag << " route:" << route <<
             " sql:" << sql << " result:" << rv);



	} catch(Ice::Exception &e) {
		MCE_WARN("DbOpe::dosql --> Exception err:" << e << " inst:" << inst <<
             " wrflag:" << wrflag << " route:" << route <<
             " sql:" << sql);
	} catch (std::exception &e) {
		MCE_WARN("DbOpe::dosql --> exception err:" << e.what() << " inst:" << inst <<
             " wrflag:" << wrflag << " route:" << route <<
             " sql:" << sql);
	} catch (...) {
    MCE_WARN("DbOpe::dosql --> Unkown exception err:" << " inst:" << inst <<
             " wrflag:" << wrflag << " route:" << route <<
             " sql:" << sql);
	}

	return rv;

}

void DbOpe::DbTableHash(const char *pref, int count, long mod, int len, char *name) {
  if (1 == count) {
    snprintf(name, len, "%s", pref);
  } else {
    int hash = (int)(mod % count);
    snprintf(name, len, "%s_%d", pref, hash);
  }
}

///////////////////////////////////////////////////////////

bool DbOpe::SqlRead(const char *inst, const char *route, const char *sql, mysqlpp::StoreQueryResult &res) {
  return DbOpe::dosql(inst, CDbRServer, route, sql, &res);
}

bool DbOpe::SqlWrite(const char *inst, const char *route, const char *sql) {
  return DbOpe::dosql(inst, CDbWServer, route, sql);
}


string DbOpe::GetDbTable(const char *pref, int count, long mod) {
  char buff[200];
  DbOpe::DbTableHash(pref, count, mod, sizeof(buff), buff);
  return buff;
}

string DbOpe::GetDbRoute(const char *pref, int count, long mod) {
  char buff[200];
  DbOpe::DbTableHash(pref, count, mod, sizeof(buff), buff);
  return buff;

}


