#include "TalkCacheLoaderI.h"
#include "QueryRunner.h"
#include "ReplaceStat.h"
#include <sstream>
#include <time.h>
#include  <boost/lexical_cast.hpp>
#include <string.h>
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
using namespace MyUtil;
using namespace xce::talk;
using namespace com::xiaonei::xce;
using namespace com::xiaonei::talk::cache::loader;

void MyUtil::initialize() {
	ServiceI& service = ServiceI::instance();
	service.getAdapter()->add(new CacheLoaderManagerI, service.createIdentity("M", ""));
	ReplaceStat::instance().loadFaceTree();	
	TaskManager::instance().scheduleRepeated(&ReplaceStat::instance());

	int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("Service.Mod");
	int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("Service.Interval", 5);
	xce::serverstate::ServerStateSubscriber::instance().initialize("ControllerTalkCacheLoader", &CacheLoaderManagerI::instance(), mod, interval, new XceFeedControllerChannel());
	MCE_INFO("MyUtil::initialize. mod:" << mod << " interval:" << interval);
}


string StatLoader::getTableExpression(int id)
{
	ostringstream name;
	name<<"biz_doing_info_"<<id%100;
	return name.str();
}

string StatLoader::getTableName(int id)
{
	ostringstream name;
	name<<"doing_"<<id%100;
	return name.str();
}

string StatLoader::getDbInstance()
{
	return "biz_doing_info";
}

bool StatLoader::loadStat( TalkUserPtr& ui )
{
	string errmeta = "", u = "", d = "";
	size_t begin_pos, end_pos, uid_pos, did_pos, uid_digital, did_digital;
	Ice::Long did = 0;
	int uid = 0;

  try{
    Statement sql_doing;
    sql_doing<<"select content,type,link, meta from "<< StatLoader::getTableName(ui->id)<<" where userid = "<<ui->id<<" order by id desc limit 1";
    mysqlpp::StoreQueryResult res = QueryRunner( StatLoader::getDbInstance(), CDbRServer, StatLoader::getTableExpression(ui->id)).store(sql_doing);
    if (!res) {
      MCE_WARN("load doing error id=" << ui->id);
      return false;
    }
    if(res.num_rows() > 0){
      mysqlpp::Row row = res.at(0);
      ui->statusOriginal = row["content"].c_str();
			//------------------对状态转发做处理 : 规则 --》 找到 meta 里面的 第一个 uid 和 did 分别 作为根状态，然后再去查db，坠到状态后面 (这段代码要用正规json解析)---------------------------
			string meta = (string)row["meta"];
			bool paserok = true;
			if(!meta.empty()){//如果这个字段为空，代表有转发
				errmeta = meta;
				string uid_str = "uid";
				string did_str = "did";
				begin_pos = meta.find_first_of("[");
				end_pos = meta.find_first_of("}");
				
				uid_pos = meta.find_first_of(uid_str, begin_pos);
				did_pos = meta.find_last_of(did_str, end_pos);
				if(uid_pos != string::npos && did_pos != string::npos){
					uid_pos += 5;
					did_pos += 3;
					uid_digital = meta.find_first_of(",", uid_pos);
					did_digital = meta.find_first_of("}", did_pos);
					if(uid_digital != string::npos && uid_digital > uid_pos && did_digital != string::npos && did_digital > did_pos){
						u = meta.substr(uid_pos, uid_digital - uid_pos);
						d = meta.substr(did_pos, did_digital - did_pos);
						try{
							uid = boost::lexical_cast<int>(u);
						}catch (...) {
							MCE_WARN("StatLoader::loadStat --> cast uid error : uid = " << uid);
						}
						try{
							did = boost::lexical_cast<Ice::Long>(d);
						}catch(...){
							did = boost::lexical_cast<Ice::Long>(strtod(d.c_str(), NULL));
						}
						//MCE_INFO("StatLoader::loadStat --> id = " << ui->id << "did_pos = " << did_pos << " did_digital = " << did_digital << " meta->uid = " << u << " meta->did = " << d << " uid = " << uid << " did = " << did);
						sql_doing.clear();
						sql_doing<<"select content from "<< StatLoader::getTableName(uid)<<" where userid = "<<uid<<" and id = " << did << " order by id desc limit 1";
						mysqlpp::StoreQueryResult root_res = QueryRunner( StatLoader::getDbInstance(), CDbRServer, StatLoader::getTableExpression(uid)).store(sql_doing);
						if (!root_res) {
							MCE_WARN("load root_doing error uid=" << uid << " did = " << did);
						}
						if(root_res.num_rows() > 0){
							mysqlpp::Row row = root_res.at(0);
							string root_content = (string)row["content"];
							MCE_INFO("StatLoader::loadStat --> load root_doing actor = " << ui->id << " root_uid = " << uid << " root_did = " << did << " content = " << root_content);
							ui->statusOriginal += root_content;
						}
					}
				}
			}
			//---------------------------------------------
      ui->statusShifted = ui->statusOriginal;
      ReplaceStat::instance().replace(ui->id,(int)row["type"],ui->statusShifted,row["link"].c_str(),MAX_LENGTH);
    }
    return true;
  }
  catch(Ice::Exception& e) {
    MCE_WARN(e << " at " << __FILE__ << ":" << __LINE__);
    return false;
  } catch (std::exception& e) {
    MCE_WARN("begin_pos:<< " << begin_pos << " end_pos:" << end_pos << " uid_pos:" << uid_pos << " did_pos:" << did_pos << " uid_digital:" << uid_digital << " did_digital:" << did_digital << " u:" << u << " d:" << d << " uid:" << uid << " did:" << did << " errmeta:" << errmeta << " " << e.what() << " at " << __FILE__ << ":" << __LINE__);
    return false;
  } catch (...) {
    MCE_WARN("Unkown exception at " << __FILE__ << ":" << __LINE__);
    return false;
  }
  return false;
}

