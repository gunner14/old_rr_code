/*
 * LogManager.cpp
 *
 *  Created on: 2012-2-23
 *      Author: ping.zhang
 */

#include "edm_logger.h"
#include "LogManager.h"

using namespace std;
using namespace IceUtil;
using namespace xce::ad;
using namespace com::xiaonei::xce;
using namespace MyUtil;

void MyUtil::initialize(){
  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&EdmLogManagerI::instance(), service.createIdentity("M", ""));

  InitLogger("edm_send", "../log/" + ServiceI::instance().getName() + "/send_val", "INFO");
  InitLogger("edm_click", "../log/" + ServiceI::instance().getName() + "/click_val", "INFO");
  InitLogger("edm_expose", "../log/" + ServiceI::instance().getName() + "/expose_val", "INFO");


  TaskManager::instance().scheduleRepeated(new EdmLogTimer());

}
void EdmLogManagerI::Expose(const string& log, const Ice::Current& current) {
  EDMEX_INFO(log);
}

void EdmLogManagerI::Click(const string& log, const Ice::Current& current) {
  EDMCK_INFO(log);
}

void EdmLogManagerI::Send(const string& log, const Ice::Current& current) {
  EDMSD_INFO(log);
}
