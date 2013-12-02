#include "TicketShare.h"
#include "Ticket.pb.h"

using namespace com::renren::tripod;
using namespace xce::tripod::ticket;
using namespace xce::tripoddata::ticket;

//TicketUtil
//--------------------------------------------------------------
string TicketUtil::toLTicket(const string& sTicket) {
  string result;
  if(_isSTicketValid(sTicket)) {
    result = _toLTicket(sTicket);
  }  
  return result;  
}

string TicketUtil::toSTicket(const string& lTicket) {
  string result;
  if(_isLTicketValid(lTicket)) {
    result = _toSTicket(lTicket);
  }
  return result; 
}

bool TicketUtil::isLTicketLegal(const string& lTicket) {
  return (_isLTicketValid(lTicket) && _isCheckSumValid(lTicket));
}

bool TicketUtil::isSTicketLegal(const string& sTicket) {
  bool result = false;
  string lTicket = _toLTicket(sTicket);
  if(lTicket.size() == LONG_TICKET_LEN) {
    result = _isCheckSumValid(lTicket);
  }
  return result;
}

string TicketUtil::genTicket(const int id, const unsigned char type, const string& info) {
  //嵌入id和type
  char buffer[LONG_TICKET_LEN - 1] = {};
  string rawTicket = _genRawTicket(info);
  string sid = _toSid(id);
  string t = _toString(type);
  unsigned i=0, j=0, k=0;
  buffer[i++] = t[0];
  buffer[i++] = rawTicket[j++];
  buffer[i++] = rawTicket[j++];
  for(; k<8; k++, i+=3, j+=2) {
    buffer[i] = sid[k];
    buffer[i+1] = rawTicket[j];
    buffer[i+2] = rawTicket[j+1];
  }
  buffer[i] = t[1];

  //嵌入校验和
  string lTicket = buffer;
  lTicket.append(_genCheckSum(lTicket));

  //压缩成短票
  string sTicket = _toSTicket(lTicket);  

  return sTicket; 
}

string TicketUtil::_toSTicket(const string& lTicket) {
  char raw[RAW_TICKET_LEN] = {}; 
  char buffer[SHORT_TICKET_LEN + 1] = {}; 

  //转成RAW
  for(unsigned i=0, j=0; i < LONG_TICKET_LEN; i+=2, j++) {
    raw[j] |= (char)(_lIndex[(unsigned)lTicket[i]] << 4); 
    raw[j] |= (char)_lIndex[(unsigned)lTicket[i + 1]];
  } 

  unsigned index;
  for(unsigned i=0, j=0; i < SHORT_TICKET_LEN; i+=4, j+=3) {
    //第一个字节的前6位
    index = (raw[j] >> 2) & 0x3F;
    buffer[i] = _sDict[index];
    //第一个字节的后2位加第二个字节的前4位
    index = ((raw[j] << 4 ) & 0x30) | ((raw[j+1] >> 4 ) & 0x0F);
    buffer[i+1] = _sDict[index];
    //第二个字节的后4位加第三个字节的前2位
    index = ((raw[j+1] << 2) & 0x3C) | ((raw[j+2] >> 6) & 0x03);
    buffer[i+2] = _sDict[index];
    //第三个字节的后6位
    index = raw[j+2] & 0x3F;
    buffer[i+3] = _sDict[index];
  }   

  return buffer; 
}

string TicketUtil::_toLTicket(const string& sTicket) {
  char raw[RAW_TICKET_LEN] = {}; 
  char buffer[LONG_TICKET_LEN + 1] = {}; 

  //从sDict字符转成RAW
  //每个sDict字符用6b表示，4个ldict字符转成3个RAW字节
  for(unsigned i=0, j=0; i < SHORT_TICKET_LEN; i+=4, j+=3) {
    char b0 = (char)(_sIndex[(unsigned)sTicket[i]]);
    char b1 = (char)(_sIndex[(unsigned)sTicket[i+1]]);
    char b2 = (char)(_sIndex[(unsigned)sTicket[i+2]]);
    char b3 = (char)(_sIndex[(unsigned)sTicket[i+3]]);

    raw[j] = (b0 << 2) | (b1 >> 4);
    raw[j+1] = (b1 << 4) | (b2 >> 2);
    raw[j+2] = (b2 << 6) | b3;
  }   

  //从RAW转成lDict字符
  //每个lDict字符用4b表示，每个RAW字节转成2个lDict字符
  unsigned index;
  for(unsigned i=0, j=0; i < LONG_TICKET_LEN; i+=2, j++) {
    //前4位
    index = (raw[j] >> 4 & 0x0F);
    buffer[i] = _lDict[index];
    //后4位
    index = (raw[j] & 0x0F);
    buffer[i+1] = _lDict[index];
  }   

  return buffer; 
}

string TicketUtil::_genRawTicket(const string& info) {
  std::ostringstream ss;
  ss << info << time(NULL);
  return MyUtil::StrUtil::md5(ss.str()).substr(7, 18);
}

string TicketUtil::_toSid(const int id) {
  char result[9] = {};
  for(unsigned i = 0; i < 8; i++) {
    result[i] = _lDict[((id >> (4*i)) ^ i) & 0xf];
  }
  return result;  
}

int TicketUtil::_toId(const string& sid) {
  int result = 0;
  for(unsigned i = 0; i < 8; i++) {
    result |= ((_lIndex[(unsigned)sid[i]] ^ i) & 0xf) << (4*i);
  }
  return result;
}

