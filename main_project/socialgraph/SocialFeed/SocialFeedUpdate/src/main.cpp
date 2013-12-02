/*
 * main.cpp
 *
 *  Modified on: Fri 16, 2012
 *      Author: zizhong.zhang
 */

#include "MyTasks.h"
using namespace MyUtil;
using namespace xce::socialgraph;
FriendFetcher* friendFetcher = NULL;
SocialFeedBlockFetcher* socialFeedBlockFetcher = NULL;

void PrintTime();
char* time2str(const time_t ltime, char* str);
void SigRoutine(int dunno);
time_t Begin();
time_t Endup();

int CreateSecondLayer(map<USERID,set<USERID> >& secondLayer, const char* fname);
int CreateFirstLayer(map<USERID,vector<USERID> >& firstLayer,
		const map<USERID,set<USERID> >& secondLayer,
		const map<USERID,vector<USERID> >& userFriends);
int ReadFriendLists(const map<USERID,set<USERID> >& secondLayer,
		map<USERID,vector<USERID> >& userFriends);
int CreateSocialFeedBatch(const map<USERID,vector<USERID> >& firstLayer,
		const map<USERID,set<USERID> >& secondLayer);

int DisplaySocialFeed(const map<USERID,vector<USERID> >& firstLayer);
set<USERID> whiteSet;  
const string log_path = "socialfeed";

int main(int argc, char* argv[])
{
    PrintTime();
//  初始化白名单    
//    const USERID white_list[] = {
//    231815969, 
//    308506621, 
//    258276022, 
//    441381318, 
//    238111132, 
//    256404211,
//    246853992,
//    388143449,
//    388144806,
//    388153007,
//    388198281,
//    390205330,
//    391930993,
//    395173214,
//    395398096,
//    283868200,
//     283887991};
//    whiteSet.clear();
//    whiteSet.insert(white_list, white_list + sizeof(white_list) / sizeof(white_list[0]));
    
    
    INIT_CLOG(log_path);
    //检查输入参数
    if(argc !=4 ||
       access(argv[1], R_OK) != 0 &&
       access(argv[2], R_OK) != 0 &&
       access(argv[3], R_OK) != 0) {
        CLOG(ERROR) << "Wrong input parameters!" ; 
        return -1;
    }

    //创建第二层
	time_t start = Begin();
	time_t phrase_start=0, phrase_end=0;
	phrase_start = time(NULL);
	map<USERID,set<USERID> > secondLayer;
    CLOG(INFO) << "create secondlayer ...";
    bool FileExists = false;
    for(int idx = 1; idx < 4; ++ idx ) {
        if(access(argv[idx], R_OK) == 0) {
            FileExists = true;
            CreateSecondLayer(secondLayer, argv[idx]);
        }    
    }
    if(FileExists == false) {
        CLOG(ERROR) << "No File Exists!";
        return -1;
    }
    phrase_end = time(NULL);
    CLOG(INFO) << "sizeof secondLayer = " << secondLayer.size() 
                <<", using " << (phrase_end-phrase_start)/60.0 << "m";

    //读取好友列表
	phrase_start = time(NULL);
	map<USERID,vector<USERID> > userFriends;
    CLOG(INFO) << "read friend lists ...";
	ReadFriendLists(secondLayer, userFriends);
	phrase_end = time(NULL);
    CLOG(INFO) << "second layer's userFriends size = " << secondLayer.size() 
                <<", using " << (phrase_end-phrase_start)/60.0 << "m";

    //创建第一层
	phrase_start = time(NULL);
	map<USERID,vector<USERID> > firstLayer;
    CLOG(INFO) << "create firstlayer ...";
	CreateFirstLayer(firstLayer, secondLayer, userFriends);
	phrase_end = time(NULL);
    CLOG(INFO) << "sizeof firstLayer = " << firstLayer.size() 
                <<", using " << (phrase_end-phrase_start)/60.0 << "m";

    //批量创建新鲜事数据
	phrase_start = time(NULL);
    CLOG(INFO) << "create social feed (add friends) ...";
	CreateSocialFeedBatch(firstLayer, secondLayer);
	phrase_end = time(NULL);
    CLOG(INFO) << firstLayer.size() << "social feed completed,"
                <<" using " << (phrase_end-phrase_start)/60.0 << "m";

//  展示cache数据
//  DisplaySocialFeed(firstLayer);

//	remove(fname);
	time_t end = Endup();
    CLOG(INFO) << "time used " << end - start << " second";
	return 0;
}

