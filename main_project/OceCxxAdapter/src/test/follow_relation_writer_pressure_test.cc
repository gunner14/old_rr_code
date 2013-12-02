#include "../follow_relation_writer_adapter.h"
#include "util/cpp/TimeCost.h"
#include "TaskManager.h"
#include <unistd.h>
#include <iostream>
#include <cstdlib>

#define TASK_LEVEL_WRITE     1
#define MAX_TASK_SIZE        10000 

using namespace std;
using xce::buddy::adapter::FollowRelationWriterAdapter;
using MyUtil::TaskManager;
using MyUtil::ThreadPoolConfig;

class WriteTask: virtual public MyUtil::Task {
public:
    WriteTask(int type, int gid, int uid ) :
        MyUtil::Task(TASK_LEVEL_WRITE),type_(type),gid_(gid),uid_(uid){
    }

    virtual void handle() {
        try {
          FollowRelationWriterAdapter::instance().AddFollowRelation(new MyUtil::GlobalId(type_, gid_), uid_);
        } catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
private:
    int type_;
    int gid_;
    int uid_;
};

void  addtest(int typecount, int gidcount, int ucountforgid, int minthcount, int maxthcount) {
  TaskManager::instance().config(TASK_LEVEL_WRITE, ThreadPoolConfig(
          minthcount, maxthcount));
  ofstream outfile;
  outfile.open("tttemp.txt");

  int gid;
  int uid;
  int type;

  std::set<int> typeset;
  std::set<int> gidset;
  std::set<int> uidset;

  for(int j=0; j< typecount;++j){
    do{
      type = rand ()% 100;     
    }while(typeset.find(type) != typeset.end());
    typeset.insert(type);

    gidset.clear();
    for (int temp =0;temp <gidcount;++temp) {
      do{
        gid = rand() % 10 * 100000000 + rand() % 10000 * 10000 + rand() % 10000;
      }while(gidset.find(gid) != gidset.end());
      gidset.insert(gid);

      outfile << -1 << "\t" << gid << "\t"<< type << "\t" << ucountforgid << "\n";

      uidset.clear();
      for(int i=0; i< ucountforgid ; ++i) { 
        do{
          uid = rand() % 10 * 100000000 + rand() % 10000 * 10000 + rand() % 10000;
        }while(uidset.find(uid) != uidset.end());
        uidset.insert(uid);

        outfile << uid <<"\n";
        while(TaskManager::instance().size(TASK_LEVEL_WRITE) >= MAX_TASK_SIZE){ 
          std::cout << "Max task size reached! TaskManager size: " << TaskManager::instance().size(TASK_LEVEL_WRITE) << ", typecount:" << j << ", gidcount:" << temp << ", uidcount:" << i << std::endl;
          sleep(1);
        }
        TaskManager::instance().execute(new WriteTask(type, gid, uid));
        std::cout << "TaskManager waitThreadSize: " << TaskManager::instance().waitThreadSize(TASK_LEVEL_WRITE) << " runningThreadSize:" << TaskManager::instance().runningThreadSize(TASK_LEVEL_WRITE) << " completedThreadSize:" << TaskManager::instance().completedTaskSize(TASK_LEVEL_WRITE) << " queueSize:" << TaskManager::instance().size(TASK_LEVEL_WRITE) << std::endl;
//usleep(10000); // 10ms
      }

    outfile << "\n";
    }
  }
  outfile.close();
}


void  removetest() {
  ifstream infile;
  infile.open("tttemp.txt");

  int gid;
  int type,uid,ucountforgid;

  int temp;

  while( infile >> temp ) {
    if(temp = -1 ) {
      infile >> gid >> type >> ucountforgid;
     
      for(int i=0; i< ucountforgid ; ++i) {
        infile >> uid;
        using namespace xce::buddy::adapter;
        FollowRelationWriterAdapter::instance().RemoveFollowRelation(new MyUtil::GlobalId(type,gid), uid);
      }
    }
  }
  infile.close();
}






int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: %s add/remove(0 for add, 1 for remove) [typecount] [gidcount] [uidcount] [minthcount] [maxthcount]\n",argv[0]);
    exit(1);
  }

  srand ((unsigned)time (0));
  int opt = atoi(argv[1]);
  int typecount = 10;
  int gidcount = 100;
  int uidcount = 1000;
  int minthcount = 1;
  int maxthcount = 8;
  if (argc > 2)
    typecount = atoi(argv[2]);
  if (argc > 3)
    gidcount = atoi(argv[3]);
  if (argc > 4)
    uidcount = atoi(argv[4]);
  if (argc > 5)
    minthcount = atoi(argv[5]);
  if (argc > 6)
    maxthcount = atoi(argv[6]);

  switch(opt){
    case 0:
      addtest(typecount, gidcount,uidcount,minthcount,maxthcount);
      break;
    case 1:
      removetest();
      break;
    default:
      break;
  }

  return 0;
}

