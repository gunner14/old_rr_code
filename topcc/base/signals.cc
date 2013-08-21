#include "base/signals.h"

#include <signal.h>
#include <list>
#include <boost/foreach.hpp>
#include <sstream>

#if defined(OS_LINUX) && defined(USE_PROFILER)
#include <google/profiler.h>
#endif

#include "base/common.h"
#include "base/once.h"
#include "base/startuplist.h"
#include "base/globalinit.h"

#ifdef OS_LINUX
extern bool RequestMinidump(const std::string &dump_path);
#elif defined(OS_WIN)
bool RequestMinidump(const std::string &dump_path) {
  return true;
}
#endif

namespace arch {

void InstallSignal(int sig, SignalHandle h) {
#ifdef OS_LINUX
  signal(sig, h);
#elif defined(OS_WIN)
  // windows不支持自定义 signal
#endif
}

// base: log, rotate
// profile: start / stop
// 

static void SignalProfile(int sig) {
#if defined(OS_LINUX) && defined(USE_PROFILER)
  if (ARCH_SIGNAL_PROFILER_START == sig) {
    std::ostringstream ostem;
    ostem << "/tmp/cell_" << getpid() << ".prf";
    ProfilerStart(ostem.str().c_str());
  }
  else if (ARCH_SIGNAL_PROFILER_STOP == sig) 
    ProfilerStop();
#endif
}

static void SignalCrash(int sig) {
  ASSERT(ARCH_SIGNAL_MINIDUMP == sig);
  if (ARCH_SIGNAL_MINIDUMP == sig) {    
    ::RequestMinidump(".");
  } 
  else if (ARCH_SIGNAL_CRASH == sig) {
    *(char *)0 = 'a';
  }
}

static void AddCommonSignal() {
  InstallSignal(ARCH_SIGNAL_PROFILER_START, SignalProfile);
  InstallSignal(ARCH_SIGNAL_PROFILER_STOP, SignalProfile);

  InstallSignal(ARCH_SIGNAL_MINIDUMP, SignalCrash);
  InstallSignal(ARCH_SIGNAL_CRASH, SignalCrash);

  // TODO: log rotate
}

GLOBAL_INIT(COMMON_SIGNALS, {
  AddStartup(&AddCommonSignal);
});

}
