#ifndef _UTIL_LOG_H_
#define _UTIL_LOG_H_
#include <stdint.h>
#include <string>
#include <iostream>
#include <sstream>

namespace arch {

enum LoggingSeverity { LS_VERBOSE, 
	LS_INFO, 
	LS_WARNING, 
	LS_ERROR};

enum LogErrorContext { ERRCTX_NONE, ERRCTX_ERRNO, ERRCTX_HRESULT };

class LogMessage {
 public:
  LogMessage(const char* file, int line, LoggingSeverity sev,
             LogErrorContext err_ctx = ERRCTX_NONE, int err = 0,
             const char* module = NULL);
  ~LogMessage();

  static inline bool Loggable(LoggingSeverity sev) { return (sev >= min_sev_); }
  std::ostream& stream() { return print_stream_; }

  enum { NO_LOGGING = LS_ERROR + 1 };

  static void LogContext(int min_sev);
 
  static void LogThreads(bool on = true);
  
  static void LogTimestamps(bool on = true);

  static void ResetTimestamps();

  static void LogToDebug(int min_sev);
  static void LogToStream(std::ostream* stream, int min_sev);

 private:
 
  static const char* Describe(LoggingSeverity sev);
  static const char* DescribeFile(const char* file);

  std::ostringstream print_stream_;

  LoggingSeverity severity_;

  std::string extra_;

  static int min_sev_, dbg_sev_, stream_sev_, ctx_sev_;

  static std::ostream * stream_;

  static bool thread_, timestamp_;

  static uint32_t start_;

  static bool is_diagnostic_mode_;


  LogMessage(const LogMessage&); void operator=(const LogMessage&);
};

void LogToDir(const char* path, int min_sev, int secondofday);

#define LOG(sev) \
  if (::arch::LogMessage::Loggable(::arch::sev)) \
    ::arch::LogMessage(__FILE__, __LINE__, ::arch::sev).stream()

#define LOGG_V(sev) \
  if (::arch::LogMessage::Loggable(sev)) \
    ::arch::LogMessage(__FILE__, __LINE__, sev).stream()

#define LOG_F(sev) LOG(sev) << __PRETTY_FUNCTION__  << ": "

}
#endif
