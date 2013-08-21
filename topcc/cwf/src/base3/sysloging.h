/*
 * cwfsyslog.h
 *
 *  Created on: 2012-3-8
 *      Author: Administrator
 */

#ifndef CWFSYSLOG_H_
#define CWFSYSLOG_H_
namespace logging
{
  void opensyslog(const char *ident);
  void syslog(const char *format, ...);
}

#endif /* CWFSYSLOG_H_ */
