#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <IceGrid/Query.h>
#include <IceBox/IceBox.h>
#include "IceLogger.h"
#include "ObjectCacheI.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace MyUtil;
using namespace Ice;

const static string XIAONEI = "xiaonei";
const static string KAIXIN = "kaixin";

class Worker{
	public:
		Worker(){
			Ice::PropertiesPtr props = Ice::createProperties();
			props->setProperty("Ice.Override.Timeout", "30000");
			props->setProperty("Ice.ThreadPool.Client.SizeMax", "1000");
			props->setProperty("Ice.ThreadPool.Client.StackSize","131072");
			props->setProperty("Ice.ThreadPool.Server.SizeMax", "1000");
			props->setProperty("Ice.ThreadPool.Server.StackSize","131072");
			props->setProperty("Ice.MessageSizeMax", "10240");

			props->setProperty("IceGrid.InstanceName", "XiaoNei");
			props->setProperty("Ice.Default.Locator","XiaoNei/Locator:default -h 10.3.17.74 -p 12000:default -h 10.3.17.75 -p 12001:default -h 10.3.17.76 -p 12002");
			Ice::InitializationData id_xiaonei_oce;
			id_xiaonei_oce.properties = props;
			_ic_xiaonei_oce = Ice::initialize(id_xiaonei_oce);
			_qr_xiaonei_oce = IceGrid::QueryPrx::uncheckedCast(_ic_xiaonei_oce->stringToProxy("XiaoNei/Query"));

			props->setProperty("IceGrid.InstanceName", "XiaoNei");
			props->setProperty("Ice.Default.Locator","XiaoNei/Locator:default -h 10.22.200.41 -p 12000:default -h 10.22.200.42 -p 12001:default -h 10.22.200.43 -p 12002");
			Ice::InitializationData id_kaixin_oce;
			id_kaixin_oce.properties = props;
			_ic_kaixin_oce = Ice::initialize(id_kaixin_oce);
			_qr_kaixin_oce = IceGrid::QueryPrx::uncheckedCast(_ic_kaixin_oce->stringToProxy("XiaoNei/Query"));

			props->setProperty("IceGrid.InstanceName", "CoreCache");
			props->setProperty("Ice.Default.Locator","CoreCache/Locator:default -h 10.3.17.83 -p 4068:default -h 10.3.17.82 -p 4068:default -h 10.3.17.81 -p 4068");
			Ice::InitializationData id_xiaonei_cache;
			id_xiaonei_cache.properties = props;
			_ic_xiaonei_cache = Ice::initialize(id_xiaonei_cache);
			_qr_xiaonei_cache = IceGrid::QueryPrx::uncheckedCast(_ic_xiaonei_cache->stringToProxy("CoreCache/Query"));

			props->setProperty("IceGrid.InstanceName", "CoreCache");
			props->setProperty("Ice.Default.Locator","CoreCache/Locator:default -h 10.22.200.51 -p 4068:default -h 10.22.200.52 -p 4068:default -h 10.22.200.53 -p 4068");
			Ice::InitializationData id_kaixin_cache;
			id_kaixin_cache.properties = props;
			_ic_kaixin_cache = Ice::initialize(id_kaixin_cache);
			_qr_kaixin_cache = IceGrid::QueryPrx::uncheckedCast(_ic_kaixin_cache->stringToProxy("CoreCache/Query"));

			vector<int> categary_usern;
			categary_usern.push_back(0);
			categary_usern.push_back(1);
			categary_usern.push_back(3);
			categary_usern.push_back(7);
			categary_usern.push_back(8);
			categary_usern.push_back(9);
			categary_usern.push_back(10);
			_sv_categary["UserN"] = categary_usern;
			_sv_cluster["UserN"] = 100;

			vector<int> categary_userconfigr;
			categary_userconfigr.push_back(0);
			_sv_categary["UserConfigR"] = categary_userconfigr;
			_sv_cluster["UserConfigR"] = 100;

			vector<int> categary_usernetworkn;
			categary_usernetworkn.push_back(0);
			categary_usernetworkn.push_back(1);
			categary_usernetworkn.push_back(2);
			categary_usernetworkn.push_back(3);
			categary_usernetworkn.push_back(4);
			categary_usernetworkn.push_back(5);
			categary_usernetworkn.push_back(6);
			_sv_categary["UserNetworkN"] = categary_usernetworkn;
			_sv_cluster["UserNetworkN"] = 10;

			vector<int> categary_offerfriends;
			categary_offerfriends.push_back(0);
			categary_offerfriends.push_back(1);
			categary_offerfriends.push_back(2);
			_sv_categary["OfferFriends"] = categary_offerfriends;
			_sv_cluster["OfferFriends"] = 10;

			vector<int> categary_buddycore;
			categary_buddycore.push_back(0);
			_sv_categary["BuddyCore"] = categary_buddycore;
			_sv_cluster["BuddyCore"] = 10;

			vector<int> categary_recentlyonlinebuddy;
			categary_recentlyonlinebuddy.push_back(0);
			_sv_categary["RecentlyOnlineBuddy"] = categary_recentlyonlinebuddy;
			_sv_cluster["RecentlyOnlineBuddy"] = 10;

			vector<int> categary_footprint;
			categary_footprint.push_back(0);
			_sv_categary["Footprint"] = categary_footprint;
			_sv_cluster["Footprint"] = 10;

			vector<int> categary_buddycache;
			categary_buddycache.push_back(0);
			_sv_categary["BuddyCache"] = categary_buddycache;
			_sv_cluster["BuddyCache"] = 10;

			vector<int> categary_buddycachebyid;
			categary_buddycachebyid.push_back(0);
			_sv_categary["BuddyCacheById"] = categary_buddycachebyid;
			_sv_cluster["BuddyCacheById"] = 10;
		}