TalkUserPtr CacheLoaderManagerI::LocateUser(int id){
  TalkUserPtr ui;
  try {
    ui = new TalkUser;
    ui->id = id;
    Statement sql;
    sql << "select * from user_names where id = " << id;
    mysqlpp::StoreQueryResult res = QueryRunner("user_names", CDbRServer).store(sql);
    if (!res) {
      MCE_WARN("CacheLoaderManagerI::LocateUser --> load name error id="<<id  << "  return NULL");
      return NULL;
    }
    if(res.num_rows() > 0){
      mysqlpp::Row row = res.at(0);
      ui->name = row["name"].c_str();
    }

    sql.clear();
    sql << "select * from user_url where id=" << id;
    res = QueryRunner("user_url", CDbRServer).store(sql);
    if (!res) {
      MCE_WARN("CacheLoaderManagerI::LocateUser --> load url error id="<<id  << " return NULL");
      return NULL;
    }
    if(res.num_rows()>0){
      mysqlpp::Row row = res.at(0);
      ui->headurl = row["headurl"].c_str();
      ui->tinyurl = row["tinyurl"].c_str();
    }

    bool loadok = StatLoader::loadStat(ui);
    if(!loadok){
      return NULL;
    }
    return ui;
  }catch(Ice::Exception& e) {
    MCE_WARN(e << " at " << __FILE__ << ":" << __LINE__);
    return NULL;
  } catch (std::exception& e) {
    MCE_WARN(e.what() << " at " << __FILE__ << ":" << __LINE__);
    return NULL;
  } catch (...) {
    MCE_WARN("Unkown exception at " << __FILE__ << ":" << __LINE__);
    return NULL;
  }
  return NULL;
}


