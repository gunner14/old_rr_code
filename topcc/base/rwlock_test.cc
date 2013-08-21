#include <gtest/gtest.h>
#include <iostream>
#include <boost/thread.hpp>

#include "base/rwlock.h"
#include "base/common.h"

static bool quit_ = false;
unsigned count = 0;
arch::ReadWriteLock lock;

void ReaderProc() {
  while(!quit_) {
    lock.ReadLock();
    std::cout << "cnt = " << count << std::endl;
    lock.ReadUnlock();
    arch::Sleep(50);
  }
}

void WriterProc() {
  while(!quit_) {
    lock.WriteLock();
    std::cout << "writing count to " << ++ count << std::endl;
    arch::Sleep(1);
    lock.WriteUnlock();

    arch::Sleep(100);
  }
}

TEST(ReadwriteLockTest, Stop) {
  boost::thread_group g;

  for (int i = 0; i < 20; ++i)
    g.create_thread(&ReaderProc);

  g.create_thread(&WriterProc);
  g.create_thread(&WriterProc);

  arch::Sleep(2000);
  quit_ = true;

  g.join_all();
}
