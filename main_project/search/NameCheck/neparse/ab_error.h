/*************************************************
* Filename: ab_error.h.
* 
* Date: 2007-06-28
* Description:declaration the error handling functions
*************************************************/
#ifndef _AB_ERROR_H
#define _AB_ERROR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <netdb.h>

/*unix_style error*/
void ab_unix_error(char *szMsg);

/*posix_style error*/
void ab_posix_error(int iCode,char *szMsg);

/*dns_style error*/
void ab_dns_error(char *szMsg);

/*application error*/
void ab_app_error(char *szMsg);
#endif
