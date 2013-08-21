/*
 * cwfsyslog.cc
 *
 *  Created on: 2012-3-8
 *      Author: Administrator
 */

#include "sysloging.h"
#include <syslog.h>
#include <stdarg.h>

namespace logging
{
  void opensyslog(const char *ident)
  {
    ::openlog(ident, LOG_ODELAY, LOG_LOCAL0);
  }
  void syslog(const char *format, ...)
  {
    va_list ap;
    va_start(ap, format);
    vsyslog(LOG_INFO, format, ap);
    va_end(ap);
  }
}

