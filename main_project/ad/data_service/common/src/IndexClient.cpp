#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/select.h>
#include <fcntl.h>
#include "aaa.pb.h"
#include "file_reader.h"
#include "IndexClient.h"
#include "IndexLoader.h"
#include "IndexItem.h"
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <boost/algorithm/string/regex.hpp>
using namespace std;
using namespace xce::ad;

void IndexClient::Initialize(string ip,int port,int timeout){
	SetCurrentInfo(ip,port,timeout);
	RegisterTables_.push_back("jebe_main.member");
	RegisterTables_.push_back("jebe_ads.campaign");
	RegisterTables_.push_back("jebe_ads.adgroup");
	RegisterTables_.push_back("jebe_ads.creative");
	RegisterTables_.push_back("jebe_ads.adgroup_adzone_ref");
	RegisterTables_.push_back("jebe_ads.bid_unit");
	IndexPreprocessor::instance().RegisterTable(RegisterTables_);
}

void IndexClient::TraceConnect(const string& ip, int port, const std::string &e){
  MCE_WARN("IndexClient::TraceConnect connect to " << ip << " " << port << " Error( " << e << ")");
  return;
}

void IndexClient::Close(){
	try{
		if(sock_ >= 0){
			close(sock_);
			sock_ = -1;
		}
	}catch(const std::exception &e){
		MCE_WARN("IndexClient::Close ,caught exception:" << e.what());
	}
}

int IndexClient::Reconnect(){
  Close(); 
	while(true){
		if(Connect() == -1){
			GetServerInfo(current_server_ip_,current_server_port_);		
		}else{
			MCE_WARN("IndexClient::Reconnect to " << current_server_ip_ << " " << current_server_port_);
			reconnected_ = true;
			break;
		}
	}
	return 0;
}

int IndexClient::Connect () {
	try{
		sock_ = socket(AF_INET, SOCK_STREAM, 0);
		if(sock_ < 0){
			TraceConnect(current_server_ip_.c_str(), current_server_port_, strerror(errno));
			return -1;
		}

		struct  sockaddr_in	serv_addr;
		fd_set		fdvar_write; 
		struct timeval	tv;
		int			n, optval, size;

		tv.tv_sec = 0; 
		tv.tv_usec = 1000 * timeout_;
		memset((char *)&serv_addr, 0, sizeof(struct sockaddr_in));
		serv_addr.sin_addr.s_addr = inet_addr(current_server_ip_.c_str());
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(current_server_port_);

		if (fcntl(sock_, F_SETFL, O_NONBLOCK) == -1) {
			TraceConnect(current_server_ip_, current_server_port_, strerror(errno));
			goto FAILED;
		}

		if ((connect(sock_, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in))) != 0) {
			if (errno != EINPROGRESS) {
				TraceConnect(current_server_ip_, current_server_port_, strerror(errno));
				goto FAILED;
			}
		}

		FD_ZERO (&fdvar_write);
		FD_SET(sock_, &fdvar_write);
		n = select (FD_SETSIZE, NULL, &fdvar_write, NULL, &tv);

		if(n == 0){
			TraceConnect(current_server_ip_, current_server_port_, "TimeOut");
			goto FAILED;
		}

		if(n < 0){
			TraceConnect(current_server_ip_, current_server_port_, strerror(errno));
			goto FAILED;
		}

		if (!FD_ISSET (sock_, &fdvar_write)){
			TraceConnect(current_server_ip_, current_server_port_, strerror(errno));
			goto FAILED;
		}else {
			int val;
			socklen_t len = sizeof(val);
			size = sizeof(optval);
			n = getsockopt(sock_, SOL_SOCKET, SO_ERROR, (char *) &val, &len);
			if (n < 0){ 
				TraceConnect(current_server_ip_, current_server_port_, strerror(errno));
				goto FAILED;
			}
			if (!val){
				MCE_INFO("IndexClient::Connect, Connect to " << current_server_ip_<< " " << current_server_port_<< " Success!");
				return 0;
			}else{
				TraceConnect(current_server_ip_, current_server_port_, strerror(errno));
				goto FAILED;
			}                
		}
	}catch(const std::exception &e){
		MCE_WARN("IndexClient::Connect,caught exception:" << e.what());
	}
