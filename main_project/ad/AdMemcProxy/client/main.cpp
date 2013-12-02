#include <boost/lexical_cast.hpp>
#include <iostream>
#include <string>
#include <map>
#include "MemcachedClientPool.h"

using namespace std;

const int THREADNUM = 10;
const int DATACOUNT = 1000000;

class TimeStat {
	private:
		timeval _begin , _end;
	public:
		TimeStat(){
			reset ();
		}
		void reset(){
			gettimeofday (&_begin , NULL);
		}
		float getTime () {
			gettimeofday (&_end , NULL);
			float timeuse = 1000000 * ( _end. tv_sec - _begin . tv_sec) + _end .tv_usec	- _begin .tv_usec ;
			timeuse /= 1000;
			return timeuse ;
		}
};

string  RandString(int n){
	string s;   
	char temp;   
	for(int i = 0; i < n; i++){   
		if(rand()%2){
			temp = '0';
			s = s + (char)(temp + rand()%10);   
		}else{
			temp = 'a';		
			s = s + (char)(temp + rand()%5);   
		}
	}   
	return s;   
}

class  MemcClientTest{
	public:
		MemcClientTest(){
			pool_.Init(50);
		}

		string  GetData(const string&  key){
			pair<int, MemcachedClient*> cli = pool_.GetClient(2); 
			if(!cli.second){ 
				cout << "Get Client Error" << endl;
				MCE_WARN("Get Client Error");
				return ""; 
			} 
			
			vector<string> keys;
			keys.push_back(key);
			
			map<string, string> res;
			map<string, int> flags; 
			cli.second->Get(keys, res, flags); 
			pool_.ReleaseClient(cli.first, cli.second); 
			if(res.size() != 1){
					return "Not Found the data";
			}
			return res[key];
		}

		bool PutData(const string& key,const  string& value){
			 pair<int, MemcachedClient*> cli = pool_.GetClient(1);
			 if(!cli.second){
				 MCE_WARN("Get Client Error");
				 return false;
			 }

			 bool res = cli.second->Set(key, value, 0);  
			 pool_.ReleaseClient(cli.first, cli.second);  
			 return res;
		}

	private:
		PoolManager pool_; 
};

struct  ThreadPara{
	int  start;
	int  count;
};

//从memcached 读数据
void*   GetData(void* para){
	ThreadPara*  tPara = (ThreadPara*)para;
	MemcClientTest test;
	for(int i = tPara->start; i < tPara->start + tPara->count; i++){
		cout << test.GetData(boost::lexical_cast<std::string>(i)) << endl;
	}
	return NULL;
}


void  GetDataById(){
	MemcClientTest test;
	int uid;
	while(cin >> uid){
		TimeStat ts;
		string  uidstr = test.GetData(boost::lexical_cast<std::string>(uid));
		//test.GetData(boost::lexical_cast<std::string>(uid));
		MCE_WARN("cost time :" << ts.getTime());
		cout << uid << ":" << uidstr << endl;

	}
}

//向memcached写数据
void*  PutData(void*  para){
	ThreadPara*  tPara = (ThreadPara*)para;
	MemcClientTest  test;
	for(int i = tPara->start; i < tPara->start + tPara->count; i++){
		bool ret = test.PutData(boost::lexical_cast<std::string>(i) , RandString(16));
		if(ret == false){
			MCE_DEBUG("put Failed");
		}
	}
	return NULL;
}


void  File2Memcached2(const string& filename){
	MemcClientTest memcclient;
	ifstream fin(filename.c_str());
	cout << "====================begin=================" << endl;
	while(!fin.eof()){
		char  chTemp[2100];
		fin.getline(chTemp, 2100);
		string strLine = chTemp;
		int npos = strLine.find_first_of('\t');
		string uid = strLine.substr(0, npos);
		string hobbies = strLine.substr(npos + 1);
		MCE_DEBUG("uid = " << uid);
		memcclient.PutData(uid, hobbies);
	}
	cout << "====================end=================" << endl;

	fin.close();
}




void  File2Memcached(const string& filename){
	FILE*  fp = fopen(filename.c_str(), "r");
	MemcClientTest  memcclient;
	TimeStat ts;
	while(!feof(fp)){
		char  chTemp[2100];
		fgets(chTemp, 2100, fp);
		string strLine = chTemp;
		int npos = strLine.find_first_of('\t');
		string uid = strLine.substr(0, npos);
		string hobbies = strLine.substr(npos + 1);
		MCE_DEBUG("uid = " << uid);
		memcclient.PutData(uid, hobbies);
	}
	MCE_DEBUG("cost time: " << ts.getTime());
	fclose(fp);
}



int main(int argc,char* argv[]){
	if(argc  < 2){
		cout << "Usage: memclient [put|get|file|Test]" << endl;
		return -1;
	}

	if(string(argv[1]) == "put"){
		//测试写memcached的性能
		MCE_INIT("./log_put", "Debug");
		TimeStat  ts;
		pthread_t thread[THREADNUM];
		for(int i=0;i<THREADNUM;i++) {
			ThreadPara para;
			para.start = i * THREADNUM;
			para.count = DATACOUNT/THREADNUM;
			int pret = pthread_create(&thread[i], NULL, PutData, &para);
			if(pret != 0){
				MCE_WARN("Thread Create Failed.");
				exit(1);
			}   
		}   
		for(int i=0;i<THREADNUM;i++){
			pthread_join(thread[i], NULL);
		} 	
		MCE_DEBUG("put  1000000 data use time" << ts.getTime() );
	} else if(string(argv[1]) == "get") {
		MCE_INIT("./log_get","Debug");
		//测试读memached的性能
		TimeStat ts;
		pthread_t thread[THREADNUM];
		for(int i=0;i<THREADNUM;i++) {
			ThreadPara para;
			para.start = i * THREADNUM;
			para.count = DATACOUNT/THREADNUM;
			int pret = pthread_create(&thread[i], NULL, GetData, &para);
			if(pret != 0){
				MCE_WARN("Thread Create Failed.");
				exit(1);
			}   
		}   
		for(int i=0;i<THREADNUM;i++){
			pthread_join(thread[i], NULL);
		}
		MCE_DEBUG("get  1000000 data use time" << ts.getTime());
	}else if(string(argv[1]) == "file"){
		MCE_INIT("./log_file", "DEBUG");
		File2Memcached2(argv[2]);
	}else if(string(argv[1]) == "Test"){
		MCE_INIT("./log_Test", "DEBUG");
		GetDataById();
	}	else{
		cout << "Usage: memclient [put|get|file]" << endl;
	}
	return 0;
}
