/**
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#ifndef _KAFKACLIENT_
#define _KAFKACLIENT_
#include <exception>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>

#include <boost/algorithm/string.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

#include "producer.hpp"
#include "LogWrapper.h"
#define foreach BOOST_FOREACH

using namespace MyUtil;
using namespace boost;
using namespace kafkaconnect;


class KafkaClient: public boost::enable_shared_from_this<KafkaClient>{

	int index_;
	std::string host_;
	std::string port_;
	boost::shared_ptr<kafkaconnect::producer> prd_;
	boost::shared_ptr<boost::thread> bt_;
	boost::asio::io_service ios_;
	boost::shared_ptr<boost::asio::io_service::work> work_;
public:
	string getHost()const{ return host_;}
	string getPort()const{ return port_;}

	KafkaClient(int index,const  string & host,const string & port):index_(index),host_(host),port_(port){
	}
	virtual ~KafkaClient(){
		cout << "dtor KafkaClient begin #" << endl;
		if(prd_){
			prd_.reset();
		}
		ios_.stop();
		//bt_.reset();	
		//work_.reset();
		cout << "dtor KafkaClient end" << endl;
	}
	bool checkConnect(){
		cout << "begin" << endl;
		if(prd_  && prd_->is_connected()){
			MCE_INFO("KafkaClient::checkConnect. already connected");
			return true;
		}
		MCE_INFO("KafkaClient::checkConnect. not connected");
		work_ = shared_ptr<boost::asio::io_service::work> (new boost::asio::io_service::work(ios_));
		bt_ = shared_ptr<boost::thread>(new boost::thread(boost::bind(&boost::asio::io_service::run, &ios_)));
		try{
			MCE_INFO( " fake cout connect" );
			prd_ = shared_ptr<producer>(new kafkaconnect::producer(ios_));
			prd_->connect(host_, port_);
			int cnt = 3;
			while(!prd_->is_connected())
			{
				cout << "cnt:" << cnt<< endl;
				if(cnt-- == 0){
					return false;	
				}
				boost::this_thread::sleep(boost::posix_time::seconds(1));
			}
			MCE_INFO("KafkaClient::checkConnect. connect success");
			return true;
		 }catch (std::exception& e) {
			cout << "exception:"	<< e.what() << endl;
		}
		return false;
	}
	void send(const std::vector<std::string> & messages,const string & topic,int partition){
		if(!checkConnect()){
			MCE_INFO("KafkaClient::send. not connected return");
			return;	
		}
		MCE_INFO("KafkaClient::send. already connected.begin send");
		prd_->send(messages, topic,partition);
	}

};
typedef boost::shared_ptr<KafkaClient> KafkaClientPtr;

inline vector<string> SimpleSplit(const string & s,const string sprt){
	vector<string> ks; 
	boost::split(ks,s, boost::is_any_of(sprt));	
	return ks;
}

inline map<int,pair<string,string> > ParseKafkaAddress(const string & addresses){
	map<int,pair<string,string> > m;
	vector<string> ks = SimpleSplit(addresses, ",");	
	int idx = 0;
	BOOST_FOREACH(const string & k, ks){
		vector<string> ipport = SimpleSplit(k,":");
		string ip = ipport[0];
		string port_= ipport[1];
		//KafkaClientPtr c = new KafkaClient(ip,port_);
		m[idx] = make_pair(ip,port_);
		++idx;
	}
	return m;
}

class ClientManager{

	map<int,KafkaClientPtr > client_map_;
	map<int,pair<string,string> > index2address_ ;
	int kafkacnt_,partitioncnt_;
public:
	
	int getPartitionCnt() const {return partitioncnt_;}
	int getKafkaCnt() const {return kafkacnt_;}
	bool init(int kafkacnt,int partitioncnt,const string & serverlist){

		kafkacnt_ = kafkacnt; 
		partitioncnt_ = partitioncnt;

		index2address_ = ParseKafkaAddress(serverlist);
		if(index2address_.size() != kafkacnt) {
			return false;	
		}
		
		typedef map<int,pair<string,string> >::value_type vt;
		foreach(const vt & p , index2address_){
			KafkaClientPtr c =KafkaClientPtr( new KafkaClient(p.first,p.second.first,p.second.second));		
			client_map_[p.first] = c;
		}
		return true;
	}

	//粗糙版本
	KafkaClientPtr getClient(int uid){
		int idx = uid % client_map_.size();	
		int size = client_map_.size ();
		if(size == 2){
			return client_map_[idx];	
		} else if(size == 1){
			return client_map_.begin()->second;		
		} else {
			return KafkaClientPtr();	
		}
	}
	size_t getClientSize(){
		return client_map_.size();
	}

	//ClientManager: work(new boost::asio::io_service::work(ios_)),bt(new boost::thread(boost::bind(&boost::asio::io_service::run, &io_service))) {
	ClientManager(){
	}
	virtual ~ClientManager(){
	}
		
};

#endif