FAILED:
	Close();
  return -1;
}

void IndexClient::run() {
	MCE_INFO("IndexClient::run Start...");
	if(Connect() == -1){
		MCE_WARN("IndexClient::run ,Connect to Server Failed,"<< current_server_ip_ << " " << current_server_port_ );
		return ;
	}
	if(GetServerList() == -1){
		MCE_WARN("IndexClient::run ,GetServerList Failed");
	}
	if(StartSync() == -1){
		MCE_WARN("IndexClient::run StartSync Failed");
	}
  fd_set    fdvar_read;
  struct timeval	tv;
  for(;;){
    while(sock_ < 0){
      Reconnect();
			MCE_INFO("IndexClient::run , Reconnect to Server, " << current_server_ip_ << " " << current_server_port_); 
			if(StartSync() == -1){
				MCE_WARN("IndexClient::run StartSync Failed");
			}
    }
    FD_ZERO (&fdvar_read);
    FD_SET(sock_, &fdvar_read);

    if(pack_.status_ > NONE_ACTION){
      struct timeval now;
      gettimeofday(&now, 0);
      if (timercmp(&now, &pack_.end_time_, >)){
        MCE_WARN("IndexClient::run ,socket timeout!");
        Close();
        pack_.Reset();
        continue;
      }else{
        timersub(&pack_.end_time_, &now, &tv); 
        //printf("before select tv.tv_sec:%d, tv.tv_usec:%d\n", tv.tv_sec, tv.tv_usec);
      }
    }else{
      tv.tv_sec = 0;
      tv.tv_usec = 1000 * 500;
    }

    int ret = select (FD_SETSIZE, &fdvar_read, NULL, NULL, &tv);
    if(ret < 0){
      MCE_WARN("IndexClient::run ,select return ERROR:"<< strerror(errno));
      continue;
    }
    if(ret == 0){
      continue;
    }

    if(!FD_ISSET(sock_, &fdvar_read)){
      MCE_WARN("IndexClient::run, select return ERROR(" << strerror(errno) << ")");      
      continue;
    }

    switch(pack_.status_){
      case NONE_ACTION:
        ret = ProcessHead();
        break;
      case READ_BODY:
        ret = ProcessBody();
        break;
      default:
        MCE_WARN("IndexClient::run ,ERROR Request,close sockey, Request TYPE:"<< pack_.type_);
        Close();
        pack_.Reset();
        continue;
    }
    if(ret == 0 && pack_.status_ == BODY_COMPLETED){
      Process();
      pack_.Reset();
    }
  }
}

int IndexClient::ProcessHead(){
	try{
		struct timeval t, now;
		t.tv_sec = 50;
		t.tv_usec = 0;
		gettimeofday(&now, 0);
		timeradd(&now, &t, &pack_.end_time_);

		char head[8];
		int ret = read(sock_, head, 8);
		if(ret != 8){
			MCE_WARN("IndexClient::ProcessHead ,Read ERROR:" << strerror(errno));
			goto FAILED;
		}

		uint16_t type;
		uint32_t len;
		pack_.ver_ = head[0];
		memcpy((char*)&(type), head + 2, 2);
		memcpy((char*)&(len), head + 4, 4);
		pack_.type_ = ntohs(type);
		pack_.len_ = ntohl(len);

		//MCE_DEBUG("IndexClient::ProcessHead,Ver:" << pack_.ver_ << ", type:" << pack_.type_ << ", len:" << pack_.len_);

		if(pack_.ver_ != 1){
			MCE_WARN("IndexClient::ProcessHead ,wrong version:" << pack_.ver_);
			goto FAILED;
		}

		if(pack_.type_ < 0 || pack_.len_ < 0 || pack_.len_ >= 20480000){
			MCE_WARN("IndexClient::ProcessHead,ERROR, type:" << pack_.type_ << ", pack_.len_:" << pack_.len_);
			goto FAILED;
		}else{
			if(pack_.type_ == 0){
				pack_.status_ = BODY_COMPLETED;
			}else{
				pack_.status_ = READ_BODY;
			}
			return 0;
		}
	}catch(const std::exception &e){
		MCE_WARN("IndexClient::ProcessBody ,caught exception:" << e.what());
		return -1;
	}
FAILED:
	MCE_WARN("IndexClient::ProcessHead,Error ,Close it");
	Close();
	pack_.Reset();
	return -1;
}

