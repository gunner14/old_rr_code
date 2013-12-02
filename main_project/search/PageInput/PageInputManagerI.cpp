/*
 * =====================================================================================
 *
 *       Filename:  PageInputManagerI.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2009年07月29日 15时03分58秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  tanbokan (), bokan.tan@opi-corp.com
 *        Company:  opi-corp.com
 *
 * =====================================================================================
 */
#include "PageInputManagerI.h"
#include <cmath>
#include <boost/lexical_cast.hpp>
#include <algorithm>
#include <algorithm>
#include <boost/foreach.hpp>


using namespace std;
using namespace MyUtil;
using namespace com::xiaonei::xce;
using namespace xce::PageInput;

class PrintHelper {
public:
	template<typename T>
	static string Print(const T & container) {
		using namespace boost;
		ostringstream ios;

		ios << "(";
		int idx = 0;
		typedef typename T::value_type vt;
		BOOST_FOREACH(const vt & v,container){
			if(idx++ > 0) ios << ",";
			ios << v;
		}
		ios << ")";

		return ios.str();
	}
};
class TimeStat {
public:
        TimeStat() {  reset(); }
        void reset() { gettimeofday(&_begin, NULL); }
        float Get(bool clear = true) {
                gettimeofday(&_end, NULL);
                float timeuse = 1000000 * (_end.tv_sec - _begin.tv_sec)
                        +( _end.tv_usec - _begin.tv_usec);
                if(clear) reset();
                return timeuse / 1000;
        }
private:
        timeval _begin, _end;
};


// =============== MyUtil::initialize ====================================
void MyUtil::initialize() {
	ServiceI& service = ServiceI::instance();
	service.registerObjectCache(PAGEINPUT_DATA,"FI",new PageInputFactoryI);
	service.getAdapter()->add(&PageInputManagerI::instance(), service.createIdentity("Manager",""));
}
// =============== PageInputFactoryI ===================================
PageInputFactoryI::PageInputFactoryI() {
	reloadAllPages();
	thread_ = new UpdateThread(this);
	thread_->start().detach();
}

Ice::ObjectPtr PageInputFactoryI::create(Ice::Int id) {
	TimeStat ts;	
	PageInputDataIPtr obj=new PageInputDataI(id);
	std::map<int, std::string> pid2pname;
	generateNameMap(id,pid2pname);
	obj->addFollowingPages(pid2pname);
	MCE_INFO("PageInputFactoryI::create. uid:" << id << " cost:" << ts.Get() );
	return obj;
}
void PageInputFactoryI::generateNameMap(Ice::Int uid, map<int, string>& id2PageName) { 
	TimeStat ts;
	float select_cost = 0,loadmiss_cost= 0;
	vector<int> pageIds;
	selectFansPages(uid,pageIds);
	select_cost = ts.Get();
	vector<int> missIds;
	ostringstream oss;
	oss<<"load fans from db size:"<<pageIds.size();
	{
		IceUtil::RWRecMutex::RLock lock(mutex_);
		for(size_t i=0;i<pageIds.size();++i){
			map<int,pair<string,unsigned char> >::iterator it=id2Name_.find(pageIds[i]);
			if(it!=id2Name_.end()){
				if ((it->second).second == (unsigned char)0) {
						id2PageName[it->first]=(it->second).first;
				}
			}else{
				missIds.push_back(pageIds[i]);
			}
		}
	}
	{
	if(!missIds.empty()){
		reloadPages(missIds);
	}
	}
	loadmiss_cost = ts.Get();
	MCE_INFO("PageInputFactoryI::generateNameMap. uid:" << uid << " pageIds:" << PrintHelper::Print(pageIds) << " miss:" << PrintHelper::Print(missIds)
			<< " id2PageName:" << id2PageName.size() << " select_cost:" << select_cost << " loadmiss_cost:" << loadmiss_cost);
}
void PageInputFactoryI::selectFansPages(int uid, vector<int>& ids) { 
	ostringstream oss;
	oss << "fans_pages_"<<uid%100;
	Statement sql;
	sql << "SELECT page_id FROM fans_pages_"<<uid%100<<" WHERE user_id="<<uid<<" and state=0";
	FansPagesResultHandler handler(ids);
	QueryRunner(DB_FANS_PAGES, CDbRServer, oss.str()).query(sql, handler);
	MCE_INFO("PageInputManagerI::selectFansPages. uid:" << uid << " pids:" << PrintHelper::Print(ids));
}


