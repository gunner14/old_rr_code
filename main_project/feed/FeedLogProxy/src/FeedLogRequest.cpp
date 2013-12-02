#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/regex.hpp>
#include "FeedLogRequest.h"
#include "FeedLogServerReplicaAdapter.h"

using namespace xce::notify;
using namespace xce::feed;
using namespace MyUtil;
using namespace boost;

//pv log
bool PvRequest::response() {
  bool result = false;
  if (!_fcgi_out) {
    MCE_WARN("PvRequest::response --> fcgi_out is null");
    return result;
  }
  string log = getProperty("log");
  if (log.empty()) {
    MCE_WARN("PvRequest::response --> pv log is empty");
  } else {
    StrSeq log_seq = LogStringParser::instance().LogSpliter(gPv, log);
    if (log_seq.size() == 0) {
      MCE_WARN("PvRequest::response --> log spliter result is empty. input is: " << log);
    } else {
      try {
        FeedLogServerReplicaAdapter::instance().PutLogBatch(gPv, log_seq);
        result = true;
      } catch (Ice::Exception& e) {
        MCE_WARN("PvRequest::response --> adapter err:" << e << ",File:" <<__FILE__ << ",Line:" << __LINE__);
      } catch (...) {
        MCE_WARN("PvRequest::response --> adapter unknown err");
      }
    }
  }
  string output = "Content-type:text/html;charset=utf-8\r\n\r\n{ok}";
  FCGX_PutS(output.c_str(),_fcgi_out);
  return result;
}

bool PvRequest::ProcessPvLog(string& pv_log) {
  return true;
}

//click log
bool ClickRequest::response() {
  bool result = false;
  if (!_fcgi_out) {
    MCE_WARN("ClickRequest::response --> fcgi_out is null");
    return result;
  }
  string log = getProperty("log");
  if (log.empty()) {
    MCE_WARN("ClickRequest::response --> click log is empty");
  } else {
    StrSeq log_seq = LogStringParser::instance().LogSpliter(gClick, log);
    if (log_seq.size() == 0) {
      MCE_WARN("ClickRequest::response --> log spliter result is empty. input is: " << log);
    } else {
      try {
        FeedLogServerReplicaAdapter::instance().PutLogBatch(gClick, log_seq);
        result = true;
      } catch (Ice::Exception& e) {
        MCE_WARN("ClickRequest::response --> adapter err:" << e << ",File:" <<__FILE__ << ",Line:" << __LINE__);
      } catch (...) {
        MCE_WARN("ClickRequest::response --> adapter unknown err");
      }
    }
  }
  string output = "Content-type:text/html;charset=utf-8\r\n\r\n{ok}";
  FCGX_PutS(output.c_str(),_fcgi_out);
  return result;
}

bool ClickRequest::ProcessClickLog(string& click_log) {
  return true;
}

//reply log
bool ReplyRequest::response() {
  bool result = false;
  if (!_fcgi_out) {
    MCE_WARN("ReplyRequest::response --> fcgi_out is null");
    return result;
  }
  string log = getProperty("log");
  if (log.empty()) {
    MCE_WARN("ReplyRequest::response --> reply log is empty");
  } else {
    StrSeq log_seq = LogStringParser::instance().LogSpliter(gReply, log);
    if (log_seq.size() == 0) {
      MCE_WARN("ReplyRequest::response --> log spliter result is empty. input is: " << log);
    } else {
      try {
        FeedLogServerReplicaAdapter::instance().PutLogBatch(gReply, log_seq);
        result = true;
      } catch (Ice::Exception& e) {
        MCE_WARN("ReplyRequest::response --> adapter err:" << e << ",File:" <<__FILE__ << ",Line:" << __LINE__);
      } catch (...) {
        MCE_WARN("ReplyRequest::response --> adapter unknown err");
      }
    }
  }
  string output = "Content-type:text/html;charset=utf-8\r\n\r\n{ok}";
  FCGX_PutS(output.c_str(),_fcgi_out);
  return result;
}

bool ReplyRequest::ProcessReplyLog(string& reply_log) {\
  return true;
}

