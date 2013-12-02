#include "StatusCacheAdapter.h"
#include "FriendRankUpdateI.h"
#include "UserModelCreator/PassportFetcher.h"
#include "UserModelCreator/main.cpp"
#include "SessionSpliter/main.cpp"
#include "ActiveRankUpdater/main.cpp"
#include "StaticRankUpdater/main.cpp"
#include "FriendRankCreator/main.cpp"
#include "FriendRankWriter/main.cpp"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"

using namespace MyUtil;
using namespace xce::serverstate;
using namespace xce::socialgraph;
using namespace xce::statuscache;

// MyUtil::initialize {{{
void MyUtil::initialize()
{
/*	MCE_INFO("FriendRankUpdateI MyUtil::initialize().");
	ServiceI& service = ServiceI::instance();
	service.getAdapter()->add(&FriendRankUpdateI::instance(),
			service.createIdentity("FriendRankUpdateNew", ""));

	// 从服务配置文件获取参数
	Ice::CommunicatorPtr& ic = service.getCommunicator();
	int mod         = ic->getProperties()->getPropertyAsInt("FriendRankUpdateNew.Mod");
  	int sInterval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("FriendRankUpdateNew.Interval", 5);
	string endpoint = ic->getProperties()->getProperty("FriendRankUpdateNew.Endpoint");
	FriendRankUpdateI::instance().Bootstrap(endpoint, mod);

	// 设置线程类消费线程的最大值和最小值，可根据任务执行时间长度来调整
	TaskManager::instance().config(TASK_LEVEL_USERMODEL, ThreadPoolConfig(1,1));
	TaskManager::instance().config(TASK_LEVEL_USERMODEL_WORKER, ThreadPoolConfig(100,100));
	TaskManager::instance().config(TASK_LEVEL_FRIENDRANK, ThreadPoolConfig(1,1));
	ServerStateSubscriber::instance().initialize("ControllerFriendRankUpdateNew", &FriendRankUpdateI::instance(), 
		mod, sInterval, new MyUtil::SocialGraphChannel());
  	ServerStateSubscriber::instance().setStatus(1);
*/	ServiceI& service = ServiceI::instance();
  	service.getAdapter()->add(&FriendRankUpdateI::instance(), service.createIdentity("FriendRankUpdateNew", ""));

  	ConnectionPoolManager::instance().initialize();

  	int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("FriendRankUpdateNew.Mod");
  	int sInterval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("FriendRankUpdateNew.Interval", 5);

  //向controller注册
        ServerStateSubscriber::instance().initialize("ControllerFriendRankUpdateNew", &FriendRankUpdateI::instance(),
                                         mod, sInterval, new MyUtil::SocialGraphChannel());
  	ServerStateSubscriber::instance().setStatus(1);

	FriendRankUpdateI::instance().Bootstrap();
	// 设置线程类消费线程的最大值和最小值，可根据任务执行时间长度来调整
        TaskManager::instance().config(TASK_LEVEL_USERMODEL, ThreadPoolConfig(1,1));
        TaskManager::instance().config(TASK_LEVEL_USERMODEL_WORKER, ThreadPoolConfig(100,100));
        TaskManager::instance().config(TASK_LEVEL_FRIENDRANK, ThreadPoolConfig(1,1));
}// }}}

void FriendRankUpdateI::Bootstrap()//{{{
{
        _usermodel = 0;
        _friendrank = 0;
}
// 进行FriendRankCacheI的初始化工作
void FriendRankUpdateI::Bootstrap(const string& endpoint, int mod)//{{{
{
	MCE_INFO( "[FriendRankUpdateI::Bootstrap] endpoint=" << endpoint << " mod=" << mod );
	_endpoint = endpoint;
	_mod = mod;
	_usermodel = 0;
	_friendrank = 0;
}// }}}
int FriendRankUpdateI::split(const char* s, vector<string>& list, const char *delim) // {{{
{
	const char* begin = s;
	const char* end;

	list.clear();
	while (*begin!='\0')
	{
		end = begin;
		while (*end!='\0' && strchr(delim,*end)==NULL)
			++end;

		if (*begin!='\0')
			list.push_back(string(begin, end-begin));

		if (*end!='\0')
			++end;

		begin = end;
	}

	return list.size();
}// }}}
long FriendRankUpdateI::GetFreeMemory() //{{{
{
	map<string,string> status;
	FILE* fp = fopen("/proc/meminfo", "rt");
	if (fp==NULL)
	{
		MCE_ERROR("Error on reading /proc/meminfo.");
		return -1;
	}
	char line[256];
	fgets(line, 256, fp);
	while (!feof(fp))
	{
		vector<string> list;
		split(line, list, "\t: \n");
		status.insert(make_pair<string,string>(list[0],list[list.size()-2]));
		fgets(line, 256, fp);
	}
	fclose(fp);

	//----------------------------
	long free_memory = atol(status["MemFree"].c_str());
	free_memory += atol(status["Cached"].c_str());
	return free_memory;
}//}}}

// FriendRankUpdateI::StateOfUserModel {{{
int FriendRankUpdateI::GetUserModel()
{
	IceUtil::RWRecMutex::RLock lock(_usermodelNewMutex);
	return _usermodel;
}// }}}
// FriendRankUpdateI::StateOfFriendRank {{{
int FriendRankUpdateI::GetFriendRank()
{
	IceUtil::RWRecMutex::RLock lock(_friendrankNewMutex);
	return _friendrank;
}// }}}

