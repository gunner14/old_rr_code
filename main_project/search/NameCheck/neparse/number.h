/**
 *@file number.h
 *@author bfzhang
 *@brief provied the number transformation api
 * create the file in 2007-08-27
 */
#ifndef _NUMBER_H
#define _NUMBER_H

extern const char GB_NUMBER_COUNT;
extern const char *GB_NUMBER[];

/**
 *@brief Chinese number ->arabic numeral 
 *@zhStr Chinese number
 *       Chinese number must just includes
 *       ('��һ�����������߰˾�ʮ��ǧ����)
 *@retval success >= 0;fail -1
 */
long long cnum2num(const char *zhStr);

#endif
