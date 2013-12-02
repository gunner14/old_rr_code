#ifdef WIN32
#include <windows.h>
#define snprintf _snprintf
#endif

#ifdef POSIX
#include <pthread.h>
#endif

#include <cstring> // for ::strchr
#include <iostream>
#include <iomanip>
#include <cstdio> // for snprintf
#include <ctime> // for snprintf

#include "feed/base_feed/logging.h"
#include "feed/base_feed/stringencode.h"

namespace {
template<class T> inline T _min(T a, T b) { return (a > b) ? b : a; }
template<class T> inline T _max(T a, T b) { return (a < b) ? b : a; }
}

namespace xce {

/////////////////////////////////////////////////////////////////////////////
// Constant Labels
/////////////////////////////////////////////////////////////////////////////

const char * FindLabel(int value, const xce::ConstantLabel entries[]) {
  for (int i=0; entries[i].label; ++i) {
    if (value == entries[i].value) {
      return entries[i].label;
    }
  }
  return 0;
}

std::string ErrorName(int err, const xce::ConstantLabel * err_table) {
	if (err == 0)
		return "No error";

  if (err_table != 0) {
    if (const char * value = FindLabel(err, err_table))
      return value;
  }
  
  char buffer[16];
  snprintf(buffer, sizeof(buffer), "0x%08x", err);
  return buffer;
}

/////////////////////////////////////////////////////////////////////////////
// LogMessage
/////////////////////////////////////////////////////////////////////////////

// #if _DEBUG
static const int LOG_DEFAULT = LS_INFO;
// #else  // !_DEBUG
// static const int LOG_DEFAULT = LogMessage::NO_LOGGING;
// #endif  // !_DEBUG

// By default, release builds don't log, debug builds at info level
int LogMessage::min_sev_ = LOG_DEFAULT;
int LogMessage::dbg_sev_ = LOG_DEFAULT;

// No file logging by default
int LogMessage::stream_sev_ = NO_LOGGING;

// Don't bother printing context for the ubiquitous INFO log messages
int LogMessage::ctx_sev_ = LS_WARNING;

// stream_ defaults to NULL
// Note: we explicitly do not clean this up, because of the uncertain ordering
// of destructors at program exit.  Let the person who sets the stream trigger
// cleanup by setting to NULL, or let it leak (safe at program exit).
std::ostream* LogMessage::stream_;

// Boolean options default to true
bool LogMessage::thread_ = true;
bool LogMessage::timestamp_ = true;

// Program start time
uint32 LogMessage::start_ = 0;

// if we're in diagnostic mode, we'll be explicitly set that way.  default to false
bool LogMessage::is_diagnostic_mode_ = false;

LogMessage::LogMessage(const char* file, int line, LoggingSeverity sev,
                       LogErrorContext err_ctx, int err, const char* module)
    : severity_(sev) {
  if (timestamp_) {

    struct ::tm tm_time;
    time_t timestamp = time(0);
#ifdef WIN32
    localtime_s(&tm_time, &timestamp);
#else
    localtime_r(&timestamp, &tm_time);
#endif    

    print_stream_ << "[" << std::setfill('0')
      << std::setw(2) << tm_time.tm_hour << ":"
          << std::setw(2) << tm_time.tm_min << ":"
          << std::setw(2) << tm_time.tm_sec
          << "] ";
  }

  if (thread_) {
#ifdef WIN32
    DWORD id = GetCurrentThreadId();
    print_stream_ << "[" << std::hex << id << std::dec << "] ";
#elif defined(POSIX)
    uint32 id = pthread_self();
    print_stream_ << "[" << std::hex << id << std::dec << "] ";
#endif
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
  #ifdef WIN32
      case ERRCTX_HRESULT: {
        char msgbuf[256];
        DWORD flags = FORMAT_MESSAGE_FROM_SYSTEM;
        HMODULE hmod = GetModuleHandleA(module);
        if (hmod)
          flags |= FORMAT_MESSAGE_FROM_HMODULE;
        if (DWORD len = FormatMessageA(
            flags, hmod, err,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            msgbuf, sizeof(msgbuf) / sizeof(msgbuf[0]), NULL)) {
          while ((len > 0) &&
              isspace(static_cast<unsigned char>(msgbuf[len-1]))) {
            msgbuf[--len] = 0;
          }
          tmp << " " << msgbuf;
        }
        break; }
  #endif  // WIN32
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
#ifdef WIN32
    static bool debugger_present = (IsDebuggerPresent() != FALSE);
    if (debugger_present) {
      log_to_stderr = false;
      OutputDebugStringA(str.c_str());
    }
    if (log_to_stderr) {
      // This handles dynamically allocated consoles, too.
      if (HANDLE error_handle = ::GetStdHandle(STD_ERROR_HANDLE)) {
        log_to_stderr = false;
        unsigned long written;
        ::WriteFile(error_handle, str.data(), str.size(), &written, 0);
      }
    }
#endif  // WIN32
    if (log_to_stderr) {
      std::cerr << str;
      std::cerr.flush();
    }
  }

  if (severity_ >= stream_sev_) {
    // If write isn't fully successful, what are we going to do, log it? :)
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

//////////////////////////////////////////////////////////////////////
// Logging Helpers
//////////////////////////////////////////////////////////////////////
#if 1
void LogMultiline(LoggingSeverity level, const char* label, bool input,
                  const char * data, size_t len, bool hex_mode,
                  LogMultilineState* state) {
  if (!LOG_CHECK_LEVEL_V(level))
    return;

  const char * direction = (input ? " << " : " >> ");
  if (hex_mode) {
    const size_t LINE_SIZE = 24;
    char hex_line[LINE_SIZE * 9 / 4 + 2], asc_line[LINE_SIZE + 1];
    while (len > 0) {
      memset(asc_line, ' ', sizeof(asc_line));
      memset(hex_line, ' ', sizeof(hex_line));
      size_t line_len = _min(len, LINE_SIZE);
      for (size_t i=0; i<line_len; ++i) {
        unsigned char ch = static_cast<unsigned char>(data[i]);
        asc_line[i] = isprint(ch) ? data[i] : '.';
        hex_line[i*2 + i/4] = hex_encode(ch >> 4);
        hex_line[i*2 + i/4 + 1] = hex_encode(ch & 0xf);
      }
      asc_line[sizeof(asc_line)-1] = 0;
      hex_line[sizeof(hex_line)-1] = 0;
      LOG_V(level) << label << direction 
                   << asc_line << " " << hex_line << " ";
      data += line_len;
      len -= line_len;
    }
    return;
  }

  size_t consecutive_unprintable = state ? state->unprintable_count_ : 0;

  std::string str(data, len);
  while (!str.empty()) {
    size_t line_end_length = 0;
    std::string::size_type pos = str.find('\n');
    std::string substr = str;
    if (pos == std::string::npos) {
      substr = str;
      str.clear();
    } else if ((pos > 0) && (str[pos-1] == '\r')) {
      line_end_length = 2;
      substr = str.substr(0, pos - 1);
      str = str.substr(pos + 1);
    } else {
      line_end_length = 1;
      substr = str.substr(0, pos);
      str = str.substr(pos + 1);
    }

    // Any lines which consist entirely of ascii characters are printed.  Other
    // lines are considered binary, and we just count the number of bytes.
    // This algorithm should be very compatible with HTTP transfers of binary
    // data.
    bool is_ascii = true, is_whitespace = true;
    for (size_t i=0; i<substr.size(); ++i) {
      unsigned char ch = static_cast<unsigned char>(substr[i]);
      if (!isprint(ch)) {
        is_ascii = false;
        break;
      } else if (!isspace(static_cast<unsigned char>(ch))) {
        is_whitespace = false;
      }
    }
    // Treat an empty line following binary data as binary.
    if (is_whitespace && consecutive_unprintable) {
      is_ascii = false;
    }
    if (!is_ascii) {
      consecutive_unprintable += substr.size() + line_end_length;
    }
    if (consecutive_unprintable && (is_ascii || str.empty())) {
      LOG_V(level) << label << direction << "## " << consecutive_unprintable
                   << " consecutive unprintable ##";
    }
    if (is_ascii) {
      consecutive_unprintable = 0;
    } else {
      continue;
    }
#if 0
    // Filter out any private data
    std::string::size_type pos_private = substr.find("Email");
    if (pos_private == std::string::npos) {
      pos_private = substr.find("Passwd");
    }
    if (pos_private == std::string::npos) {
      LOG_V(level) << label << direction << substr;
    } else {
      LOG_V(level) << label << direction << "## omitted for privacy ##";
    }
#endif
  }

  if (state) {
    state->unprintable_count_ = consecutive_unprintable;
  }
}

//////////////////////////////////////////////////////////////////////
#endif
} // namespace xce