int IndexClient::ProcessBody(){
	//printf("begin read body: start:%d, len:%d\n", pack_.read_, pack_.len_ - pack_.read_);
	try{
		int ret = read(sock_, pack_.body_ + pack_.read_, pack_.len_ - pack_.read_);
		if(ret == pack_.len_ - pack_.read_){
			pack_.status_ = BODY_COMPLETED;
			pack_.body_[pack_.len_] = '\0';
			return 0;
		}
		switch(ret){
			case -1:
				if(errno != EINTR){
					MCE_WARN("Read body error, len:" << pack_.len_ << " , errono:" << strerror(errno));
					goto FAILED;
				}else{
					pack_.status_ = READ_BODY;
					return 0;
				}
			case 0:
				MCE_INFO("Peer close\n");
				goto FAILED;
			default:
				//printf("read size:%d, total:%d\n", ret, pack_.len_);
				pack_.read_ += ret;
				pack_.status_ = READ_BODY;
				return 0;
		}
	}catch(const std::exception &e){
		MCE_WARN("IndexClient::ProcessBody ,caught exception:" << e.what());
		return -1;
	}
FAILED:
	MCE_WARN("IndexClient::ProcessBody, Error, close it");
	Close();
	pack_.Reset();
	return -1;
}


int IndexClient::Process(){
  switch(pack_.type_){
    case 0:
      ProcessHeartBeat();
      break;
    case 2:
      ProcessSnapshot();
      break;
    case 3:
      ProcessIncrement();
      break;
    case 4:
      ProcessServerList();
      break;
    case 6:
      ProcessIncReady();

    default:
      //MCE_WARN("Process, error type:" << type);
      return -1;

    return 0;
      
  }  

  return 0;
}

int IndexClient::ProcessIncrement(){
	try{
		follower::DiffPackage inc_pack;
		inc_pack.ParseFromString(pack_.body_); 
		inc_starttime_ = inc_pack.starttime();
		inc_endtime_ = inc_pack.endtime();
		std::string table = inc_pack.tablename();
		MCE_INFO("IndexClient::ProcessIncrement ,inc_pack.data_size:" << inc_pack.data_size() << ", table:" << inc_pack.tablename() << ", start_time:" << inc_starttime_ << ", end_time:" << inc_endtime_); 
		for(int i = 0; i < inc_pack.data_size(); ++i){
			IndexItem item(table);
			const follower::DiffPackage_Row& row = inc_pack.data(i);
			item.fields.push_back(row.key());
			for(int j = 0; j < row.values_size(); ++j){
				item.fields.push_back(row.values(j));
			}
			IndexPreprocessor::instance().AddIncItem(item);
		}
	}catch(const std::exception &e){
		MCE_WARN("IndexClient::ProcessIncrement ,caught exception:" << e.what());
		return -1;
	}
	return 0;
}

int IndexClient::ProcessHeartBeat(){
	return 0;
}


int IndexClient::ProcessSnapshot(){
	try{
		follower::Snapshot ss;
		ss.ParseFromString(pack_.body_);
		MCE_INFO("IndexClient::ProcessSnapshot,ProcessSnapshot, table:" << ss.tablename() << ", snapshot:" <<  ss.snapshotpath());
		IndexPreprocessor::instance().PreprocessBaseIndex(ss.tablename(), ss.snapshotpath());
	}catch(const std::exception &e){
		MCE_WARN("IndexClient::ProcessIncrement,caught exception:" << e.what());
		return -1;
	}  
  return 0;
}

