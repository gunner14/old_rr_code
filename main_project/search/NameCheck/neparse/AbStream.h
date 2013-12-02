#ifndef  __AB_STREAM_H__
#define  __AB_STREAM_H__


#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "AbCommon.h"
#include "AbMsgHeader.h"
#include "AbStd.h"

class AbStream
{
public:
    AbStream()
    {
        m_bConnected = false;
    }
    ~AbStream()
    {
        if(m_bConnected)
            Close();
    }
public:
    int Connect(char const * szAddr, UINT16 unPort, int iWaitmilisec);
    //send iBufLen bytes, -1:failure, 0:success, -2: timeout.
    int SendMsg(char const *pBuf, int iBufLen, int iWaitmilisec);
    //recv iBufLen bytes, -1:failure, 0:success, -2: timeout.
    int RecvMsg(char *pBuf, int iBufLen, int iWaitmilisec);
    //recv header and buff, -1:failure, >=0:success, -2: timeout.
    int RecvMsg(AB_MSG_HEADER_S& header, char *pBuf, int iBufLen, int iWaitmilisec);
    int Close();
    char* GetError()
    {
        return m_szError;
    }
private:
    int m_sockFd;
    bool m_bConnected;
    char m_szIp[16];
    UINT16 m_unPort;
    char m_szError[512];
    char m_szHeadBuf[AB_MSG_HEADER_LEN_ + 1];//the buf for header
};

#endif

