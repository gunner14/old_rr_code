#ifndef _MEMCPROTOEDMPARSER_H_
#define _MEMCPROTOEDMPARSER_H_

#include <string>
using namespace std;
enum EdmCmdType{
  EDM_CMD_GET=1, 
  EDM_CMD_SET=2, 
  EDM_CMD_DELETE=3, 
  EDM_CMD_APPEND=4, 
  EDM_CMD_VALUE=5, 

  EDM_CMD_END=11, 
  EDM_CMD_STORED=12, 
  EDM_CMD_NOT_STORED=13, 
  EDM_CMD_NOT_FOUND=14,
  EDM_CMD_DELETED=15, 
  EDM_CMD_NOTDEF=-1
};

enum EdmParserErrCode{
  EDMPARSER_ERR = -1,
  EDMPARSER_SUC = 0,
  EDMPARSER_MORE = 1
};

struct EdmMemCmdHead{
  EdmParserErrCode succ_;
  EdmCmdType type_;
  int     data_len_;
  char*   body_;
  int     body_len_;
  char*   key_;
  int     key_len_;
  int     flag_;
};
class EdmMemcProtoParser{
public:
  static const char* END_BUF;
  static const int   END_BUF_LEN;
  static const char* STORED_BUF;
  static const int   STORED_BUF_LEN;
  static const char* NOT_STORED_BUF;
  static const int   NOT_STORED_BUF_LEN;
  static const char* DELETED_BUF;
  static const int   DELETED_BUF_LEN;
  static const char* NOT_FOUND_BUF;
  static const int   NOT_FOUND_BUF_LEN;

  static EdmCmdType    GetEdmCmdType(char* buf, int buf_len);
  static int        CmdKeySize(char* buf, int buf_len);
  static EdmMemCmdHead ParserHead(char* buf, int buf_len);
};
#endif
