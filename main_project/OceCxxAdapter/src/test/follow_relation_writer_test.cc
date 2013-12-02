#include <iostream>
#include "../follow_relation_writer_adapter.h"

using xce::buddy::adapter::FollowRelationWriterAdapter;

int main(int argc, char *argv[]){
  if (argc < 4 ) {
    printf("Usage: %s type, id, uid add/remove(0 for add, 1 for remove)\n",argv[0]);
    exit(1);
  }
 
  int type = atoi(argv[1]);
  int id = atoi(argv[2]);
  int uid = atoi(argv[3]);
  int opt = atoi(argv[4]);

  switch(opt){
    case 0:
      try{
        FollowRelationWriterAdapter::instance().AddFollowRelation(new MyUtil::GlobalId(type,id), uid);
      } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
      }
      break;
    case 1:
      try {
        FollowRelationWriterAdapter::instance().RemoveFollowRelation(new MyUtil::GlobalId(type,id), uid);
      } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
      }
      break;
    default:
      break;
  }
}