TalkUserMap CacheLoaderManagerI::LocateUser(const MyUtil::IntSeq& keys){
	TalkUserMap result;
	if(keys.empty()) {
		return TalkUserMap();
	}
	Statement sql1;
	sql1 << "select id, name from user_names where id in (";
	for (size_t i = 0; i < keys.size(); ++i) {
		if (i) {
			sql1<<",";
		}
		sql1<< keys.at(i) ;
	}
	sql1<<")";
	Statement sql2;
	sql2 << "select id, headurl, tinyurl from user_url where id in (";
	for (size_t i = 0; i < keys.size(); ++i) {
		if (i) {
			sql2<<",";
		}
		sql2<< keys.at(i) ;
	}
	sql2<<")";

	map< int,IntSeq > keySet;
	try {
		mysqlpp::StoreQueryResult res = QueryRunner("user_names", CDbRServer).store(sql1);
		if (!res || res.num_rows()<=0) {
			MCE_WARN("CacheLoaderManagerI::LocateUser --> BECAUSE NAME ERROR SO RETURN EMPTY MAP");
			return TalkUserMap();
		}
		for (size_t i = 0; i<res.num_rows(); ++i) {
			mysqlpp::Row row = res.at(i);
			TalkUserPtr ui = new TalkUser;
			ui->id = (int)row["id"];//ptr->userId;
			ui->name = row["name"].c_str();//ptr->realName;
			//ui->headurl = row["HEADURL"].c_str();//ptr->link;
			result[ui->id] = ui;
			keySet[(ui->id%100)].push_back(ui->id);
		}
		res = QueryRunner("user_url", CDbRServer).store(sql2);
		if (!res || res.num_rows()<=0) {
			MCE_WARN("CacheLoaderManagerI::LocateUser --> BECAUSE URL ERROR SO RETURN EMPTY MAP");
			return TalkUserMap();
		}
		for (size_t i = 0; i<res.num_rows(); ++i) {
			mysqlpp::Row row = res.at(i);
			int id = (int)row["id"];
			if(result.find(id) == result.end()){
				continue;
			}
			TalkUserPtr ui = result[id];
			ui->headurl = row["headurl"].c_str();
			ui->tinyurl = row["tinyurl"].c_str();
		}
		for(TalkUserMap::iterator uit = result.begin(); uit != result.end();){
    	bool loadok = StatLoader::loadStat(uit->second);
			if(!loadok){
				result.erase(uit++);
			}else{
				++uit;
			}
		}
    return result;
	} catch(Ice::Exception& e) {
		MCE_WARN(e << " at " << __FILE__ << ":" << __LINE__);
    return TalkUserMap();
	} catch (std::exception& e) {
		MCE_WARN(e.what() << " at " << __FILE__ << ":" << __LINE__);
    return TalkUserMap();
	} catch (...) {
		MCE_WARN("Unkown exception at " << __FILE__ << ":" << __LINE__);
    return TalkUserMap();
	}
	MCE_DEBUG("Create res.size()="<<result.size());
	return TalkUserMap();
}
/* brief: 更新给定用户memcached存储信息
 * userid: 给定用户
 * props: 更新数据的key-value对儿
 * return: void
 */