int IndexClient::ProcessIncReady(){
  std::string table_name(pack_.body_, pack_.len_);
  IndexPreprocessor::instance().SetIncReady(table_name);
  return 0;
}

int IndexClient::ProcessServerList(){
	try{
		vector<std::string> server_list;
		follower::ServerList sl;
		sl.ParseFromString(pack_.body_); 
		MCE_INFO("IndexClient::ProcessServerList,ServerList.size: " <<  sl.server_size());
		for(int i = 0; i < sl.server_size(); ++i){
			std::string s = sl.server(i);
			server_list.push_back(s);
			MCE_INFO("IndexClient::ProcessServerList,Add Server :" << s);
		}
		{
			IceUtil::RWRecMutex::WLock wlock(lock_);
			server_list_.swap(server_list);		
		}
	}catch(const std::exception &e){
		MCE_WARN("IndexClient::ProcessServerList,caught exception:" << e.what());
		return -1;
	} 
  return 0;
}

void IndexClient::GetServerInfo(std::string & ip, int & port){
	IceUtil::RWRecMutex::RLock rlock(lock_);
	if(server_list_.size() <= 0 ){
		MCE_WARN("IndexClient::GetAndParseServer No Server");
		return ;
	}
	while(true){
		int randnum = rand()% server_list_.size() ;	
		std::string strServer = server_list_[randnum];
		vector<string> result;
		boost::algorithm::split(result, strServer, boost::is_any_of(":"));
		if(result.size() != 2){
			continue;
		}else{
			ip = result.at(0);
			port = boost::lexical_cast<int>(result.at(1));
			MCE_INFO("IndexClient::GetAndParseServer Success,ip:"<< ip << " ,port:" << port);
			break;
		}
	}
}

int IndexClient::StartSync(){
	try{
		std::string serialized;
		follower::StartSync ss;
		for(std::vector<std::string>::const_iterator it = RegisterTables_.begin(); it != RegisterTables_.end(); ++it){
			const std::string &tn = *it;
			follower::SyncItem *si = ss.add_items();
			si->set_tablename(tn);
			if(reconnected_){
				si->set_endtime(inc_endtime_);
				reconnected_ = false;
			}else{
				si->set_endtime(0);
			}
		}
		ss.SerializeToString(&serialized);

		char *buf = new char[8 + serialized.size()];
		buf[0] = 0x1;
		buf[1] = 0x0;
		uint16_t type = htons(1);
		uint32_t len = htonl(serialized.size());
		memcpy(buf+2, (unsigned char*)(&type), 2);
		memcpy(buf+4, (unsigned char*)(&len), 4);
		memcpy(buf+8, serialized.c_str(), serialized.size());
		int sz = serialized.size() + 8;
		//printf("type:%u, len:%u, serialized_len:%u\n", type, len, serialized.size());
		int ret = write(sock_, buf, sz);
		if(ret < 0){
			MCE_WARN("IndexClient::StartSync, send error:" <<  strerror(errno));
			Close();
			pack_.Reset();
			return -1;
		}
		delete []buf;
	}catch(const std::exception &e){
		MCE_WARN("IndexClient::StartSync,caught exception:" << e.what());
		return -1;
	}
	return 0;
}

int IndexClient::GetServerList(){
	try{
		char *buf = new char[8];
		buf[0] = 0x1;
		buf[1] = 0x0;
		uint16_t type = htons(5);
		uint32_t len = htonl(0);
		memcpy(buf+2, (unsigned char*)(&type), 2);
		memcpy(buf+4, (unsigned char*)(&len), 4);
		int ret = write(sock_, buf, 8);
		MCE_DEBUG("IndexClient::GetServerList,type:" << type << " ,buf:"<< string(buf) << " ,Ret:" << ret);
		if(ret < 0){
			MCE_WARN("IndexClient::GetServerList ,send error:" <<  strerror(errno));
			Close();
			pack_.Reset();
			return -1;
		}
		delete []buf;
	}catch(const std::exception &e){
		MCE_WARN("IndexClient::GetServerList,caught exception:" << e.what());
		return -1;
	}
	return 0;
}