		~Worker(){};

		void reload(const string& xORk,const string& adapter,int userId){
			bool hasReplica=false;
			Ice::CommunicatorPtr ic = 0;
			IceGrid::QueryPrx qr = 0;
			selectGrid(ic,qr,xORk,adapter,hasReplica);
			if(!ic){
				cout << "Cannot reload from business \"" << xORk << "\". Only \"" << XIAONEI << "\" or \"" << KAIXIN << "\" be accepted!" << endl;
				return;
			}
			map<string,vector<int> >::iterator cateIt = _sv_categary.find(adapter);
			map<string,int>::iterator clusIt = _sv_cluster.find(adapter);
			if(cateIt==_sv_categary.end()||clusIt==_sv_cluster.end()){
				cout << "Cannot reload service \"" << adapter << "\". Permission denied!" << endl;
				return;
			}

			vector<int>& categaries = cateIt->second;
			int cluster = clusIt->second;

			ostringstream endpoint;
			endpoint << "SC@" << adapter;
			if(cluster>1){
				endpoint << (userId%cluster);
			}
			Ice::ObjectPrx prx = ic->stringToProxy(endpoint.str());
			cout << " *--- reloading " << userId << " in service: " << adapter << endl;
			for(vector<int>::const_iterator cit=categaries.begin();cit!=categaries.end();++cit){
				doload(prx,hasReplica,qr,*cit,userId);
				cout << "   -- Categary " << *cit << " : Object reloaded!" << endl; 
			}
			cout << "   -- done." << endl;
		}
		void reload(const string& xORk,const string& adapter,const vector<int>& userlist){
			bool hasReplica=false;
			Ice::CommunicatorPtr ic = 0;
			IceGrid::QueryPrx qr = 0;
			selectGrid(ic,qr,xORk,adapter,hasReplica);
			if(!ic){
				cout << "Cannot reload from business \"" << xORk << "\". Only \"" << ::XIAONEI << "\" or \"" << ::KAIXIN << "\" be accepted!" << endl;
				return;
			}
			map<string,vector<int> >::iterator cateIt = _sv_categary.find(adapter);
			map<string,int>::iterator clusIt = _sv_cluster.find(adapter);
			if(cateIt==_sv_categary.end()||clusIt==_sv_cluster.end()){
				cout << "Cannot reload service \"" << adapter << "\". Permission denied!" << endl;
				return;
			}

			vector<int>& categaries = cateIt->second;
			int cluster = clusIt->second;

			if(userlist.size()==0){cout << "   -- user listis empty, do nothing." << endl;return;}

			vector<MyUtil::LongSeq> reloadlist;
			int length=cluster>1?cluster:1;
			reloadlist.resize(length);
			int batchCount=0;
			for(vector<int>::const_iterator it=userlist.begin();it!=userlist.end();++it){
				int pos= cluster>1 ? (*it)%(cluster) : 0;
				reloadlist[pos].push_back(*it);
				cout << "   -- add " << (long)(*it) << " to reloadlist[" << pos << "]" << endl;
				if( ++batchCount<10000 && (it+1)!=userlist.end() ){continue;}
				for(int pos=0;pos<length;++pos){
					ostringstream endpoint;
					endpoint << "SC@" << adapter;
					if(cluster>1){
						endpoint << pos;
					}
					Ice::ObjectPrx prx = ic->stringToProxy(endpoint.str());
					ostringstream categarylist;
					for(vector<int>::const_iterator cit=categaries.begin();cit!=categaries.end();++cit){
						doload(prx,hasReplica,qr,*cit,reloadlist[pos]);
						categarylist << *cit << " ";
					}
					cout << "   -- " << reloadlist[pos].size() << " objects reloaded in " << endpoint.str() << " categary( " << categarylist.str() << ")" << endl;
				}
				reloadlist.clear();
				reloadlist.resize(length);
				batchCount=0;
				cout << "   -- sleeping 3 seconds ..." << endl;
				sleep(1);
			}
			cout << "   -- done." << endl;
		}
	private:
		Ice::CommunicatorPtr _ic_xiaonei_oce;
		Ice::CommunicatorPtr _ic_xiaonei_cache;
		Ice::CommunicatorPtr _ic_kaixin_oce;
		Ice::CommunicatorPtr _ic_kaixin_cache;
		IceGrid::QueryPrx _qr_xiaonei_oce;
		IceGrid::QueryPrx _qr_xiaonei_cache;
		IceGrid::QueryPrx _qr_kaixin_oce;
		IceGrid::QueryPrx _qr_kaixin_cache;
		map<string,vector<int> > _sv_categary;
		map<string,int> _sv_cluster;
		void selectGrid(Ice::CommunicatorPtr& ic,IceGrid::QueryPrx& qr,const string& xORk, const string& adapter,bool& hasReplica){
			if(xORk==::XIAONEI){
				if(adapter=="BuddyCache"||adapter=="BuddyCacheById"||adapter=="UserCache"){
					hasReplica=true; 
					ic=_ic_xiaonei_cache;
					qr=_qr_xiaonei_cache;
				}else{
					hasReplica=false; 
					ic=_ic_xiaonei_oce;
					qr=_qr_xiaonei_oce;
				}
			} else if(xORk==::KAIXIN){
				if(adapter=="BuddyCache"||adapter=="BuddyCacheById"||adapter=="UserCache"){
					hasReplica=true; 
					ic=_ic_kaixin_cache;
					qr=_qr_kaixin_cache;
					hasReplica=true; 
				}else{
					hasReplica=false; 
					ic=_ic_kaixin_oce;
					qr=_qr_kaixin_oce;
				}
			} else {
				hasReplica=false;
				ic=0;
				qr=0;
			}
		}
		void doload(const Ice::ObjectPrx& prx,bool hasReplica,const IceGrid::QueryPrx& query,int categary,int userId){
			if(!hasReplica){
				ObjectCachePrx obj = ObjectCachePrx::uncheckedCast(prx);
				obj->removeObject(categary,userId);
				cout << "   -- test " << obj->hasObject(categary,userId) << endl;
				try{
					obj->locateObject(categary,userId);
				}catch(...){}
				cout << "   -- test " << obj->hasObject(categary,userId) << endl;
				return;
			}
			vector<Ice::ObjectPrx> prxs = query->findAllReplicas(prx);
			//cout << "   -- find " << prxs.size() << " replicas! " << endl;
			for(vector<Ice::ObjectPrx>::iterator itr=prxs.begin();itr!=prxs.end();++itr){
				ObjectCachePrx obj = ObjectCachePrx::uncheckedCast((*itr));
				obj->removeObject(categary,userId);
				try{
					obj->locateObject(categary,userId);
				}catch(...){}
			}
		}