void CacheLoaderManagerI::updateUser(int userid, const MyUtil::Str2StrMap& props, const Ice::Current& current){
  IceUtil::Mutex::Lock lock(_mutex[userid % poolsize]);
  TalkUserPtr res;
	bool update = false;
	ostringstream up_str;
	try{
  res = TalkCacheClient::instance().GetUser(userid);
	}catch(Ice::Exception& e){
		MCE_WARN("CacheLoaderManagerI::updateUser-->TalkCacheClient::GetUser-->" << e);
		}
  if(!res){
		MCE_DEBUG("CacheLoaderManagerI::updateUser--> uid:" << userid << " memcache no user : " << userid << " and load from DB!");
    //TaskManager::instance().execute(new LoadTask(userid));
    return;
  }
	TalkUserPtr befor_u = new TalkUser;
	befor_u->id = res->id;
	befor_u->name = res->name;
	befor_u->statusOriginal = res->statusOriginal;
	befor_u->statusShifted = res->statusShifted;
	befor_u->headurl = res->headurl;
	befor_u->tinyurl = res->tinyurl;

  MCE_DEBUG("CacheLoaderManagerI::updateUser --> uid:" << userid << " get from memcache userid = " << userid);
  res->id = userid;
  MyUtil::Str2StrMap::const_iterator nit = props.find("NAME");
  if(nit != props.end()){
    res->name = nit->second;
		update = true;
		up_str << "upname ";
    MCE_DEBUG("CacheLoaderManagerI::updateUser --> uid:" << userid << " CHANGE NAME");
  }
  MyUtil::Str2StrMap::const_iterator dit = props.find("DOING");
  if(dit != props.end()){
    res->statusOriginal = dit->second;
    res->statusShifted = dit->second;
    ReplaceStat::instance().replace(res->id, TYPE_NORMAL, res->statusShifted, " ", MAX_LENGTH);
		update = true;
		up_str << "updoing ";
    MCE_DEBUG("CacheLoaderManagerI::updateUser --> uid:" << userid << " CHANGE DOING " << res->statusOriginal << " --- " << res->statusShifted);
  }
  MyUtil::Str2StrMap::const_iterator hit = props.find("HEADURL");
  if(hit != props.end()){
    res->headurl = hit->second;
		update = true;
		up_str << "upheadurl ";
    MCE_DEBUG("CacheLoaderManagerI::updateUser --> uid:" << userid << " CHANGE HEADURL");
  }
  MyUtil::Str2StrMap::const_iterator tit = props.find("TINYURL");
  if(tit != props.end()){
    res->tinyurl = tit->second;
		update = true;
		up_str << "uptinyurl ";
    MCE_DEBUG("CacheLoaderManagerI::updateUser --> uid:" << userid << " CHANGE TINYURL");
  }
	if(update){
		try{


			TalkUserPtr u = res;
			ostringstream os;
  		os << "CacheLoaderManagerI::updateUser--> up_str = " << up_str.str() << " check user BEFOR change : id = " << befor_u->id << " name = " << befor_u->name << " statusOriginal = " << befor_u->statusOriginal << " statusShifted = " << befor_u->statusShifted << " headurl = " << befor_u->headurl << " tinyurl = " << befor_u->tinyurl << " AFTER change : id = " << u->id << " name = " << u->name << " statusOriginal = " << u->statusOriginal << " statusShifted = " << u->statusShifted << " headurl = " << u->headurl << " tinyurl = " << u->tinyurl;
			if(befor_u->id != u->id){
				os << " id is diferent : " << befor_u->id << " vs " << u->id;
			}

			if(befor_u->name != u->name){
				os << " name is diferent : " << befor_u->name << " vs " << u->name;
			}
			if(befor_u->statusOriginal != u->statusOriginal){
				os << " statusOriginal is diferent : " << befor_u->statusOriginal << " vs " << u->statusOriginal;
			}
			if(befor_u->statusShifted != u->statusShifted){
				os << " statusShifted is diferent : " << befor_u->statusShifted << " vs " << u->statusShifted;
			}
			if(befor_u->headurl != u->headurl){
				os << " headurl is diferent : " << befor_u->headurl << " vs " << u->headurl;
			}
			if(befor_u->tinyurl != u->tinyurl){
				os << " tinyurl is diferent : " << befor_u->tinyurl << " vs " << u->tinyurl;
			}
			MCE_INFO(os.str());
			if(befor_u->id!= u->id){
				MCE_WARN("CacheLoaderManagerI::updateUser id is not equal so return ");
				return;
			}
			MCE_DEBUG("CacheLoaderManagerI::updateUser --> uid:" << userid << " CALL TalkCacheClient.SetUser");
			try{
  		TalkCacheClient::instance().SetUser(res);
			}catch(Ice::Exception& e){
				MCE_WARN("CacheLoaderManagerI::updateUser-->TalkCacheClient::SetUser-->" << e);
				}
		}catch(...){
			MCE_WARN("CacheLoaderManagerI::updateUser -->  uid:"<< userid << " CALL TalkCacheClient.SetUse Exception");
		}
	}
	else{
		MCE_DEBUG("CacheLoaderManagerI::updateUser --> uid:" << userid << " has no update..........");
	}
}

/* brief: 从数据库读取该用户的信息，并刷新cache
 * userid: 给定用户
 * return: 用户信息
 */
TalkUserPtr CacheLoaderManagerI::LoadById(int userid, const Ice::Current& current){
  TalkUserPtr ui = LocateUser(userid);
  if(ui){
		MCE_DEBUG("CacheLoaderManagerI::LoadById --> userid = " << userid << " SUCCESS!");
		try{
      TalkUserPtr u = ui; 
      MCE_INFO("CacheLoaderManagerI::LoadById--> check user : id = " << u->id << " name = " << u->name << " statusOriginal = " << u->statusOriginal << " statusShifted = " << u->statusShifted << " headurl = " << u->headurl << " tinyurl = " << u->tinyurl);
  		TalkCacheClient::instance().SetUser(ui);
		}catch(Ice::Exception& e){
			MCE_WARN("CacheLoaderManagerI::LoadById-->TalkCacheClient::SetUser-->" << e);
			}
		catch(...){
			MCE_WARN("CacheLoaderManagerI::LoadById -->  CALL TalkCacheClient.SetUse Exception userid:" << userid);
		}
  }
	else{
		MCE_WARN("CacheLoaderManagerI::LoadById --> DONOTSETMEMCACHED userid = " << userid);
	}
  return ui;
}
/* brief: 从数据库读用户序列的信息，并刷新cache
 * userids: 给定用户序列
 * return: 用户信息map
 */