// SigRoutine 
void SigRoutine(int sig)
{
	switch (sig)
	{
		case 1:
            CLOG(INFO) << "Get a signal -- SIGHUP "; 
			break;
		case 2:
            CLOG(INFO) << "Get a signal -- SIGINT ";	
			break;
		case 3:
            CLOG(INFO) << "Get a signal -- SIGQUIT ";
			break;
	}
	Endup();
	exit(0);
}

void PrintTime() 
{
	time_t now = time(NULL);
	char date_str[20];
	time2str(now, date_str);
	CLOG(INFO) << "+------------------------------+";
	CLOG(INFO) << "| " << date_str << "          |";
	CLOG(INFO) << "+------------------------------+|";
}

char* time2str(const time_t ltime, char* str) 
{
	assert(str!=NULL);

	struct tm *Tm = localtime(&ltime);
	sprintf(str, "%04d-%02d-%02d %02d:%02d:%02d",
			Tm->tm_year+1900,
			Tm->tm_mon+1,
			Tm->tm_mday,
			Tm->tm_hour,
			Tm->tm_min,
			Tm->tm_sec);
	return str;
}


time_t Begin() 
{
	signal(SIGHUP, SigRoutine); // 下面设置三个信号的处理方法
	signal(SIGQUIT, SigRoutine);
	signal(SIGINT, SigRoutine);
	//fprintf(stderr, "\033[?25l");  // 隐藏stderr光标
	friendFetcher = new FriendFetcher;
	socialFeedBlockFetcher = new SocialFeedBlockFetcher;

	return time(NULL);
}

time_t Endup() 
{
	CLOG(INFO) << "cleaning jobs ... OK";
	delete friendFetcher;
	delete socialFeedBlockFetcher;
	//fprintf(stderr, "\033[?25h");  // 恢复stderr光标
	return time(NULL);
}

int DisplaySocialFeed(const map<USERID,vector<USERID> >& firstLayer)
{
	TaskManager::instance().config(TASK_LEVEL_DISPLAY_FEEDS, ThreadPoolConfig(0, 100));
	int task_size = 0;
	long wait = 0;
	long count = 0;

	for (map<USERID,vector<USERID> >::const_iterator firstIter = firstLayer.begin();
			firstIter != firstLayer.end(); ++firstIter)
	{

		if (++count % 5000 == 0)
           CLOG(INFO) << "create (" << firstIter->first << "), " << count << " / " << firstLayer.size();
		TaskManager::instance().execute(new DisplayFeedTask(
					firstIter->first));

		while ((task_size=TaskManager::instance().size(TASK_LEVEL_DISPLAY_FEEDS))>5000)
		{
			wait+=2;
			sleep(2);
			if (wait % 60 == 0)
				CLOG(INFO) << "TASK_LEVEL_DISPLAY_FEEDS: sleep " << wait << " second. left " << task_size << " tasks.";
		}
	}
	while (TaskManager::instance().size(TASK_LEVEL_DISPLAY_FEEDS)>0)
	{
		sleep(2);
	}
	return 0;
}

int CreateSocialFeedBatch(const map<USERID,vector<USERID> >& firstLayer, 
		const map<USERID,set<USERID> >& secondLayer)
{
	TaskManager::instance().config(TASK_LEVEL_CREATE_FEEDS, ThreadPoolConfig(0, 100));
	int task_size = 0;
	long wait = 0;
	long count = 0;

	for (map<USERID,vector<USERID> >::const_iterator firstIter = firstLayer.begin();
			firstIter != firstLayer.end(); ++firstIter)
	{
	//	if (firstIter->first % 10 != 2)
	//		continue;
		//if (firstIter->first != 239737004 &&
		//		firstIter->first != 238111132 &&
		//		firstIter->first != 27640369 &&
		//		firstIter->first != 285340707 &&
		//		firstIter->first != 237767538 &&
		//		firstIter->first != 19875 &&
		//		firstIter->first != 1002
		//	 )
		//	continue;
        //白名单测试        
        //if(whiteSet.count(firstIter->first) == 0) continue;

		if (++count % 5000 == 0)
           CLOG(INFO) << "creating (" << firstIter->first << "), " << count << " / " << firstLayer.size();

		TaskManager::instance().execute(new CreateSocailFeedTask(
					firstIter->first, firstIter->second, secondLayer));

		while ((task_size=TaskManager::instance().size(TASK_LEVEL_CREATE_FEEDS))>5000)
		{
			wait+=2;
			sleep(2);
			if (wait % 60 == 0)
				CLOG(INFO) << "TASK_LEVEL_CREATE_FEEDS: sleep " << wait << " second. left " << task_size << " tasks.";
		}
	}
	while (TaskManager::instance().size(TASK_LEVEL_CREATE_FEEDS)>0)
	{
		sleep(2);
	}
	return 0;
} 