		void doload(const Ice::ObjectPrx& prx,bool hasReplica,const IceGrid::QueryPrx& query,int categary,const MyUtil::LongSeq& userList){
			if(!hasReplica){
				ObjectCachePrx obj = ObjectCachePrx::uncheckedCast(prx);
				obj->removeObjects(categary,userList);
				try{
					obj->locateObjects(categary,userList);
				}catch(...){}
				return;
			}
			vector<Ice::ObjectPrx> prxs = query->findAllReplicas(prx);
			cout << "   -- find " << prxs.size() << " replicas! " << endl;
			for(vector<Ice::ObjectPrx>::iterator itr=prxs.begin();itr!=prxs.end();++itr){
				ObjectCachePrx obj = ObjectCachePrx::uncheckedCast((*itr));
				obj->removeObjects(categary,userList);
				try{
					obj->locateObjects(categary,userList);
				}catch(...){}
			}
		}
};

vector<int> getUserListFromFile(const char* filename)
{
	cout << " *--- reading user list from file: " << filename << endl;
	set<int> userset;
	ifstream ifs(filename);
	if (!ifs.is_open())
	{
		cout << "   -- error opening file" << filename << endl;
		return vector<int>();
	}
	string buffer;
	while (getline(ifs,buffer)){
		try{
			int userId=boost::lexical_cast<int>(buffer);
			//cout << "   -- read ID: " << userId << endl;
			userset.insert(userId);
		}catch(...){
			cout << "   -- skipped bad ID: " << buffer << endl;
		}
	}
	cout << "   -- reach end, file closed." << endl;
	ifs.close();
	vector<int> userlist;
	for(set<int>::iterator si=userset.begin(); si!=userset.end(); si++)
	{
		userlist.push_back(*si);
	}
	return userlist;
}