string TicketUtil::_toString(const char c) {
  char result[3] = {};
  result[0] = _lDict[(unsigned)((c >> 4) & 0xf)];
  result[1] = _lDict[(unsigned)(c & 0xf)];
  return result;
}

char TicketUtil::_toChar(const string& s) {
  char result = 0;
  result |= _lIndex[(unsigned)s[0]] << 4;
  result |= _lIndex[(unsigned)s[1]];
  return result;
}

string TicketUtil::_genCheckSum(const string& lTicket) {
  char result[3] = {};
  for(unsigned i=0; i < LONG_TICKET_LEN - 2; i+=2) {
    result[0] ^= _lIndex[(unsigned)lTicket[i]];
    result[1] ^= _lIndex[(unsigned)lTicket[i+1]];
  }
  result[0] = _lDict[result[0]];
  result[1] = _lDict[result[1]];

  return result;
}

bool TicketUtil::_isCheckSumValid(const string& lTicket) {
  string sumGen = _genCheckSum(lTicket);
  string sumTicket = lTicket.substr(LONG_TICKET_LEN - 2);
  return (sumGen == sumTicket);
}

bool TicketUtil::_isLTicketValid(const string& lTicket) {
  bool result = true;
  //判断长度是否有效
  if(lTicket.size() != LONG_TICKET_LEN) {
    result = false;
  } else {
    //判断字符集是否有效
    try {
      for(unsigned i = 0; i < LONG_TICKET_LEN; i++) {
        result = result && _lMask.test((unsigned)lTicket[i]); 
      }   
    } catch (...) { //out_of_range
      result = false;
    }
  }
  return result;
}

bool TicketUtil::_isSTicketValid(const string& sTicket) {
  bool result = true;
  //判断长度是否有效
  if(sTicket.size() != SHORT_TICKET_LEN) {
    result = false;
  } else {
    //判断字符集是否有效
    try {
      for(unsigned i = 0; i < SHORT_TICKET_LEN; i++) {
        result = result && _sMask.test((unsigned)sTicket[i]);
      }   
    } catch (...) { //out_of_range
      result = false;
    } 
  } 
  return result;
}

unsigned char TicketUtil::getType(const string& lTicket) {
  char type[3] = {};
  type[0] = lTicket[0];
  type[1] = lTicket[27];
  return _toChar(type);
}

int TicketUtil::getId(const string& lTicket) {
  char buffer[9] = {};
  for(unsigned i=1; i<=8; i++) {
    buffer[i-1] = lTicket[3*i];
  }
  return _toId(buffer);
}
    
//初始化函数
bool TicketUtil::_init() {
  _lDict = LDICT;
  _sDict = SDICT;

  for(unsigned i = 0; i < _lDict.size(); i++) {
    _lMask.set((unsigned)_lDict[i]);
    _lIndex[(unsigned)_lDict[i]] = i;
  }  

  for(unsigned i = 0; i < _sDict.size(); i++) {
    _sMask.set((unsigned)_sDict[i]);
    _sIndex[(unsigned)_sDict[i]] = i;
  }  

  return true;
}

//初始化工具类的静态成员
string TicketUtil::_lDict;
string TicketUtil::_sDict;
bitset<128> TicketUtil::_lMask;
bitset<128> TicketUtil::_sMask;
unsigned char TicketUtil::_lIndex[128];
unsigned char TicketUtil::_sIndex[128];
bool TicketUtil::_initFlag = TicketUtil::_init(); //用静态变量的初始化启动初始化函数

//BatchTicketHandlerI
//--------------------------------------------------------------
BatchTicketHandlerI::BatchTicketHandlerI(DataMap& results) : _results(results) {
}

bool BatchTicketHandlerI::handle(mysqlpp::Row& row) const {
  long id = (long)row[CTID.c_str()];
  long realId = (long)row[CTREALID.c_str()];
  string ticket = (string)row[CTTICKET.c_str()];
  string ip = (string)row[CTIP.c_str()];
  unsigned type = (unsigned)row[CTTYPE.c_str()];
  unsigned expireTime = (unsigned)row[CTEXPIREDTIME.c_str()];
  unsigned now = time(NULL);
  int expire = expireTime - now;
  if(expire > 0) {
    TicketData data;
    data.set_id(id);
    data.set_real_id(realId);
    data.set_type(type);
    data.set_ip(ip);
    ostringstream os;
    data.SerializeToOstream(&os);
    string value = os.str();
    _results[ticket] = Data(value.begin(), value.end());
  } else {
    //可删除DB过时的票
  }
  return true;
}

//BatchValidTicketHandlerI
//获取有效的票和超时的时间，方便load的时候设置expiredTime参数
//--------------------------------------------------------------
BatchValidTicketHandlerI::BatchValidTicketHandlerI(MyUtil::Str2IntMap& results) : _results(results) {
}

bool BatchValidTicketHandlerI::handle(mysqlpp::Row& row) const {
  string ticket = (string)row[CTTICKET.c_str()];
  unsigned expireTime = (unsigned)row[CTEXPIREDTIME.c_str()];
  unsigned now = time(NULL);
  int expire = expireTime - now;
  if(expire > 0) {
    _results[ticket] = expire;
  } else {
    //可删除DB过时的票
  }
  return true;
}