//================ PinYinResultHandler ===================================
bool PinYinResultHandler::handle(mysqlpp::Row& res) const {
#ifndef NEWARCH
	string name=res["hanzi"].get_string();
	string pinyin=res["pinyin"].get_string();
#else
	string name=res["hanzi"].c_str();
	string pinyin=res["pinyin"].c_str();
#endif
	if(pinyin=="")
		return false;
	dict_[name].push_back(pinyin);
	return true;
}
//================ PageNameResultHandler ===================================
bool PageNameResultHandler::handle(mysqlpp::Row& res) const {
  	pair<string, unsigned char> name2state;
  	int id=(int)res["id"];
  	string name=res["name"].c_str();
	unsigned char state = (unsigned char)res["state"];
	if(!name.empty()){
  		name2state = make_pair(name,state);
		id2Name_.insert(make_pair<int,pair<string,unsigned char> >(id,name2state));
	}else{
		MCE_INFO("PageInputManagerI::handle. name empty. pid:" << id << " name:" << name << " state:" << res["state"]);	
	}
	return true;
}
//================ FansPagesResultHandler ===================================
bool FansPagesResultHandler::handle(mysqlpp::Row& res) const {
	int id=(int)res["page_id"];
	ids_.push_back(id);
	return true;
}
// =============== PageInputManagerI ===================================
PageInputManagerI::PageInputManagerI()
{
	try{
		Statement sql;
		sql << "SELECT hanzi, pinyin FROM search_pinyin";
		PinYinResultHandler handler(newSet, dict_);
		QueryRunner(DB_FRIENDINPUT_PINYIN, CDbRServer).query(sql, handler);
	}catch(...){
		MCE_WARN("initialize error");
	}
//	reload();
}
void PageInputFactoryI::reloadAllPages(){
	TimeStat ts;
	map<int,pair<string,unsigned char> > id2NameNew_;
	int oldsize = 0,newsize=0;
	try{
		Statement sql;
		sql << "SELECT id, name, state FROM page where state=0";
		PageNameResultHandler handler(id2NameNew_);
		QueryRunner(DB_SEARCH2_PAGE, CDbRServer).query(sql, handler);
		IceUtil::RWRecMutex::WLock lock(mutex_);
		oldsize = id2Name_.size();
		newsize = id2NameNew_.size();
	    	id2Name_.swap(id2NameNew_);
//		for(map<int,pair<string,unsigned char> >::iterator it=id2Name_.begin();it!=id2Name_.end();++it){
//			MCE_INFO("reloadAllPages id:"<<it->first<<" name:"<<(it->second).first);
//		}
	}catch(exception& ex){
		MCE_WARN(ex.what());
	}catch(...){
		MCE_WARN("reloadAllPages error");
	}
	MCE_INFO("reloadAllPages all from db done. oldsize:"<< oldsize << " newsize:" << newsize << " cost:" << ts.Get());
}



void PageInputFactoryI::reloadPages(const vector<int>& ids){
	set<int> pid_set(ids.begin(),ids.end());
	map<int,pair<string,unsigned char> > id2NameNew_;
	ostringstream oss;
	for(unsigned int i=0;i<ids.size();++i){
		if(i>0)
			oss<<",";
		oss<<ids[i];
	}
	vector<int> res;
	try{
		Statement sql;
		sql << "SELECT id, name, state FROM page WHERE id in ( "<<oss.str()<<" )";
		PageNameResultHandler handler(id2NameNew_);
		QueryRunner(DB_SEARCH2_PAGE, CDbRServer).query(sql, handler);
		IceUtil::RWRecMutex::WLock lock(mutex_);
		typedef map<int,pair<string,unsigned char> >::value_type Pid2Info;
		BOOST_FOREACH(Pid2Info & kv, id2NameNew_ ){
			id2Name_[kv.first] = kv.second	;
			res.push_back(kv.first);
			pid_set.erase(kv.first);
		}
		BOOST_FOREACH(int pid , pid_set){
			id2Name_[pid] = make_pair("",1);
		}
	}catch(exception& ex){
		MCE_WARN(ex.what());
	}catch(...){
		MCE_WARN("PageInputManagerI::reloadPages. error");
	}
	MCE_INFO("PageInputManagerI::reloadPages from db done. request pids:" << PrintHelper::Print(ids) << " result:" << PrintHelper::Print(res) << " not found in db:" 
			<< PrintHelper::Print(pid_set));
}

int PageInputManagerI::getStrLen(const string& condition) {
  int size = 0;
  for(vector<string>::size_type i = 0;i < condition.size();++i) {
    if((condition[i]&0x80) != 0) {
      ++size;
      i += 2;
    } else {
      ++size;
    }
  }
  return size;
}