TalkUserMap CacheLoaderManagerI::LoadBySeq(const MyUtil::IntSeq& userids, const Ice::Current& current){
  /*
  srand(time(NULL));
	if(rand()%10 >= 5){
		MCE_DEBUG("CacheLoaderManagerI::LoadBySeq --> lost the userids");
		return TalkUserMap();
	}
  */
  MCE_INFO("CacheLoaderManagerI::LoadBySeq --> size:" << userids.size());
	//return TalkUserMap();
	TalkUserMap ans;
	ans = LocateUser(userids);
	if(userids.size() != ans.size()){
		MCE_WARN("CacheLoaderManagerI::LoadBySeq --> userids.size = " << userids.size() << " loadsize = " << ans.size() << " DONOTSETMEMCACHED size = " << userids.size() - ans.size());
	}
	for(TalkUserMap::iterator it = ans.begin(); it != ans.end(); ++it){
    TalkUserPtr u = it->second;
    MCE_INFO("CacheLoaderManagerI::LoadBySeq --> check user : id = " << u->id << " name = " << u->name << " statusOriginal = " << u->statusOriginal << " statusShifted = " << u->statusShifted << " headurl = " << u->headurl << " tinyurl = " << u->tinyurl);
 	try{
   TalkCacheClient::instance().SetUser(it->second);
	}catch(Ice::Exception& e){
		MCE_WARN("CacheLoaderManagerI::LoadBySeq-->TalkCacheClient::SetUser-->" << e);
		}
	}
	MCE_DEBUG("CacheLoaderManagerI::LoadBySeq--> userids.size = " << userids.size() << " ans.size = " << ans.size());
	return ans;
}

void LoadTask::handle(){
  TalkUserPtr ui = CacheLoaderManagerI::instance().LocateUser(_userid);
  if(ui){
		try{
			MCE_DEBUG("LoadTask::handle --> CALL TalkCacheClient.SetUser");
  		TalkCacheClient::instance().SetUser(ui);
		}catch(Ice::Exception& e){
			MCE_WARN("LoadTask::handle-->TalkCacheClient::SetUser-->" << e);
			}
		catch(...){
			MCE_WARN("LoadTask::handle-->  CALL TalkCacheClient.SetUse Exception");
		}
  }
}


/* brief: 异步加载数据库用户序列的信息，并刷新cache
 * userids: 给定用户序列
 * return: void
 */
void CacheLoaderManagerI::LoadBySeqAsyn(const MyUtil::IntSeq& userids, const Ice::Current& current){

  MCE_INFO("CacheLoaderManagerI::LoadBySeqAsyn --> size:" << userids.size());
	//return TalkUserMap();
	TalkUserMap ans;
	ans = LocateUser(userids);
	if(userids.size() != ans.size()){
		MCE_WARN("CacheLoaderManagerI::LoadBySeqAsyn --> userids.size = " << userids.size() << " loadsize = " << ans.size() << " DONOTSETMEMCACHED size = " << userids.size() - ans.size());
	}
	for(TalkUserMap::iterator it = ans.begin(); it != ans.end(); ++it){
    TalkUserPtr u = it->second;
    MCE_INFO("CacheLoaderManagerI::LoadBySeqAsyn --> check user : id = " << u->id << " name = " << u->name << " statusOriginal = " << u->statusOriginal << " statusShifted = " << u->statusShifted << " headurl = " << u->headurl << " tinyurl = " << u->tinyurl);
    try{
      TalkCacheClient::instance().SetUser(it->second);
    }catch(Ice::Exception& e){
      MCE_WARN("CacheLoaderManagerI::LoadBySeqAsyn-->TalkCacheClient::SetUser-->" << e);
		}
	}
	MCE_DEBUG("CacheLoaderManagerI::LoadBySeqAsyn--> userids.size = " << userids.size() << " ans.size = " << ans.size());

}
