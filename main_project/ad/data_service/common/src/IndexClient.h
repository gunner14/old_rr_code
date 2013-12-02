/*
 * =====================================================================================
 *
 *       Filename:  t.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年08月23日 10时32分27秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef __INDEX_CLIENT_H__
#define __INDEX_CLIENT_H__

#define NONE_ACTION -1
#define READ_HEAD 1
#define READ_BODY 2
#define BODY_COMPLETED 3

#include <vector>
#include <list>
#include <set>
#include <iostream>
#include <IceUtil/RWRecMutex.h>
#include <IceUtil/Monitor.h>
#include <IceUtil/Mutex.h>
#include <IceUtil/Thread.h>
#include "Singleton.h"

namespace xce{
namespace ad{
class IndexClient : public IceUtil::Thread,public MyUtil::Singleton<IndexClient>{
  public:
    IndexClient(): sock_(-1){}

    int PreProcessBase(const std::string &tn, const std::string &filename);
    void TraceConnect(const string& ip, int port, const std::string &e);
    void Close();
    int Reconnect();
    int Connect (); 
		void Initialize(string ip,int port,int timeout);
    virtual void run();
    int PreProcess();
    int ProcessHead();
    int ProcessBody();
    int Process();
    int ProcessIncrement();
    int ProcessHeartBeat();
    int ProcessSnapshot();
    int ProcessServerList();
    int ProcessIncReady();
    int StartSync();
    int GetServerList();
		void GetServerInfo(std::string & ip, int & port);
		void SetCurrentInfo(std::string ip, int port,int timeout){
			current_server_ip_ = ip;
			current_server_port_ = port;
			timeout_ = timeout;
		}

  private:

    struct Packet{
      Packet(){
        Reset();
        body_ = new char[20480000];
      }
      ~Packet(){
        delete [] body_;
      }
      void Reset(){
        ver_ = -1;
        type_ = -1;
        len_ = -1;
        read_ = 0;
        status_ = NONE_ACTION;
      }
      int8_t status_;
      int8_t ver_;
      int16_t type_;
      int32_t len_;
      int32_t read_;
      timeval end_time_;
      char *body_;
    };

    Packet pack_;
    int sock_;
		bool reconnected_;
		int64_t inc_endtime_;
		int64_t inc_starttime_;
		int timeout_;
		int current_server_port_;
		std::string current_server_ip_;
		vector<std::string> server_list_;	
		std::vector<std::string> RegisterTables_;
		IceUtil::RWRecMutex lock_; 
};
}
}

#endif