MyUtil::IntSeq PageInputManagerI::getPageInput(Ice::Int hostID,const string& condition,Ice::Int limit,const Ice::Current& crt) {
	MCE_INFO("PageInputManagerI::getPageInput begin hostID= "<<hostID<<" condition="<<condition<<" condition size="<<condition.size()<<" limit="<<limit);
	TimeStat ts;
	//MCE_INFO(crt.adapter->getName()<<" "<<crt.facet<<" "<<crt.operation<<" "<<crt.requestId);
  int len = getStrLen(condition);
	if(len>15||len==0){
		MCE_INFO("PageInputManagerI::getPageInput done hostID= "<<hostID<<" condition="<<condition<<" len="<<len<<" limit="<<limit << " return null" );
		return vector<int>();
	} else{
    PageInputDataIPtr server = ServiceI::instance().findObject<PageInputDataIPtr>(PAGEINPUT_DATA, (long)hostID);
    if(server) {
      time_t old = server->getTime();
      int duration = time(NULL) - old;
      MCE_INFO("duration is :"<<duration);
      if(duration >= TIMEOUT_CACHE) {
        ServiceI::instance().removeObject(PAGEINPUT_DATA, (long)hostID);
      }
    } 

		MyUtil::IntSeq pids;
		pids = ServiceI::instance().locateObject<PageInputDataIPtr>(PAGEINPUT_DATA, (long)hostID)->search(condition,limit);
    MCE_INFO("cmtest PageInputManagerI::getPageInput done hostID= "<<hostID<<" condition="<<condition<<" limit="<<limit << " res:"  << PrintHelper::Print(pids)<<" cost:" << ts.Get());

		return pids;
	}
};

// ============== PageInputDataI ======================================
void PageInputDataI::splitWord(const string& value, vector<string>& words)
{
	for(vector<string>::size_type i=0;i<value.size();++i) {
		if((value[i]&0x80)!=0) {
			words.push_back(value.substr(i,3));
			i+=2;
		}else {
			words.push_back(string(1,value[i]));		
		}		
	}
}
void PageInputDataI::addFollowingPages(std::map<int,std::string>& result)
{
	friends.swap(result);
	string name;
	unsigned int id;
	for(map<int,string>::iterator it=friends.begin();it!=friends.end();++it) {
		name=it->second;
		id=it->first;
		vector<string> words;
		splitWord(name,words);
		for(unsigned int i=0;i<words.size();++i) {
			postingsHZ[words[i]].push_back(id);
		}
		for(unsigned int i=0;i<words.size();++i) {
			if(words[i].size()==3) {
			//	vector<string> pinyins=PageInputManagerI::instance().dict_[words[i]];
        vector<string> pinyins;
        map<string,vector<string> >::const_iterator it = PageInputManagerI::instance().dict_.find(words[i]);
        if(it == PageInputManagerI::instance().dict_.end()){
          continue;
        }
        pinyins = it->second;
                        
				for(vector<string>::iterator it_vec=pinyins.begin();it_vec!=pinyins.end();++it_vec) {
					postingsPY[(*it_vec)].push_back(id);
				}
			}			
		}
	}
   old_ = time(NULL);
}
bool PageInputDataI::isPinYin(const std::string& str, const std::vector<std::string> words) {
	if(words.size()<str.size()) {
		return false;
	} else {
		for(std::size_t i=0;i<words.size();++i) {
			if(words[i]<"a"||words[i]>"z")
			{
				return false;
			}
		}
		return true;
	}
}


vector<int> PageInputDataI::getPrefix(string query) {
	vector<int> tmp;
	map<string,vector<int> >::iterator it=postingsPY.lower_bound(query);
	while(it!=postingsPY.end()) {
		if(startsWith(it->first,query))
			union_vec(tmp,it->second);	
		else
			break;
		++it;
	}
	return tmp;
}

void PageInputDataI::intersect_vec(std::vector<int>& A,const std::vector<int>& B)
{
	vector<int> C;
	vector<int>::iterator itA=A.begin();
	vector<int>::const_iterator itB=B.begin();
	while(itA!=A.end()&&itB!=B.end()) {
		if((*itA)==(*itB)) {
			C.push_back(*itA);	
			++itA;
			++itB;
		}else if((*itA)>(*itB)) {
			++itB;				
		}else {
			++itA;				
		}	
	}
	A.swap(C);
}

void PageInputDataI::union_vec(std::vector<int>&A, const std::vector<int>& B) {
	vector<int> C;
	vector<int>::iterator itA=A.begin();
	vector<int>::const_iterator itB=B.begin();
	while(itA!=A.end()&&itB!=B.end()){
		if((*itA)==(*itB)){
			C.push_back(*itA);
			++itA;
			++itB;
		}else if((*itA)<(*itB)){
			C.push_back(*itA);
			++itA;
		}else{
			C.push_back(*itB);
			++itB;
		}
	}
	while(itA!=A.end()){
		C.push_back(*itA);
		++itA;
	}
	while(itB!=B.end()){
		C.push_back(*itB);
		++itB;
	}
	A.swap(C);
}
bool PageInputDataI::startsWith(const std::string& base, const std::string& pre) {
	if (base.size()<pre.size()) {
		return false;
	}
	int pc=pre.size();
	int po=0;
	while (--pc >= 0) {
		if (base[po] != pre[po++] ) {
			return false;
		}
	}
	return true;
}


