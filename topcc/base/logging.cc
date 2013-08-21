#include "logging.h"
#include <pthread.h>
#include <cstring> // for ::strchr
#include <iostream>
#include <iomanip>
#include <cstdio> // for snprintf
#include <ctime> // for snprintf
#include <fstream>

namespace arch  {

template<class T> inline T _min(T a, T b) { return (a > b) ? b : a; }
template<class T> inline T _max(T a, T b) { return (a < b) ? b : a; }

int LogMessage::min_sev_ = LS_INFO;
int LogMessage::dbg_sev_ = LS_INFO;
int LogMessage::stream_sev_ = NO_LOGGING;
int LogMessage::ctx_sev_ = LS_WARNING;
std::ostream* LogMessage::stream_;

bool LogMessage::thread_ = true;
bool LogMessage::timestamp_ = true;
uint32_t LogMessage::start_ = 0;

static time_t log_time_ = 0;
static pthread_mutex_t glock_ = PTHREAD_MUTEX_INITIALIZER;
static char log_path_[2048];
static int min_level_;
static char outlog_[2048];

void CreateLog() {
  std::ofstream* fstem = new std::ofstream(outlog_, std::ios::binary);
  LogMessage::LogToStream(fstem, min_level_);
}

void CheckTime(time_t t) {
  if (t > log_time_ && log_time_ != 0) {
    pthread_mutex_lock(&glock_);
    if (t > log_time_) {
      char newfile[2048];
      tm* tt = localtime(&log_time_);
      strcpy(newfile, log_path_);
      int len = strlen(log_path_);
      strftime(newfile+len, 2048 - len, "/out.%Y-%m-%d.log",tt);
      int err = rename(outlog_, newfile);
      CreateLog();
      log_time_ += 86400;
    }
    pthread_mutex_unlock(&glock_);
  }
}

void LogToDir(const char* path, int min_sev, int secondofday) {
  strcpy(log_path_, path);
  min_level_ = min_sev;
  time_t now = time(NULL);
  now = (now / 86400) * 86400;
  log_time_ = now + secondofday;
  strcpy(outlog_, log_path_);
  strcat(outlog_, "/out.log");
  CreateLog();
}

LogMessage::LogMessage(const char* file, int line, LoggingSeverity sev,
                       LogErrorContext err_ctx, int err, const char* module)
    : severity_(sev) {
  if (timestamp_) {

    struct ::tm tm_time;
    time_t timestamp = time(0);
    
    CheckTime(timestamp); // check time and change file name

    localtime_r(&timestamp, &tm_time);

    print_stream_ << "[" << std::setfill('0')
      << std::setw(2) << tm_time.tm_hour << ":"
          << std::setw(2) << tm_time.tm_min << ":"
          << std::setw(2) << tm_time.tm_sec
          << "] ";
  }

  if (thread_) {
    uint32_t id = pthread_self();
    print_stream_ << "[" << std::hex << id << std::dec << "] ";
  }

  if (severity_ >= ctx_sev_) {
    print_stream_ << Describe(sev) << "(" << DescribeFile(file)
                  << ":" << line << "): ";
  }

  if (err_ctx != ERRCTX_NONE) {
    std::ostringstream tmp;
    tmp << "[0x" << std::setfill('0') << std::hex << std::setw(8) << err << "]";
    switch (err_ctx) {
      case ERRCTX_ERRNO:
        tmp << " " << strerror(err);
        break;
      default:
        break;
    }
    extra_ = tmp.str();
  }
}

LogMessage::~LogMessage() {
  if (!extra_.empty())
    print_stream_ << " : " << extra_;
  print_stream_ << std::endl;
  const std::string& str = print_stream_.str();

  if (severity_ >= dbg_sev_) {
    bool log_to_stderr = true;
    if (log_to_stderr) {
      std::cerr << str;
      std::cerr.flush();
    }
  }

  if (severity_ >= stream_sev_) {
     stream_->write(str.data(), str.size());
    stream_->flush();
  }
}

void LogMessage::LogContext(int min_sev) {
  ctx_sev_ = min_sev;
}

void LogMessage::LogThreads(bool on) {
  thread_ = on;
}

void LogMessage::LogTimestamps(bool on) {
  timestamp_ = on;
}

void LogMessage::ResetTimestamps() {
  start_ = 0;
}

void LogMessage::LogToDebug(int min_sev) {
  dbg_sev_ = min_sev;
  min_sev_ = _min(dbg_sev_, stream_sev_);
}

void LogMessage::LogToStream(std::ostream* stream, int min_sev) {
  delete stream_;
  stream_ = stream;
  stream_sev_ = (stream_ == 0) ? NO_LOGGING : min_sev;
  min_sev_ = _min(dbg_sev_, stream_sev_);
}

const char* LogMessage::Describe(LoggingSeverity sev) {
  switch (sev) {
  case LS_VERBOSE:   return "Verbose";
  case LS_INFO:      return "Info";
  case LS_WARNING:   return "Warning";
  case LS_ERROR:     return "Error";
  default:           return "<unknown>";
  }
}

const char* LogMessage::DescribeFile(const char* file) {
  const char* end1 = ::strrchr(file, '/');
  const char* end2 = ::strrchr(file, '\\');
  if (!end1 && !end2)
    return file;
  else
    return (end1 > end2) ? end1 + 1 : end2 + 1;
}

} //namespace
