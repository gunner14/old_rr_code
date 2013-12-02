/*
 * =====================================================================================
 *
 *       Filename:  LuaInterface.h
 *
 *    Description:  Lua类
 *
 *        Version:  1.0
 *        Created:  2012年08月21日 11时15分59秒
 *       Revision:  none
 *
 *         Author:  Duo Zhang (), duo.zhang@renren-inc.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _FEEDNEWS_LUAINTERFACE_H_
#define _FEEDNEWS_LUAINTERFACE_H_

#include "ServiceI.h"
#include "PropertiesManager.h"
extern "C"
{
#include "Lua/lua.h"
#include "Lua/lualib.h"
#include "Lua/lauxlib.h"
}
#include "IceUtil/Mutex.h"
#include <queue>

#define LUA_ARGV_ERROR 1
#define LUA_UNAVAILABLE 2
#define LUA_CALL_ERROR 3

namespace xce {
namespace feed {

class Lua : public UpdaterBase, public MyUtil::Singleton<Lua> {
public:
  Lua() : maxpoolsize_(0), isavailable_(false), curversion_(0), pollfullwarncount(0) {
    PropertiesManager::instance().AppendUpdater(this);
  }

  virtual void Update();

  int CallVA(const char * sig, ...);

private:
  string get_property_script();

  lua_State * get_new_lua_state(const string & script);
  lua_State * get_lua_state(int & version);
  void return_lua_state(lua_State * L, int version);

private:
  IceUtil::Mutex mutex_;
  std::queue<lua_State*> luapool_;
  int maxpoolsize_;
  bool isavailable_;
  int curversion_;
  int pollfullwarncount;
};
/*
class LuaDescriptor : public MyUtil::Descriptor {
protected:
  virtual void configure(const Ice::PropertiesPtr& props) {
    Lua::instance().Update();
  }
};
*/
}
}

#endif
