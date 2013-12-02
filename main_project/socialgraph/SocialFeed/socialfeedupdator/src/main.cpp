/*
 * main.cpp
 *
 *  Modified on: Sep 5, 2012
 *       Author: shengqi.chen
 */
#include   "MyTasks.h"

using namespace   MyUtil;
using namespace   xce::socialgraph;

std::set<USERID>        whiteSet;  
/*
FriendFetcher*          friendFetcher = NULL;
SocialFeedBlockFetcher* socialFeedBlockFetcher = NULL;*/

const   string  log_path = "socialfeedupdator";
const   string  CONST_WHITE_USERID_FILE    = "/data/xce/SGProject/data/addfriendfeed/userid_file";
const   long    CONST_MAX_WHITE_USERID_NUM = 30000000;//MAX 3千万 
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

//print time 
void   PrintTime();

//exchange the time to string format
char*  time2str(const time_t ltime, char* str);

//the singal route 
void   SigRoutine(int dunno);

//create friend fetcher, blocked fetcher and set signal route
time_t Begin();

//clern up the resource
time_t Endup();

//get newly added friends of each user in the adding action log file:second layer
int CreateSecondLayer(map<USERID,set<USERID> >& secondLayer, const char* fname);

//get all of the user whose friends have added new firends and user ids of those friends for the user:first layer  
int CreateFirstLayer(map<USERID,vector<USERID> >& firstLayer,
		     const map<USERID,set<USERID> >& secondLayer,
		     const map<USERID,vector<USERID> >& userFriends);

//get friends list for each user who has added new friends 
int ReadFriendLists(const map<USERID,set<USERID> >& secondLayer,
		    map<USERID,vector<USERID> >& userFriends);

//create the socialfeed from the two layers 
int CreateSocialFeedBatch(const map<USERID,vector<USERID> >& firstLayer,
		          const map<USERID,set<USERID> >& secondLayer);

//display the socialfeed for test
int DisplaySocialFeed(const map<USERID,vector<USERID> >& firstLayer);


bool LoadWhiteUserList(string white_userid_file, std::map<int,bool>& whiteusermap);


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]){

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
     
    //check whether the input params are valid
    INIT_CLOG(log_path);
    if(argc !=4 ||
       access(argv[1], R_OK) != 0 &&
       access(argv[2], R_OK) != 0 &&
       access(argv[3], R_OK) != 0) {
       CLOG(ERROR) << "::main() wrong input parameters(files path)!" ; 
       return -1;
    }

    time_t phrase_start=0, phrase_end=0;
    std::map<USERID,std::set<USERID> > secondLayer;

    //0.set singnal route
    time_t start = Begin();

    //1.read the friend newly added records: friends newly added and create the second layer
    CLOG(INFO) << "::main() create secondlayer ...";
    phrase_start = time(NULL);
       bool  FileExists = false;
       for(int idx = 1; idx < 4; ++ idx ) {
	   if(access(argv[idx], R_OK) == 0) {
	      FileExists = true;
	      CreateSecondLayer(secondLayer, argv[idx]);
	   }    
       }
       /*
       if(access("./test", R_OK) == 0) {
          FileExists = true;
          CreateSecondLayer(secondLayer, "./test");
       } */
       if(FileExists == false) {
	   CLOG(ERROR) << "::main() No File Exists!";
	   return -1;
       }
    phrase_end = time(NULL);
    CLOG(INFO) << "::main() size of secondLayer = " << secondLayer.size() 
               <<", using " << (phrase_end-phrase_start)/60.0 << "m";

    //2.read friends list for the second layer
    std::map<USERID,vector<USERID> > userFriends;
    CLOG(INFO) << "::main() read friend lists ...";
        phrase_start = time(NULL);
        ReadFriendLists(secondLayer, userFriends);
        phrase_end   = time(NULL);
    CLOG(INFO) << "::main() second layer's userFriends size = " << secondLayer.size() 
               <<", using " << (phrase_end-phrase_start)/60.0 << "m";

    //3.create first layer 
    map<USERID,vector<USERID> > firstLayer;
    CLOG(INFO) << "::main() create firstlayer ...";
        phrase_start = time(NULL);
        CreateFirstLayer(firstLayer, secondLayer, userFriends);
        phrase_end = time(NULL);
    CLOG(INFO) << "::main() sizeof firstLayer = " << firstLayer.size() 
               <<", using " << (phrase_end-phrase_start)/60.0 << "m";

    //4.create socialfeeds by batch way
    phrase_start = time(NULL);
    CLOG(INFO) << "create social feed ...";
	CreateSocialFeedBatch(firstLayer, secondLayer);
	phrase_end = time(NULL);
    CLOG(INFO) << firstLayer.size() << "social feed completed,"
                <<" using " << (phrase_end-phrase_start)/60.0 << "m";

    //5.display the socialfeed for testing
    //  DisplaySocialFeed(firstLayer);
   
    //6.clern up the resources
    time_t end = Endup();
    CLOG(INFO) << "::main() main has run over,total time used " << end - start << " second";
    return  0;
}

