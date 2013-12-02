/*
 * =====================================================================================
 *
 *       Filename:  LuaInterface.cpp
 *
 *    Description:  实现
 *
 *        Version:  1.0
 *        Created:  2012年08月22日 15时29分33秒
 *       Revision:  none
 *
 *         Author:  Duo Zhang (), duo.zhang@renren-inc.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "LuaInterface.h"

#define FUNCTION_NAME "GetScore"
#define POOL_SIZE_LIMIT 200

using namespace xce::feed;
using namespace MyUtil;

void Lua::Update() {
  if (!get_new_lua_state(get_property_script())) {
    MCE_WARN("Lua::Update -> Update Fail!!");
    return ;
  }
  IceUtil::Mutex::Lock lock(mutex_);
  while(!luapool_.empty()) {
    lua_State * L = luapool_.front();
    if (L) {
      lua_close(L);
    }
    luapool_.pop();
    maxpoolsize_--;
  }
  curversion_++;
  MCE_INFO("Lua Class is available!!");
  isavailable_ = true;
}

int Lua::CallVA(const char * sig, ...) {
  if (sig == 0) {
    return LUA_ARGV_ERROR;
  }

  if (!isavailable_) {
    return LUA_UNAVAILABLE;
  }

  int version = 0;
  lua_State * L = get_lua_state(version);
  if (!L) {
    return LUA_UNAVAILABLE;
  }

  lua_getglobal(L, FUNCTION_NAME);

  int narg = 0, nres = 0;
  va_list vl;
  va_start(vl, sig);

  while(*sig) {
    if(*sig == 'd') {
      lua_pushnumber(L, va_arg(vl, double));
    } else if(*sig == 'i') {
      lua_pushinteger(L, va_arg(vl, int));
    } else if(*sig == 'b') {
      lua_pushboolean(L, va_arg(vl, int));
    } else if(*sig == 's') {
      lua_pushstring(L, va_arg(vl, char *));
    } else if(*sig == '>') {
      sig++;
      break;
    } else {
      va_arg(vl, void *);
      lua_pushnil(L);
    }
    sig++;
    narg++;
    luaL_checkstack(L, 1, "to many arguments");
  }

  nres = strlen(sig);
  if(lua_pcall(L, narg, nres, 0) != 0) {
    MCE_WARN("Lua::CallVA -> error calling " << FUNCTION_NAME << ": " << lua_tostring(L, -1));
    va_end(vl);
    lua_pop(L, 1);
    return_lua_state(L, version);

    return LUA_CALL_ERROR;
  }

  nres = -nres;
  while(*sig) {
    switch(*sig++) {
      case 'd': 
        if(lua_isnumber(L, nres)) {
          *va_arg(vl, double *) = lua_tonumber(L, nres);
        }
        break;
      case 'b': 
        if(lua_isboolean(L, nres)) {
          *va_arg(vl, bool *) = lua_toboolean(L, nres);
        }
        break;
      case 'i':
        if(lua_isnumber(L, nres)) {
          *va_arg(vl, int *) = lua_tointeger(L, nres);
        }
        break;
      case 's':
        if(lua_isstring(L, nres)) {
          *va_arg(vl, const char **) = lua_tostring(L, nres);
        }
        break;
    }
    nres++;
  }

  va_end(vl);
  lua_pop(L, 1);
  return_lua_state(L, version);

  return 0;
}

inline string Lua::get_property_script() {
  ServiceI & service = ServiceI::instance();
  ostringstream key;
  key << "Service." << service.getBusiness() << ".LuaScript.GetScore";
  return ServiceI::instance().getCommunicator()->getProperties()->getPropertyWithDefault(key.str(), "");
}

lua_State * Lua::get_new_lua_state(const string & script) {
  if (script == "") {
    MCE_WARN("Lua::get_new_lua_state -> String Script is Null. Lua Class is unavailable!!");
    isavailable_ = false;
    return 0;
  }
  lua_State * L = luaL_newstate();
  if (!L) {
    MCE_WARN("Lua::get_new_lua_state -> luaL_newstate fail!");
    return 0;
  }
  luaL_openlibs(L);
  if (luaL_dostring(L, script.c_str())) {
    MCE_WARN("Lua::get_new_lua_state -> luaL_dostring Error: " << lua_tostring(L, -1) << " Lua Class is unavailable!!");
    lua_close(L);
    isavailable_ = false;
    return 0;
  }
  return L;
}

lua_State * Lua::get_lua_state(int & version) {
  lua_State * L = 0;
  IceUtil::Mutex::Lock lock(mutex_);
  if (!luapool_.empty()) {
    L = luapool_.front();
    luapool_.pop();
    version = curversion_;
    return L;
  }
  if (maxpoolsize_ > POOL_SIZE_LIMIT) {
    pollfullwarncount++;
    if (pollfullwarncount > 10000) {
      MCE_WARN("Lua::get_lua_state maxpoolsize_ > " << POOL_SIZE_LIMIT << "! WARN count: " << pollfullwarncount);
      pollfullwarncount = 0;
    }
    return 0;
  }
  L = get_new_lua_state(get_property_script());
  if (L) {
    maxpoolsize_++;
    MCE_INFO("Lua::get_lua_state luapool_ is empty. Add a new lua state. new size: " << maxpoolsize_);
    version = curversion_;
  }
  return L;
}

void Lua::return_lua_state(lua_State * L, int version) {
  if (!L) {
    return ;
  }
  IceUtil::Mutex::Lock lock(mutex_);
  if (version == curversion_) {
    luapool_.push(L);
  } else {
    maxpoolsize_--;
    lua_close(L);
  }
}
