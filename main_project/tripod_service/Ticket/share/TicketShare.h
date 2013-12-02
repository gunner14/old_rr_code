#ifndef __XCE_TRIPOD_TICKET_TICKETSHARE_H__
#define __XCE_TRIPOD_TICKET_TICKETSHARE_H__

#include "TripodTicket.h"
#include "TripodData.h"
#include "util/cpp/String.h"
#include <Ice/Ice.h>
#include <QueryRunner.h>
#include <IceUtil/IceUtil.h>
#include <bitset>
#include <string>

namespace xce {
namespace tripod {
namespace ticket {

using namespace std;

//DB
//--------------------------------------------------------------
const string DB_TRIPOD_TICKET = "tripod_ticket";
const string TABLE_P_TICKET = "p_ticket";
const string TABLE_T_TICKET = "t_ticket";

const unsigned LONG_TICKET_LEN = 30;
const unsigned SHORT_TICKET_LEN = 20;
const unsigned RAW_TICKET_LEN = 15;

const string LDICT = "0123456789abcdef";
const string SDICT = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-_";

class TicketUtil {
public:
  static string toLTicket(const string& sTicket);
  static string toSTicket(const string& lTicket);
  static bool isLTicketLegal(const string& lTicket);
  static bool isSTicketLegal(const string& sTicket);
  static string genTicket(const int id, const unsigned char type, const string& info);
  //从票中获取信息
  static int getId(const string& lTicket);
  static unsigned char getType(const string& lTicket);

private:
  //字典表，用于保存票的编码字符
  static string _lDict;
  static string _sDict;

  //辅助表，用于快速判断字符是否存在字典表中
  static bitset<128> _lMask;  
  static bitset<128> _sMask;

  //索引表，用于快速查找合法字符的下标，不合法的字符下标值为0，但实际不会被访问到
  static unsigned char _lIndex[128];
  static unsigned char _sIndex[128]; 

  //用于初始化静态成员
  static bool _initFlag;
  static bool _init();
  
public: //调试使用，TODO 发布后去掉public
  //长短票转换函数，不做字符串的合法性校验
  static string _toLTicket(const string& sTicket);
  static string _toSTicket(const string& lTicket);

  //有info加上时间生成72bit的密钥，用18个16进制字符保存为一个string，info要随机或足够安全，不可被恶意猜测
  static string _genRawTicket(const string& info);

  //ID和SID互转，为提高效率，不做合法性检测
  static string _toSid(const int id);
  static int _toId(const string& sid);

  //char和16进制string的转换，为提高效率，不做合法性检测
  static string _toString(const char c);
  static char _toChar(const string& s);

  //校验计算
  static string _genCheckSum(const string& lTicket);
  static bool _isCheckSumValid(const string& lTicket);

  //字符串合法性判断，不判断checksum
  static bool _isLTicketValid(const string& lTicket);
  static bool _isSTicketValid(const string& sTicket);

};

class BatchTicketHandlerI: public com::xiaonei::xce::ResultHandler {
public:
  BatchTicketHandlerI(com::renren::tripod::DataMap& results);
  virtual bool handle(mysqlpp::Row& row) const;
private: 
  com::renren::tripod::DataMap& _results;
};

class BatchValidTicketHandlerI: public com::xiaonei::xce::ResultHandler {
public:
  BatchValidTicketHandlerI(MyUtil::Str2IntMap& results);
  virtual bool handle(mysqlpp::Row& row) const;
private: 
  MyUtil::Str2IntMap& _results;
};

}  //namespace ticket
}  //namespace tripod
}  //namespace xce
#endif