void help()
{
	cout << "Usage: Reload [OPTIONS]" << endl;
	cout << "-h, --help\tDisplay this help and exit." << endl;
	cout << "-b, --business\tBusiness Name. ( \"xiaonei\" or \"kaixin\" )" << endl;
	cout << "-s, --service\tService name for reload. Currently supported: UserN/UserConfigR/UserNetworkN/OfferFriends/Footprint." << endl;
	cout << "-u, --user\tUser ID for reload." << endl;
	cout << "-f, --file\tFile which contains user list. (one id each line)" << endl;
}

int main (int argc, char * const argv[]) {
	string business="";
	string service="";
	int user=0;
	char* filename="";
	if (argc<2)
	{
		help();
		return 1;
	}

#define OPT(_a1, _a2)	else if ( !strcmp(argv[i], _a1) || !strcmp(argv[i], _a2) )
	int i;
	for (i=1; i<argc; i++)
	{
		if ( i == 0);
		OPT("-h", "--help")	{help(); return 0;}
		else if (i + 1 == argc)	break;
		OPT("-b", "--business")	business= argv[++i];
		OPT("-s", "--service")	service = argv[++i];
		OPT("-u", "--user")	user = atoi(argv[++i]);
		OPT("-f", "--file")	filename = argv[++i];
		else break;
	}
#undef OPT

	if (i!=argc)
	{
		cout << "ERROR: malformed or unknown option near '" << argv[i] << "'." << endl;
		return 1;
	}
	if (business=="")
	{
		cout << "Need Business Name." << endl;
		return 1;
	}
	if (service=="")
	{
		cout << "Need Service Name." << endl;
		return 1;
	}
	if (user==0 && filename=="")
	{
		cout << "Need user ID or File which contains user list" << endl;
		return 1;
	}

	::Worker w;
	if(filename!=""){
		w.reload(business,service,getUserListFromFile(filename));
	}
	if(user!=0){
		w.reload(business,service,user);
	}
}