//share log
bool ShareRequest::response() {
  bool result = false;
  if (!_fcgi_out) {
    MCE_WARN("ShareRequest::response --> fcgi_out is null");
    return result;
  }
  string log = getProperty("log");
  if (log.empty()) {
    MCE_WARN("ShareRequest::response --> share log is empty");
  } else {
    StrSeq log_seq = LogStringParser::instance().LogSpliter(gShare, log);
    if (log_seq.size() == 0) {
      MCE_WARN("ShareRequest::response --> log spliter result is empty. input is: " << log);
    } else {
      try {
        FeedLogServerReplicaAdapter::instance().PutLogBatch(gShare, log_seq);
        result = true;
      } catch (Ice::Exception& e) {
        MCE_WARN("ShareRequest::response --> adapter err:" << e << ",File:" <<__FILE__ << ",Line:" << __LINE__);
      } catch (...) {
        MCE_WARN("ShareRequest::response --> adapter unknown err");
      }
    }
  }
  string output = "Content-type:text/html;charset=utf-8\r\n\r\n{ok}";
  FCGX_PutS(output.c_str(),_fcgi_out);
  return result;
}

bool ShareRequest::ProcessShareLog(string& share_log) {
  return true;
}

//like log
bool LikeRequest::response() {
  bool result = false;
  if (!_fcgi_out) {
    MCE_WARN("LikeRequest::response --> fcgi_out is null");
    return result;
  }
  string log = getProperty("log");
  if (log.empty()) {
    MCE_WARN("LikeRequest::response --> like log is empty");
  } else {
    StrSeq log_seq = LogStringParser::instance().LogSpliter(gLike, log);
    if (log_seq.size() == 0) {
      MCE_WARN("LikeRequest::response --> log spliter result is empty. input is: " << log);
    } else {
      try {
        FeedLogServerReplicaAdapter::instance().PutLogBatch(gLike, log_seq);
        result = true;
      } catch (Ice::Exception& e) {
        MCE_WARN("LikeRequest::response --> adapter err:" << e << ",File:" <<__FILE__ << ",Line:" << __LINE__);
      } catch (...) {
        MCE_WARN("LikeRequest::response --> adapter unknown err");
      }
    }
  }
  string output = "Content-type:text/html;charset=utf-8\r\n\r\n{ok}";
  FCGX_PutS(output.c_str(),_fcgi_out);
  return result;
}

bool LikeRequest::ProcessLikeLog(string& like_log) {
  return true;
}

//read log
bool ReadRequest::response() {
  bool result = false;
  if (!_fcgi_out) {
    MCE_WARN("ReadRequest::response --> fcgi_out is null");
    return result;
  }
  string log = getProperty("log");
  if (log.empty()) {
    MCE_WARN("ReadRequest::response --> read log is empty");
  } else {
    StrSeq log_seq = LogStringParser::instance().LogSpliter(gRead, log);
    if (log_seq.size() == 0) {
      MCE_WARN("ReadRequest::response --> log spliter result is empty. input is: " << log);
    } else {
      try {
        FeedLogServerReplicaAdapter::instance().PutLogBatch(gRead, log_seq);
        result = true;
      } catch (Ice::Exception& e) {
        MCE_WARN("ReadRequest::response --> adapter err:" << e << ",File:" <<__FILE__ << ",Line:" << __LINE__);
      } catch (...) {
        MCE_WARN("ReadRequest::response --> adapter unknown err");
      }
    }
  }
  string output = "Content-type:text/html;charset=utf-8\r\n\r\n{ok}";
  FCGX_PutS(output.c_str(),_fcgi_out);
  return result;
}

bool ReadRequest::ProcessReadLog(string& read_log) {
  return true;
}

//bolck log
bool BlockRequest::response() {
  bool result = false;
  if (!_fcgi_out) {
    MCE_WARN("BlockRequest::response --> fcgi_out is null");
    return result;
  }
  string log = getProperty("log");
  if (log.empty()) {
    MCE_WARN("BlockRequest::response --> block log is empty");
  } else {
    StrSeq log_seq = LogStringParser::instance().LogSpliter(gBlock, log);
    if (log_seq.size() == 0) {
      MCE_WARN("BlockRequest::response --> log spliter result is empty. input is: " << log);
    } else {
      try {
        FeedLogServerReplicaAdapter::instance().PutLogBatch(gBlock, log_seq);
        result = true;
      } catch (Ice::Exception& e) {
        MCE_WARN("BlockRequest::response --> adapter err:" << e << ",File:" <<__FILE__ << ",Line:" << __LINE__);
      } catch (...) {
        MCE_WARN("BlockRequest::response --> adapter unknown err");
      }
    }
  }
  string output = "Content-type:text/html;charset=utf-8\r\n\r\n{ok}";
  FCGX_PutS(output.c_str(),_fcgi_out);
  return result;
}

bool BlockRequest::ProcessBlockLog(string& block_log) {
  return true;
}