// FriendRankUpdateI::IncrFriendRank,DescFriendRank {{{
void FriendRankUpdateI::IncrFriendRank()
{
	IceUtil::RWRecMutex::WLock lock(_friendrankNewMutex);
	++_friendrank;
}
void FriendRankUpdateI::DescFriendRank()
{
	IceUtil::RWRecMutex::WLock lock(_friendrankNewMutex);
	--_friendrank;
}// }}}
// FriendRankUpdateI::IncrUserModel,DescUserModel {{{
void FriendRankUpdateI::IncrUserModel()
{
	IceUtil::RWRecMutex::WLock lock(_usermodelNewMutex);
	++_usermodel;
}
void FriendRankUpdateI::DescUserModel()
{
	IceUtil::RWRecMutex::WLock lock(_usermodelNewMutex);
	--_usermodel;
}// }}}

// TriggerUserModel {{{
int FriendRankUpdateI::TriggerUserModel(const Ice::Current& cur)
{
	MCE_INFO("FriendRankUpdateI::TriggerUserModel()");
	if (GetUserModel()!=0)
		return 1;

	IncrUserModel();
	TaskManager::instance().execute(new UserModelTask(this));
	return 0;
}//}}}
// TriggerFriendRank {{{
int FriendRankUpdateI::TriggerFriendRank(const Ice::Current& cur)
{
	MCE_INFO("FriendRankUpdateI::TriggerFriendRank()");
	if (GetFriendRank()!=0)
	{
		int i = GetFriendRank();
		MCE_INFO("_friendRank:"<<i);
		MCE_INFO("The last task is still running");
		return 1;
	}
	if (GetFreeMemory()<26214400) // 25G
	{
		MCE_INFO("The task cannot be started. need 26214400(KB) but "<<GetFreeMemory()<<" left");
		return 2;
	}

	TaskManager::instance().execute(new FriendRankTask(this));
	return 0;
}// }}}

// UserModelTask::handle {{{
void UserModelTask::handle()
{
	// step1: read valid users to file
	const char* fname = "/data/xce/friendranknew/data/passport.list";
	FILE* fp = fopen(fname, "wb");
	if (fp==NULL)
	{
		MCE_ERROR("FriendRankUpdateI::TriggerUserModel() : " << strerror(errno) << " : " << fname);
		return;
	}
	SavePassport(fp);
	fclose(fp);

	// step2: invoke 100 thread to read database
	long WORKNUM = 100;
	long STEP = 1000;
	for (long WORKER=0; WORKER<WORKNUM; ++WORKER)
	{
		TaskManager::instance().execute(new UserModelWorkerTask(_pUpdate, WORKER, WORKNUM, STEP));
	}
	_pUpdate->DescUserModel();
}// }}}
// UserModelTask::SavePassport {{{
void UserModelTask::SavePassport(FILE* fp)
{
	// 方法1：读StatusCache
	//MCE_INFO("\tRead StatusCacheAdapter ... ");
	//bitset<STATUS_CACHE_SIZE>* state = new bitset<STATUS_CACHE_SIZE>;
	//try {
	//	StatusCacheAdapter::instance().getAllStatus(*state);
	//} catch (Ice::TimeoutException& e) {
	//	MCE_ERROR("TriggerUserModel timeout exception" << e);
	//} catch (Ice::Exception& e) {
	//	MCE_ERROR("TriggerUserModel ice::exception " << e.what());
	//} catch (...) {
	//	MCE_ERROR("TriggerUserModel exception");
	//}
	//MCE_INFO("\tRead StatusCache ... OK.");

	//long valid = 0;
	//for (int i=0; i<STATUS_CACHE_SIZE; ++i)
	//{
	//	if ((state->test(i)))
	//	{
	//		++valid;
	//		fwrite(&i, sizeof(int), 1, fp);
	//	}
	//}
	//MCE_INFO("\tSave StatusCache ... OK. Valid users : " << valid);
	//delete state;

	// 方法2：读user_passport
	PassportFetcher* gPassport = new PassportFetcher;
	int beginid = 0;
	int step = 1000000;
	int numstep = 0;
	while (true)
	{
		MCE_INFO("["<<++numstep<<"] Reading user_passport (*"<<step<<")");
		pair<int,int> ret = gPassport->Save2File(step, beginid, fp);
		if (ret.second<step)
			break;
		beginid = ret.first + 1;
	}
	MCE_INFO("[OK] Reading user_passport (*"<<step<<")");
	delete gPassport;
}// }}}

// UserModelWorkerTask::handle {{{
void UserModelWorkerTask::handle()
{
	_pUpdate->IncrUserModel();
	CreateUserModel(_WORKER, _WORKNUM, _STEP);
	_pUpdate->DescUserModel();
}// }}}

// FriendRankTask::handle {{{
void FriendRankTask::handle()
{
	MCE_INFO("handle()");
	_pUpdate->IncrFriendRank();
//	SplitSession();
//	UpdateActiveRank();
//	UpdateStaticRank();
	
	CreateFriendRank();
	WriteFriendRank();
	_pUpdate->DescFriendRank();

	// TODO: hash_map内存不释放？？暂时使用重启服务的方法解决
	MCE_INFO("Force service to restart.");
	assert(false);
}// }}}

