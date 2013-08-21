#include "tickmanager.h"
#include "base3/logging.h"
#include "base3/ptime.h"
#include "util.h"
#include "arch_diff/site_xiaonei.h"
#include "upload/url_parse.h"
#include <boost/algorithm/string/predicate.hpp>

namespace upload {
TicketManager g_tickmanager;

bool TicketManager::Init(const std::string &conf) {
  return tickcache_.Init(conf);
}

bool TicketManager::IsSafeHttpReferer(const std::string& referer) {
  std::string host;
  int port;
  std::string fullpath;
  if (referer.empty())
    return true;
  bool b = arch::ParseUrlSimple(referer, &host, &port, &fullpath);
  if (b && boost::algorithm::ends_with(host, "renren.com")) {
    return true;
  } else {
    return false;
  }
}

#if CHECK_INTERNAL
bool TicketManager::Valid(TicketType tickettype,
    const std::string &pagetype,
    const std::string &ticketsuffix,
    const std::string &hostid,
    const std::string &t,
    const std::string &tick,
    const std::string &real_ip,
    const std::string &x_forward_for) {
#else
bool TicketManager::Valid(TicketType tickettype, const std::string &pagetype,
                          const std::string &ticketsuffix,
                          const std::string &hostid, const std::string &t,
                          const std::string &tick,
                          const std::string& http_referer) {
#endif
  PTIME(pt1, "verityticket", true, 5);

  int uid = atoi(hostid.c_str());
  if (tickettype == kTicketCookie) {

    if (t.empty()) {
      return false;
    }
    if (!IsSafeHttpReferer(http_referer))
      return false;
    std::string memkey = "T_TICKET_" + pagetype + "_" + hostid;
    std::string memvalue = tickcache_.Get(memkey);
    if (!memvalue.empty() && memvalue == t)
      return true;
    std::string s = xce::Query(uid);
    tickcache_.Set(memkey, s);
    return t == s;
    //return xce::CheckTicket(t, uid);
  } else if (tickettype == kTicketTMorph) {
    std::string memkey = "TM_TICKET_" + pagetype + "_" + hostid;
    std::string memvalue = tickcache_.Get(memkey);
    if (!memvalue.empty() && memvalue == tick)
      return true;
    std::string orgt = xce::Query(uid);
    std::string s = StrMd5(
        std::string("pagetype=") + pagetype + "&fix=" + ticketsuffix
            + "&hostid=" + hostid + "&t=" + orgt);
    tickcache_.Set(memkey, s);
    //LOG(INFO) << "ticket: " << s << " md5: "  << StrMd5(s) << " tick: " << tick << " uid: " << uid;
    return tick == s;
  } else if (tickettype == kTicketUrlMorph) {
    std::string s = std::string("pagetype=") + pagetype + "&fix=" + ticketsuffix
        + "&hostid=" + hostid;
    return tick == StrMd5(s);
  } else if (tickettype == kTicketFix) {
#if CHECK_INTERNAL
    if (IsInternalIP(real_ip,x_forward_for) && ticketsuffix == tick) {
      return true;
    } else {
      return false;
    }
#else
    return ticketsuffix == tick;
#endif
  } else if (tickettype == kTicketNone) {
    return true;
  } else if (tickettype == kTickTrans) {
    int ticket_id = xce::GetTicketId(t);
    int trans_id = xce::GetTransId(ticket_id);
    return trans_id > 0;
  }
  return false;
}

#if CHECK_INTERNAL
bool TicketManager::IsInternalIP(const std::string &real_ip, const std::string &x_forward_for) {
  if (real_ip.empty())
  return false;
  if (x_forward_for.empty())
  return false;
  size_t pos = x_forward_for.find_first_of(' ');
  if (pos == string::npos)
  return false;
  if (x_forward_for[0] != '1' || x_forward_for[1] != '0' || x_forward_for[2] != '.')
  return false;
  return true;
}
#endif

}
