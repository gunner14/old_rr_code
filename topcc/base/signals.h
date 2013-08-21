#ifndef ARCH_BASE_SIGNALS_H__
#define ARCH_BASE_SIGNALS_H__

// ----------------------------------------------------
// log, Rotate
#define ARCH_SIGNAL_LOG_OPEN     35
#define ARCH_SIGNAL_LOG_ROTATE   36

// profile
#define ARCH_SIGNAL_PROFILER_START      37
#define ARCH_SIGNAL_PROFILER_STOP       38

// ----------------------------------------------------
// config, 本地缓存的map加载，导出
#define ARCH_SIGNAL_CONFIG

// ----------------------------------------------------
// breakpad
#define ARCH_SIGNAL_MINIDUMP           45
#define ARCH_SIGNAL_CRASH              46

// ----------------------------------------------------
// cache, 加载/Dump
#define ARCH_SIGNAL_CACHE_DUMP         50
#define ARCH_SIGNAL_CACHE_DUMP_ROTATE  51
#define ARCH_SIGNAL_CACHE_RESTORE      52

// ----------------------------------------------------
// for cwf
#define ARCH_SIGNAL_CWF_LOAD_TEMPLATE  60

namespace arch {

typedef void (*SignalHandle)(int);

void InstallSignal(int sig, SignalHandle);

}
#endif // ARCH_BASE_SIGNALS_H__