///////////////////////////////////////////////////////
void  SigRoutine(int sig){

	switch (sig){

	  case 1:
            CLOG(INFO) << "::SigRoutine() Get a signal -- SIGHUP "; 
			break;
	  case 2:
            CLOG(INFO) << "::SigRoutine() Get a signal -- SIGINT ";	
			break;
	  case 3:
            CLOG(INFO) << "::SigRoutine() Get a signal -- SIGQUIT ";
			break;
	}
	Endup();
	exit(0);
}

/////////////////////////////////////////////////////////
void PrintTime() {
	time_t now = time(NULL);
	char date_str[20];
	time2str(now, date_str);
	CLOG(INFO) << "+------------------------------+";
	CLOG(INFO) << "| " << date_str << "          |";
	CLOG(INFO) << "+------------------------------+|";
}

/////////////////////////////////////////////////////////
char* time2str(const time_t ltime, char* str) {

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


/////////////////////////////////////////////////////////
time_t Begin(){

	signal(SIGHUP,  SigRoutine); // 下面设置三个信号的处理方法
	signal(SIGQUIT, SigRoutine);
	signal(SIGINT,  SigRoutine);
	//fprintf(stderr, "\033[?25l");  // 隐藏stderr光标
	/*friendFetcher = new FriendFetcher;
	socialFeedBlockFetcher = new SocialFeedBlockFetcher;*/

	return time(NULL);
}


////////////////////////////////////////////////////////
time_t Endup() 
{
	CLOG(INFO) << "::Endup() cleaning jobs ... OK";
   /*     if(friendFetcher){
	   delete  friendFetcher;
        }
        if(socialFeedBlockFetcher){
	   delete  socialFeedBlockFetcher;
        }*/
	//fprintf(stderr, "\033[?25h");  // 恢复stderr光标
	return time(NULL);
}


//////////////////////////////////////////////////////////////////////////////////////////
int DisplaySocialFeed(const map<USERID,vector<USERID> >& firstLayer)
{
	TaskManager::instance().config(TASK_LEVEL_DISPLAY_FEEDS, ThreadPoolConfig(0, 100));
	int task_size = 0;
	long wait     = 0;
	long count    = 0;

	for (map<USERID,vector<USERID> >::const_iterator firstIter = firstLayer.begin();
			firstIter != firstLayer.end(); ++firstIter){

		if (++count % 5000 == 0){
                    CLOG(INFO) << "::DisplaySocialFeed() create (" << firstIter->first << "), " << count << " / " << firstLayer.size();
                }
		TaskManager::instance().execute(new DisplayFeedTask(firstIter->first));
		while ((task_size=TaskManager::instance().size(TASK_LEVEL_DISPLAY_FEEDS))>5000){
			wait+=2;
			sleep(2);
			if (wait % 60 == 0){
			    CLOG(INFO) << "::DisplaySocialFeed() TASK_LEVEL_DISPLAY_FEEDS: sleep " << wait << " second. left " << task_size << " tasks.";
		        }
                }
	}
	while (TaskManager::instance().size(TASK_LEVEL_DISPLAY_FEEDS)>0){
	       sleep(2);
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
int CreateSocialFeedBatch(const map<USERID,vector<USERID> >& firstLayer, 
		          const map<USERID,set<USERID> >& secondLayer)
{
	TaskManager::instance().config(TASK_LEVEL_CREATE_FEEDS, ThreadPoolConfig(0, 100));
	int task_size = 0;
	long wait     = 0;
	long count    = 0;

	for (map<USERID,vector<USERID> >::const_iterator firstIter = firstLayer.begin();
			firstIter != firstLayer.end(); ++firstIter){
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
		if (++count % 5000 == 0){
                   CLOG(INFO) << "::CreateSocialFeedBatch() creating (" << firstIter->first << "), " << count << " / " << firstLayer.size();
                }
		TaskManager::instance().execute(new CreateSocailFeedTask(firstIter->first, firstIter->second, secondLayer));
		while ((task_size=TaskManager::instance().size(TASK_LEVEL_CREATE_FEEDS))>5000){
		   wait += 2;
		   sleep(2);
		   if (wait % 60 == 0){
		      CLOG(INFO) << "TASK_LEVEL_CREATE_FEEDS: sleep " << wait << " second. left " << task_size << " tasks.";
		   }
                }
	}
	while (TaskManager::instance().size(TASK_LEVEL_CREATE_FEEDS)>0){
	       sleep(2);
	}
	return 0;
} 

//////////////////////////////////////////////////////////////////
int ReadFriendLists(const map<USERID,set<USERID> >& secondLayer, 
		    std::map<USERID,vector<USERID> >& userFriends){

	TaskManager::instance().config(TASK_LEVEL_READ_FRIENDS, ThreadPoolConfig(0, 100));
	int  task_size = 0;
	long wait      = 0;
	long count     = 0;

	for (std::map<USERID,set<USERID> >::const_iterator iter = secondLayer.begin();
			iter != secondLayer.end(); ++iter){
		if (++count % 5000 == 0){
                   CLOG(INFO) << "::ReadFriendLists() read secondlayer propertion:" << count << " / " << secondLayer.size();
                }
		pair<map<USERID,vector<USERID> >::iterator, bool> result;
                result = userFriends.insert(make_pair<USERID,vector<USERID> >(iter->first, vector<USERID>()));
		
                TaskManager::instance().execute(new ReadFriendTask(iter->first, result.first->second));
		while ((task_size=TaskManager::instance().size(TASK_LEVEL_READ_FRIENDS))>10000){
			wait += 2;
			sleep(2);
			//fprintf(stderr, "sleep %ld second. left %d tasks.\r", wait, task_size);
		}
	}
	CLOG(INFO) << "::ReadFriendLists() read friendlist for secondlayer completed.";

	while (TaskManager::instance().size(TASK_LEVEL_READ_FRIENDS)>0){
	       sleep(2);
	}
	return 0;
} 

////////////////////////////////////////////////////////////////////////////////////
int CreateSecondLayer(std::map<USERID,set<USERID> >& secondLayer, const char* fname) 
{
	FILE* fp = fopen(fname, "rt");  // addfriend.recently test
	assert(fp!=NULL);
	int  uid = 0;
	int  fid = 0;
        int  acc = 0;
	long line = 0;
	std::map<USERID,set<USERID> >::iterator iter;
        //get each user's newly added friends from the adding action log file
	while (!feof(fp)){
		if (++line % 100000 == 0){
		    CLOG(INFO) << "::CreateSecondLayer() lines number now:" << line;
                }
                fscanf(fp, "%d%d%d\n", &uid, &fid, &acc);
           //     printf("%d %d %d\n",uid,fid,acc);
                if (acc == 0){ 
                   continue;
                }
		//fprintf(stderr, "<%d><%d>\n", uid, fid);
		//-----------------------------
		iter = secondLayer.find(uid);
		if (iter == secondLayer.end()){
		    pair<map<USERID,set<USERID> >::iterator, bool> result;
                    result = secondLayer.insert(make_pair<USERID,set<USERID> >(uid, set<USERID>()));
		    iter   = result.first;
		}
		iter->second.insert(fid);
		//-----------------------------
		iter = secondLayer.find(fid);
		if (iter == secondLayer.end()){
	            pair<map<USERID,set<USERID> >::iterator, bool> result;
                    result = secondLayer.insert(make_pair<USERID,set<USERID> >(fid, set<USERID>()));
	            iter   = result.first;
		}
		iter->second.insert(uid);
	}
        CLOG(INFO)<< "::CreateSecondLayer() computing the friends adding log file:" << fname << " over!";
	fclose(fp);
	return  0;
} 


///////////////////////////////////////////////////////////////
int CreateFirstLayer(map<USERID,vector<USERID> >& firstLayer, 
		     const map<USERID,set<USERID> >& secondLayer,
		     const map<USERID,vector<USERID> >& userFriends){

	map<USERID,vector<USERID> >::iterator firstIter;
	map<USERID,vector<USERID> >::const_iterator friendIter;
	long line    = 0;
        long counter = 0;
        string  white_userid_file = CONST_WHITE_USERID_FILE;
        std::map<int, bool>  whiteusermap;
        if (!LoadWhiteUserList(white_userid_file, whiteusermap)) { 
            return -1; 
        }
        //let all friends of the user who added friends to know the adding action 
	for (map<USERID,set<USERID> >::const_iterator iter = secondLayer.begin();
             iter != secondLayer.end() && counter < CONST_MAX_WHITE_USERID_NUM; ++iter){
		if (++line % 5000 == 0){
                   CLOG(INFO) << "::CreateFirstLayer() read secondlayer proportion:" << line << " / " << secondLayer.size();
                }
                //get friends by uid
		friendIter = userFriends.find(iter->first);
		assert(friendIter != userFriends.end());
		const vector<USERID>& friendlist = friendIter->second;
                //for each friend of one user:tell all of his friends the action of adding friends    
		for (vector<USERID>::const_iterator fidIter = friendlist.begin();
		     fidIter != friendlist.end(); ++fidIter){
                        //filter:not the target 
                        if (whiteusermap.find(*fidIter) == whiteusermap.end()){
                            continue;
                        }
                        counter ++;
			firstIter = firstLayer.find(*fidIter);
			if (firstIter == firstLayer.end()){
			    pair<map<USERID,vector<USERID> >::iterator, bool> result;
                            result    = firstLayer.insert(make_pair<USERID,vector<USERID> >(*fidIter, vector<USERID>()));
			    firstIter = result.first;
			}
			firstIter->second.push_back(iter->first);
		}
                //getchar();
	 }
	 return 0;
} 

bool LoadWhiteUserList(string white_userid_file, std::map<int,bool>& whiteusermap){
          FILE*   fp = fopen(white_userid_file.c_str(), "r");
          if (NULL == fp){
              CLOG(ERROR) << "LoadWhiteUserList target userid file is not exist:" << white_userid_file;
              return false;
          }
          CLOG(INFO) << "LoadWhiteUserList loading...";
          whiteusermap.clear();
          char* charptr = NULL;
          ssize_t  read;
          size_t   len;
          while ((read = getline(&charptr, &len, fp)) != -1){
                 std::string  buffer = charptr;
                 size_t    bufferlen = buffer.size();
                 if (bufferlen == 0) {
                     continue;
                 }
                 if (buffer[bufferlen - 1] == '\r') {
                     buffer.resize (bufferlen - 1);
                 }
                 int tempuserid = (int)atoi(buffer.c_str());
                 std::pair<std::map<int, bool>::iterator, bool> pr = whiteusermap.insert(pair<int, bool>(tempuserid,true));
                 if (!pr.second){
                     CLOG(WARN) << "LoadWhiteUserList Userid File insert userid  failed: " << tempuserid;
                 }
                 if (charptr) {
                     free(charptr);
                     charptr = NULL;
                 }
          }
          fclose(fp);
          CLOG(INFO) << "LoadWhiteUserList loaded ok";
          return  true;
}
