#include <set>
#include <ostream>
#include <unistd.h>
#include <sys/time.h>
#include "FeedPipe.h"
#include "RFeed.h"
#include "Tasks.h"
using namespace MyUtil;
using namespace xce::feed;

//--------------------------------------------------------------------------------
void RemovePipe::handle(const ObjectSeq& seq) {

  MCE_INFO("RemovePipe::handle --> thread_id = " << pthread_self() << " seq size:" << seq.size());

  if (seq.empty()) {
    return;
  } 
  std::set<std::string> removeKeySet; 
  std::size_t si = seq.size();

  int j = 0;
  timeval beg, end;
  gettimeofday(&beg, NULL);

  for (size_t i = 0; i < seq.size(); ++i) {

    RemoveKeyPtr st = RemoveKeyPtr::dynamicCast(seq.at(i));
    ostringstream oss;
    oss << "s:" << st->source << " t:" << st->type << " op:" << st->op;
    string key = oss.str();
    if(removeKeySet.count(key)) {
      j++;
      continue;
    } else {
      gettimeofday(&end, NULL);
      float timeuse=1000000*(end.tv_sec-beg.tv_sec)+end.tv_usec-beg.tv_usec;
      timeuse/=1000;
      MCE_INFO("RemovePipe::handle --> execute task. source:" << st->source << " type:" << st->type << " op:" << st->op << " i:" << i << " seq_size:" << si << " last dup:" << j << " time:" << timeuse); 
      j = 0;
      removeKeySet.insert(key);
      if(st->op == BySource) {
        TaskManager::instance().execute(new RemoveFeedBySourceStypeTask(st->source, st->type));
      } else {
        TaskManager::instance().execute(new RemoveFeedByParentTask(st->source, st->type));
      }
      gettimeofday(&beg, NULL);
      struct timeval tv = { 0, 100 * 1000 };
      select(0, NULL, NULL, NULL, &tv);
    }
  }
  MCE_INFO("RemovePipe::handle --> loop end. last dup:" << j);

}