//action log
bool ActionRequest::response() {
  bool result = false;
  if (!_fcgi_out) {
    MCE_WARN("ActionRequest::response --> fcgi_out is null");
    return false;
  }
  string log = getProperty("log");
  if (log.empty()) {
    MCE_WARN("ActionRequest::response --> action log is empty");
  } else {
    StrSeq log_seq = LogStringParser::instance().LogSpliter(gAction, log);
    if (log_seq.size() == 0) {
      MCE_WARN("ActionRequest::response --> log spliter result is empty. input is: " << log);
    } else {
      try {
        FeedLogServerReplicaAdapter::instance().PutLogBatch(gAction, log_seq);
        result = true;
      } catch (Ice::Exception& e) {
        MCE_WARN("ActionRequest::response --> adapter err:" << e << ",File:" <<__FILE__ << ",Line:" << __LINE__);
      } catch (...) {
        MCE_WARN("ActionRequest::response --> adapter unknown err");
      }
    }
  }
  string output = "Content-type:text/html;charset=utf-8\r\n\r\n{ok}";
  FCGX_PutS(output.c_str(),_fcgi_out);
  return result;
}

bool ActionRequest::ProcessActionLog(string& action_log) {
  return true;
}

//at log
bool AtRequest::response() {
  bool result = false;
  if (!_fcgi_out) {
    MCE_WARN("AtRequest::response --> fcgi_out is null");
    return result;
  }
  string log = getProperty("log");
  if (log.empty()) {
    MCE_WARN("AtRequest::response --> at log is empty");
  } else {
    StrSeq log_seq = LogStringParser::instance().LogSpliter(gAt, log);
    if (log_seq.size() == 0) {
      MCE_WARN("AtRequest::response --> log spliter result is empty. input is: " << log);
    } else {
      try {
        FeedLogServerReplicaAdapter::instance().PutLogBatch(gAt, log_seq);
        result = true;
      } catch (Ice::Exception& e) {
        MCE_WARN("AtRequest::response --> adapter err:" << e << ",File:" <<__FILE__ << ",Line:" << __LINE__);
      } catch (...) {
        MCE_WARN("AtRequest::response --> adapter unknown err");
      }
    }
  }
  string output = "Content-type:text/html;charset=utf-8\r\n\r\n{ok}";
  FCGX_PutS(output.c_str(),_fcgi_out);
  return result;
}

bool AtRequest::ProcessAtLog(string& at_log) {
  return true;
}

//feed log
const char* FeedLogRequest::log_types_[] = {"pv", "click", "reply", "read", "share", "block", "like","action", "at"};

bool FeedLogRequest::response() {
  bool result = false;
  if (!_fcgi_out) {
    MCE_WARN("FeedLogRequest::response --> fcgi_out is null");
    return result;
  }
  map<string, MyUtil::StrSeq> logs_map;
  stringstream ss;
  for (int i = 0; i < sizeof(log_types_) / sizeof(const char *); ++i) {
    string log_type = log_types_[i];
    string log = getProperty(log_type);
    if (!log.empty()) {
      StrSeq log_seq = LogStringParser::instance().LogSpliter(log_type, log);
      if (log_seq.size() == 0) {
        MCE_WARN("FeedLogRequest::response --> " << log_type << " log spliter result is empty. input is: " << log);
      } else {
        ss << log_type << " ";
        logs_map[log_type] = log_seq;
      }
    }
  }
  if (!logs_map.empty()) {
    try {
      FeedLogServerReplicaAdapter::instance().PutLogsMap(logs_map);
      result = true;
    } catch (Ice::Exception& e) {
      MCE_WARN("FeedLogRequest::response --> put" << ss.str() << "log. invoke log server adapter err:" << e << ",File:" <<__FILE__ << ",Line:" << __LINE__);
    } catch (...) {
      MCE_WARN("FeedLogRequest::response --> put" << ss.str() << "log. invoke log server adapter unknown err");
    }
  }
  string output = "Content-type:text/html;charset=utf-8\r\n\r\n{ok}";
  FCGX_PutS(output.c_str(), _fcgi_out);
  return result;
}

bool FeedLogRequest::ProcessFeedLog(string& feed_log) {
  return true;
}

typedef boost::tokenizer<boost::char_separator<char> > log_tokenizer;

StrSeq LogStringParser::LogSpliter(const string& log_name, const string& log) {
  boost::char_separator<char> sep("|");  
  StrSeq log_seq;
  log_tokenizer tokens(log, sep);
  try {
    for (log_tokenizer::iterator it = tokens.begin(); it != tokens.end(); ++it) {
      log_seq.push_back(*it);
    }
  } catch (...) {
    MCE_WARN("LogStringParser::LogSpliter --> parse log error. log_name: " << log_name << " input log: " << log);
  }
  return log_seq;
}
