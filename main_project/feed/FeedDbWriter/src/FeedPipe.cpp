#include "FeedPipe.h"
#include "RFeed.h"
#include "FeedDbWriterI.h"
#include "TaskManager.h"
#include <set>
#include <ostream>
#include <unistd.h>
#include <sys/time.h>
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

    RemoveFeedFromDBKeyPtr st = RemoveFeedFromDBKeyPtr::dynamicCast(seq.at(i));
    if (st->indexSeq.empty()) {
      continue;
    }
    gettimeofday(&end, NULL);
    float timeuse=1000000*(end.tv_sec-beg.tv_sec)+end.tv_usec-beg.tv_usec;
    timeuse/=1000;
    MCE_INFO("RemovePipe::handle --> execute task. source:" << st->indexSeq[0]->source << " type:" << st->indexSeq[0]->stype << " i:" << i << " seq_size:" << si  << " time:" << timeuse); 
    TaskManager::instance().execute(new RemoveFeedTask(st->indexSeq));
    gettimeofday(&beg, NULL);
    struct timeval tv = { 0, 100 * 1000 };
    select(0, NULL, NULL, NULL, &tv);
  }
  MCE_INFO("RemovePipe::handle --> loop end. last dup:" << j);
}

