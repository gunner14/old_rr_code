/*
 * =====================================================================================
 *
 *       Filename:  test.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年05月25日 17时14分16秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include "ad/Degrade/src/Degrade.h"

int main(int argc, char **argv){
  std::string host = argv[1];
  std::string user = argv[2];
  std::string password = argv[3];
  int port = atoi(argv[4]);
  std::string db = argv[5];

  DegradeManager::instance().SetDB(host , user, password, port, db);
  DegradeManager::instance().Register("engine_u");
  DegradeManager::instance().Register("engine_b");

  int ret = DegradeManager::instance().Init();
  if(ret < 0){
    printf("DegradeManager Init Failed\n");
    return -1;
  }
  
  while(true){
    if(DegradeManager::instance().IsEnable("engine_u")){
      printf("EngineU is enable.\n");
    }else{
      printf("EngineU is disable.\n");
    }
    sleep(5);
  }

  return 0;
}
