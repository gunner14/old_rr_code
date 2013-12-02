#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <sstream>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/lexical_cast.hpp>
#include "TaskManager.h"
#include "socialgraph/recommendcontent/PageModelCreator/RcdPageWriter/PageCacheTask.h"

using namespace xce::socialgraph;

const int kLevel_Rcd_UserPage = 4;
int main(int argc, char** argv) {
  MyUtil::TaskManager::instance().config(kLevel_Rcd_UserPage, MyUtil::ThreadPoolConfig(8,100)); 
  std::ifstream fin("/data/xce/yize.tan/recommendpage/result_part");
  if (!fin) {
    exit(-1);
  }
  InitLogger("recommendPage");
  std::string line;
  int user_id;
  std::string value;
  std::vector<int> seq;
  std::istringstream stream(value);
  std::set<int> recommend_seq;
  std::map<int, std::set<int> >recommend_result;
  int count  = 1;
  int task_size = 0;
  while(std::getline(fin, line)) {
    value = "";
    recommend_seq.clear();
    std::istringstream os(line);
    os >> user_id;
    os >> value;
    std::istringstream stream(value);
    while(std::getline(stream, value, ',')) {
      int rcd_id = boost::lexical_cast<int>(value);
      recommend_seq.insert(rcd_id);
    }
    if (user_id != 0 && recommend_seq.size() > 0) {
      MyUtil::TaskManager::instance().execute(new SetPageToCacheTask(user_id, recommend_seq));
      //RcdPageCacheHelper::setRcdPageIntoCache(user_id, recommend_seq);
      while((task_size=MyUtil::TaskManager::instance().size(kLevel_Rcd_UserPage)) > 1000){
        sleep(2);
      }
      count ++;
      if( count % 10000 == 0) {
        CLOG(INFO) << "sucessfully do " << count << " results";
      }
    }
  }
  while(MyUtil::TaskManager::instance().size(kLevel_Rcd_UserPage) > 0) {
    sleep(60);
  }
  CLOG(INFO) << count << "users have been processed!";
  fin.close();
  cin >> user_id; 
}