int ReadFriendLists(const map<USERID,set<USERID> >& secondLayer, 
		map<USERID,vector<USERID> >& userFriends)
{
	TaskManager::instance().config(TASK_LEVEL_READ_FRIENDS, ThreadPoolConfig(0, 100));
	int task_size = 0;
	long wait = 0;
	long count = 0;

	for (map<USERID,set<USERID> >::const_iterator iter = secondLayer.begin();
			iter != secondLayer.end(); ++iter)
	{
		if (++count % 5000 == 0)
           CLOG(INFO) << count << " / " << secondLayer.size();

		pair<map<USERID,vector<USERID> >::iterator, bool> result =
			userFriends.insert(make_pair<USERID,vector<USERID> >(iter->first, vector<USERID>()));
		TaskManager::instance().execute(
				new ReadFriendTask(iter->first, result.first->second));

		while ((task_size=TaskManager::instance().size(TASK_LEVEL_READ_FRIENDS))>10000)
		{
			wait+=2;
			sleep(2);
			//fprintf(stderr, "sleep %ld second. left %d tasks.\r", wait, task_size);
		}
	}
	CLOG(INFO) << "read friendlist completed.";

	while (TaskManager::instance().size(TASK_LEVEL_READ_FRIENDS)>0)
	{
		sleep(2);
	}
	return 0;
} 

int CreateSecondLayer(map<USERID,set<USERID> >& secondLayer, const char* fname) 
{
	FILE* fp = fopen(fname, "rt");  // addfriend.recently test
	assert(fp!=NULL);
	int uid = 0;
	int fid = 0;
    int acc = 0;
	long line = 0;
	map<USERID,set<USERID> >::iterator iter;

	while (!feof(fp))
	{
		if (++line % 100000 == 0)
		    CLOG(INFO) << line;
           
    fscanf(fp, "%d%d%d\n", &uid, &fid, &acc);
        if(acc == 0) continue;
		//fprintf(stderr, "<%d><%d>\n", uid, fid);
		//-----------------------------

		iter = secondLayer.find(uid);
		if (iter == secondLayer.end())
		{
			pair<map<USERID,set<USERID> >::iterator, bool> result =
				secondLayer.insert(make_pair<USERID,set<USERID> >(uid, set<USERID>()));
			iter =result.first;
		}
		iter->second.insert(fid);
		//-----------------------------
		iter = secondLayer.find(fid);
		if (iter == secondLayer.end())
		{
			pair<map<USERID,set<USERID> >::iterator, bool> result =
				secondLayer.insert(make_pair<USERID,set<USERID> >(fid, set<USERID>()));
			iter =result.first;
		}
		iter->second.insert(uid);
	}
    CLOG(INFO);
	fclose(fp);
	return 0;
} 

int CreateFirstLayer(map<USERID,vector<USERID> >& firstLayer, 
		const map<USERID,set<USERID> >& secondLayer,
		const map<USERID,vector<USERID> >& userFriends)
{
	map<USERID,vector<USERID> >::iterator firstIter;
	map<USERID,vector<USERID> >::const_iterator friendIter;
	long line = 0;

	for (map<USERID,set<USERID> >::const_iterator iter = secondLayer.begin();
			iter != secondLayer.end(); ++iter)
	{
		if (++line % 5000 == 0)
           CLOG(INFO) << line << " / " << secondLayer.size();

		friendIter = userFriends.find(iter->first);
		assert(friendIter != userFriends.end());
		const vector<USERID>& friendlist = friendIter->second;
		for (vector<USERID>::const_iterator fidIter = friendlist.begin();
				fidIter != friendlist.end(); ++fidIter)
		{
			//fprintf(stderr, "%d,", *fidIter);
			firstIter = firstLayer.find(*fidIter);
			if (firstIter == firstLayer.end())
			{
				pair<map<USERID,vector<USERID> >::iterator, bool> result =
					firstLayer.insert(make_pair<USERID,vector<USERID> >(*fidIter, vector<USERID>()));
				firstIter =result.first;
			}
			firstIter->second.push_back(iter->first);
		}
		//getchar();
	}
	return 0;
} 

