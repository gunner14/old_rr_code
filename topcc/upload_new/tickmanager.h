#ifndef _UPLOAD_TICKET_MANAGER_HEAD_
#define _UPLOAD_TICKET_MANAGER_HEAD_

#include "memcache.h"
#include "runtimeconfig.h"

#define CHECK_INTERNAL 0
namespace upload {

class TicketManager {
public:
  bool Init(const std::string &conf);
#if CHECK_INTERNAL
  bool Valid(TicketType tickettype,
      const std::string &pagetype,
      const std::string &ticketsuffix,
      const std::string &hostid,
      const std::string &t,
      const std::string &tick,
      const std::string &real_ip,
      const std::string &x_forward_for);
#else
  bool Valid(TicketType tickettype, const std::string &pagetype,
             const std::string &ticketsuffix, const std::string &hostid,
             const std::string &t, const std::string &tick,
             const std::string& http_referer);

#endif
private:
  CMemcache tickcache_;
  bool IsSafeHttpReferer(const std::string& referer);
#if CHECK_INTERNAL
  bool IsInternalIP(const std::string &real_ip,const std::string &x_forward_for);
#endif

};

extern TicketManager g_tickmanager;

}
;

#endif
