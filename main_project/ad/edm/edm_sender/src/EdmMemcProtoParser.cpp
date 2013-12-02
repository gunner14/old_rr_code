#include "EdmMemcProtoParser.h"
#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include "IceLogger.h"

const char* EdmMemcProtoParser::END_BUF = "END\r\n";
const int   EdmMemcProtoParser::END_BUF_LEN = 5;
const char* EdmMemcProtoParser::STORED_BUF = "STORED\r\n";
const int   EdmMemcProtoParser::STORED_BUF_LEN = 8;
const char* EdmMemcProtoParser::NOT_STORED_BUF = "NOT_STORED\r\n";
const int   EdmMemcProtoParser::NOT_STORED_BUF_LEN = 12;
const char* EdmMemcProtoParser::DELETED_BUF = "DELETED\r\n";
const int   EdmMemcProtoParser::DELETED_BUF_LEN = 9;
const char* EdmMemcProtoParser::NOT_FOUND_BUF = "NOT_FOUND\r\n";
const int   EdmMemcProtoParser::NOT_FOUND_BUF_LEN = 11;

EdmCmdType EdmMemcProtoParser::GetEdmCmdType(char* buf, int buf_len){
  EdmCmdType cmd_type;
  if(buf[0]=='g' && buf[1]=='e' && buf[2]=='t'){
    cmd_type = EDM_CMD_GET;
  }else if(buf[0]=='s' && buf[1]=='e' && buf[2]=='t'){
    cmd_type = EDM_CMD_SET;
  }else if(buf[0]=='d' && buf[1]=='e' && buf[2]=='l'){
    cmd_type = EDM_CMD_DELETE;
  }else if(buf[0]=='a' && buf[1]=='p' && buf[2]=='p'){
    cmd_type = EDM_CMD_APPEND;
  }else if(buf[0]=='V' && buf[1]=='A' && buf[2]=='L' && buf[3]=='U' && buf[4]=='E'){
    cmd_type = EDM_CMD_VALUE;
  }else if(buf[0]=='E' && buf[1]=='N' && buf[2]=='D'){
    cmd_type = EDM_CMD_END; 
  }else if(buf[0]=='D' && buf[1]=='E' && buf[2]=='L' && buf[3]=='E' && buf[4]=='T'){
    cmd_type = EDM_CMD_DELETED;
  }else if(buf[0]=='N' && buf[1]=='O' && buf[2]=='T' && buf[3]=='_' && buf[4]=='F'){
    cmd_type = EDM_CMD_NOT_FOUND;
  }else if(buf[0]=='S' && buf[1]=='T' && buf[2]=='O' && buf[3]=='R' && buf[4]=='E'){
    cmd_type = EDM_CMD_STORED; 
  }else if(buf[0]=='N' && buf[1]=='O' && buf[2]=='T' && buf[3]=='_' && buf[4]=='S'){
    cmd_type = EDM_CMD_NOT_STORED; 
  }else{ 
    //MCE_WARN("EdmMemcProtoParser::GetEdmCmdType --> undef " << buf_len << " " << buf);
    cmd_type = EDM_CMD_NOTDEF;
  }
  return cmd_type;
}

int EdmMemcProtoParser::CmdKeySize(char* buf, int buf_len){
  char* p = (char *)memchr(buf, ' ', buf_len);
  if(!p){
    return -1;
  }
  return p - buf;
}

EdmMemCmdHead EdmMemcProtoParser::ParserHead(char* buf, int buf_len){
  EdmMemCmdHead cmd_head;
  cmd_head.succ_ = EDMPARSER_ERR;
  cmd_head.body_len_ = 0;
  cmd_head.data_len_ = 0;
  cmd_head.type_ = GetEdmCmdType(buf, buf_len);
  if(cmd_head.type_ == EDM_CMD_NOTDEF){
    cmd_head.succ_ = EDMPARSER_MORE;
    return cmd_head;
  }
  char* p = (char *)memchr(buf, '\r', buf_len);
  if(!p){
    cmd_head.succ_ = EDMPARSER_MORE;
    return cmd_head;
  }
  cmd_head.succ_ = EDMPARSER_SUC;
  if(cmd_head.type_==EDM_CMD_SET || cmd_head.type_==EDM_CMD_APPEND || cmd_head.type_==EDM_CMD_VALUE){
    string head(buf, p-buf);
    vector<string> flags;
    boost::split(flags, head, boost::is_any_of(" "), boost::token_compress_on);
    string str_len;
    if(cmd_head.type_==EDM_CMD_SET && flags.size()==5){
      str_len = flags[4];
      cmd_head.key_ = buf + 4;
    }else if(cmd_head.type_==EDM_CMD_VALUE && flags.size()==4){
      str_len = flags[3];
      cmd_head.key_ = buf + 6;
    }else if(cmd_head.type_==EDM_CMD_APPEND && flags.size()==5){
      str_len = flags[4];
      cmd_head.key_ = buf + 7;
    }else{
      cmd_head.succ_ = EDMPARSER_ERR;
      MCE_WARN("EdmMemcProtoParser::ParserHead --> err " << cmd_head.type_ << " " << flags.size());
      return cmd_head;
    }
    cmd_head.body_ = p + 2;
    cmd_head.key_len_ = CmdKeySize(cmd_head.key_, buf_len-(cmd_head.key_-buf));
    cmd_head.body_len_ = 0;
    cmd_head.flag_ = 0;
    try{
      cmd_head.body_len_ = boost::lexical_cast<size_t>(str_len);
      cmd_head.flag_ = boost::lexical_cast<size_t>(flags[2]);
    }catch(...){
      //MCE_WARN("MemcachedClient::ParserHead --> cast err, " << str_len << " " << flags[2]);
    }
    cmd_head.data_len_ = p - buf + 2 + cmd_head.body_len_ + 2;
    if(cmd_head.data_len_ > buf_len){
      cmd_head.succ_ = EDMPARSER_MORE;
    }
  }else if(cmd_head.type_==EDM_CMD_GET){
    cmd_head.key_ = buf + 4;
    cmd_head.key_len_ = p - buf -4;  
    cmd_head.data_len_ = p - buf + 2;
  }else if(cmd_head.type_==EDM_CMD_DELETE){
    cmd_head.key_ = buf + 7;
    cmd_head.key_len_ = p - buf - 7;  
    cmd_head.data_len_ = p - buf + 2;
  }else if(cmd_head.type_==EDM_CMD_END){
    if(buf_len != END_BUF_LEN){
      cmd_head.succ_ = EDMPARSER_MORE;
    }
  }else if(cmd_head.type_==EDM_CMD_STORED){
    if(buf_len != STORED_BUF_LEN){
      cmd_head.succ_ = EDMPARSER_MORE;
    }
  }else if(cmd_head.type_==EDM_CMD_NOT_STORED){
    if(buf_len != NOT_STORED_BUF_LEN){
      cmd_head.succ_ = EDMPARSER_MORE;
    }
  }else if(cmd_head.type_==EDM_CMD_DELETED){
    if(buf_len != DELETED_BUF_LEN){
      cmd_head.succ_ = EDMPARSER_MORE;
    }
  }else if(cmd_head.type_==EDM_CMD_NOT_FOUND){
    if(buf_len != NOT_FOUND_BUF_LEN){
      cmd_head.succ_ = EDMPARSER_MORE;
    }
  }else{
    cmd_head.succ_ = EDMPARSER_ERR;
  }
  return cmd_head;
}