MyUtil::IntSeq PageInputDataI::search(const string& query,Ice::Int limit)
{
	TimeStat ts;
	set<int> pids;
	vector<string> words;
	splitWord(query, words);
	bool ispy = false;
	if(isPinYin(query, words)) {
		ispy = true;
		vector<string> subStrs(query.size());
		splitPY(query,0,0,subStrs, pids);

		if(!pids.empty()){
			MCE_INFO("PageInputDataI::search. uid:" << uid << " query:" << query << " limit:" << limit << " ispy:" << ispy << " cost:"<< ts.Get() << " res:" << PrintHelper::Print(pids));
      return select(pids,limit);
		}
	}
  vector<int> results = searchHZ(query,limit);
	MCE_INFO("PageInputDataI::search. uid:" << uid << " query:" << query << " limit:" << limit << " ispy:" << ispy << " cost:"<< ts.Get() << " res:" << PrintHelper::Print(results));	
    return results;
}

vector<int> PageInputDataI::select(const set<int>& mySet,unsigned int limit) {
	size_t count = 0;
	vector<int> res;
	for(set<int>::const_iterator it=mySet.begin();it!=mySet.end() && count<limit;++it,++count){
		res.push_back(*it);	
		//MCE_INFO("Find PY:"<<res[count]<<" "<<friends[res[count]]);
	}
	//MCE_INFO("getPagesnumber : "<<res.size());
	return res;
}

bool PageInputDataI::reChoose(const int& doc,const vector<std::string>& strs,const int& len) {
	int inc=0;
	vector<string> words;
	splitWord(friends[doc], words);
	for(unsigned int i=0;i<words.size()&&inc<len;++i) {
		if(words[i].size()==3) {

//			vector<string>& pinyins = PageInputManagerI::instance().dict_[words[i]];
      vector<string> pinyins; 
      map<string,vector<string> >::const_iterator it = PageInputManagerI::instance().dict_.find(words[i]);
      if(it ==PageInputManagerI::instance().dict_.end()){
        continue;
      }
      pinyins = it->second;
			if(!pinyins.empty()) {
				for(vector<string>::iterator it=pinyins.begin();it!=pinyins.end();++it) {
					if(startsWith(*(it),strs[inc])) {
						++inc;
						break;
					}
				}
			}
		}
	}
	if(inc==len)
		return true;
	else
		return false;
}				
void PageInputDataI::splitPY(const string& pinyin,unsigned int start,int num,vector<string> subStrs,set<int>& pids) {
	if(start>=pinyin.size()) {
		vector<int> temp=getPrefix(subStrs[0]);
		for(int i=1;i<num;i++) {
			intersect_vec(temp,getPrefix(subStrs[i]));
			if(temp.empty())
				return;
		}
		for(vector<int>::iterator it=temp.begin();it!=temp.end();++it) {
			if(reChoose(*it,subStrs,num)) {
				pids.insert(*it);
			}
		}
		temp.clear();
		return;
	}else {
		for(unsigned int end=start+1;end<=pinyin.size();++end) {
			string temp=pinyin.substr(start,end-start);
			map<string,vector<int> >::iterator it=postingsPY.lower_bound(temp);
			if(it!=postingsPY.end()&&startsWith(it->first,temp)) {
				subStrs[num]=temp;
				splitPY(pinyin,end,num+1,subStrs,pids);			
			}		
		}	
	}
}		

vector<int> PageInputDataI::searchHZ(const string& name,const int& limit) {
	map<string,vector<int> >::iterator it;
	vector<string> words;
	splitWord(name, words);
	vector<int> results=postingsHZ[words[0]];
	for(unsigned int i=1;i<words.size();++i) {
		it=postingsHZ.find(words[i]);
		if( it==postingsHZ.end() ){
			return vector<int>();
		}
		intersect_vec(results,it->second);
		if(results.empty())
			return results;
	}
  set<int> pids;
  for(vector<int>::iterator it = results.begin();it != results.end();it++) {
    pids.insert(*it);
  }
  int count = 0;
  results.clear();
  for(set<int>::iterator iter = pids.begin();iter != pids.end()&&count<limit;iter++) {
    results.push_back(*iter);
    count++;
  }

	ostringstream oss;
	for(size_t i=0;i<results.size();++i){
		oss << " " << results[i]<<" "<<friends[results[i]];
	}
	MCE_INFO("PageInputManagerI::searchHZ. uid:"<< uid << " name:" << name << " limit:" << limit << " res:" << oss.str());
   	return results;
}
