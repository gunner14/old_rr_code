#include "../share/TicketShare.h"
#include <iostream>

using namespace std;
using namespace xce::tripod::ticket;

int main() {
  string lTicket = "12345abcde67890abcde12345abcde";
  string sTicket = "1234567890ABCDEhijkl";
  cout << "toLTicket(1234567890ABCDEhijkl) = " << TicketUtil::toLTicket(sTicket) << endl;
  cout << "toSTicket(12345abcde67890abcde12345abcde) = " << TicketUtil::toSTicket(lTicket) << endl;
  cout << TicketUtil::toSTicket(TicketUtil::toLTicket(sTicket)) << endl;
  cout << TicketUtil::toLTicket(TicketUtil::toSTicket(lTicket)) << endl;

  int id = 88888888;
  cout << TicketUtil::_toSid(id) << endl;
  cout << TicketUtil::_toId(TicketUtil::_toSid(id)) << endl;
  
  cout << TicketUtil::_genCheckSum("12345abcde67890abcde12345abc") << endl;
  cout << TicketUtil::_isCheckSumValid("12345abcde67890abcde12345abc35") << endl;

  cout << "toChar" << (unsigned)(unsigned char)TicketUtil::_toChar("35") << endl;
  cout << TicketUtil::_toString(TicketUtil::_toChar("35")) << endl;
  
  cout << TicketUtil::_genRawTicket("35") << endl;
  
  string s = TicketUtil::genTicket(88888888, 12, "renren.com");
  cout << s << endl;
  cout << (unsigned)TicketUtil::getType(TicketUtil::toLTicket(s)) << endl;
  cout << (unsigned)TicketUtil::getId(TicketUtil::toLTicket(s)) << endl;
  cout << TicketUtil::_toSid((unsigned)TicketUtil::getId(TicketUtil::toLTicket(s))) << endl;

  return 0;
}
