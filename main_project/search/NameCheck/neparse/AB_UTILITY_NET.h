/*******************************************************************************
* File name: AB_UTILITY_NET.h
* Author: zhangyong  Version: 0.2  Date: 13/01/2007
* Description: 
*
*******************************************************************************/

#ifndef _AB_UTILITY_NET_H
#define _AB_UTILITY_NET_H 

#include "AB_UTILITY_COMMON.h"
#include "AB_UTILITY_PACK.h"
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
//#include <service.h>



# define MAX_IPGROUP_NUM      10

typedef struct {
       int   addrtype;
       UINT32  ip[MAX_IPGROUP_NUM];
} IP_GROUP_S;



// wrappers for unix/linux
int ABU_NET_socket(int family, int type, int protocol);
int ABU_NET_connect(int sockFd,const struct sockaddr *pMyAddr,socklen_t addrLen);

// connect in a timeout
int ABU_NET_connectTO(int sockFd,const struct sockaddr *pMyAddr,socklen_t addrLen, int secs);

// tcp connect in a timeout
/**
	@desc to connect (NONBLOCK mode).
	@return  -1: error
	         -2:timeout
		    0:succ
*/
int ABU_NET_TCPConnectTO(char* host, int port, int secs);

// tcp connect 
int ABU_NET_TCPConnect(char* host, int port);
// domain connect
int ABU_NET_TCPDomainConnect(char* path);

// wrapper for linux/unix
int ABU_NET_listen(int sockFd, int backlog);

// for tcp
int ABU_NET_TCPListen(int port, int queue);
int ABU_NET_TCPDomainListen(char * path, int queue);

int ABU_NET_bind(int sockFd,const struct sockaddr *pMyAddr,socklen_t addrLen);
int ABU_NET_accept(int sockFd,struct sockaddr *pSA,socklen_t *paddrLen);

// wrapper of getsockname
int ABU_NET_getSockName(int sockFd,struct sockaddr *pLocalAddr,socklen_t	* pAddrLen);
// wrapper of getpeername
int ABU_NET_getPeerName(int sockFd,struct sockaddr *pPeerAddr,socklen_t	* pAddrLen);


// The MT safe version of getservbyname
int ABU_NET_getServByName_r(const char *name,const char *proto,struct servent *result_buf, char *buf,size_t buflen,struct servent **result);

// The MT safe version of getservbyport
int ABU_NET_getServByPort_r(int port, const char *proto, struct servent *result_buf, char *buf, size_t buflen,struct servent **result);




// write & read
/**
	@desc to write data to a socket (BLOCK mode).
	@return  -1: error
		  >0: the bytes read
*/
int ABU_NET_write(int fd, void *ptr1, size_t nbytes); // write in BLOCK mode
/**
	@desc to write data in secs to a socket (BLOCK mode).
	@return  -1: error
		  >0: the bytes read
*/
int ABU_NET_writeTO(int fd, void *ptr1, size_t nbytes, int secs);

/**
	@desc to write data to a socket (NON-BLOCK mode).
	@return  -1: error
		  >0: the bytes read
*/
int ABU_NET_writeNB(int fd, void *ptr1, size_t nbytes); // write in NON-BLOCK mode

/**
	@desc to write data in secs to a socket (NON-BLOCK mode).
	@return  -1: error
		  >0: the bytes read
*/
int ABU_NET_writeNBTO(int fd, void *ptr1, size_t nbytes, int secs); // write in NON-BLOCK mode with timeout

// wrapper of send
ssize_t ABU_NET_send(int sockfd,void *buf,size_t len,int flags);

/**
	@desc to tell if a sock is writable in seconds (BLOCK mode)
	@return -1: error
		  0: timeout
		fd#: otherwise, writable immediately.
*/
int ABU_NET_sockWriteable(int fd, int seconds);


/**
	@desc to read data from a socket (BLOCK mode).
	@return  -1: error
		  >0: the bytes read
*/
ssize_t ABU_NET_read(int fd, void *ptr1, size_t nbytes); // read in BLOCK mode

/**
	@desc to read data in secs from a socket (BLOCK mode).
	@return  -1: error
		  >0: the bytes read
*/
ssize_t ABU_NET_readTO(int fd, void *ptr1, size_t nbytes, int secs);

/**
    @desc to read data from a socket (NON-BLOCK mode).
    @return 
    -1: error
    -2: timeout
    >0: the bytes read
*/
ssize_t ABU_NET_readNB(int fd, void *ptr1, size_t nbytes); // read in NON-BLOCK mode

/** 
  * @brief 
  *    To read data from a conn until it is closed or buffer is full
  * @param fd 
  * @param ptr1 
  * @param nbytes 
  * @param secs 
  * @return 
         -1: error
         -2: timeout
          0: no data read but conn  is closed
         >0: the bytes read
  */
ssize_t ABU_NET_readNBTO(int fd, void *ptr1, size_t nbytes, int secs);

// wrapper of recv
ssize_t ABU_NET_recv(int sockfd,void *buf,size_t len,int flags); // wrap

/**
	@desc to tell if a sock is writable in seconds (BLOCK mode)
	@return -1: error
		  0: timeout
		fd#: otherwise, writable immediately.
*/
int ABU_NET_sockReadable(int fd, int seconds);

// wrapper of select
int ABU_NET_select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);

/**
	@desc to return the byteorder of the host machine.
	@return  0: LITTLE_ENDIAN
		  1: BIG_ENDIAN
*/
//#define LITTLE_ENDIAN 0
//#define BIG_ENDIAN     1
int ABU_NET_getHostByteOrder();  // BIG_ENDIAN: 1 or LITTLE_ENDIAN: 0

// Wrap TCP options access functions: setsockopt
int ABU_NET_setSockOpt(int sockFd,int level,int optName,const void * pOptVal,socklen_t optlen);
// SO_RCVBUF
int ABU_NET_opSockOpt_RcvBuf(int s,void *optval,int mode /* 0: GET | 1 SET */); 

// SO_SNDBUF
int ABU_NET_opSockOpt_SndBuf(int s,void *optval,char mode /* 0: GET | 1 SET */); 

// SO_REUSEADDR
int ABU_NET_opSockOpt_ReuseAddr(int s,void *optval,char mode /* 0: GET | 1 SET */); 

// SO_KEEPALIVE
int ABU_NET_opSockOpt_KeepAlive(int s,void *optval,char mode /* 0: GET | 1 SET */); 

// SO_SNDTIMEO
int ABU_NET_opSockOpt_SndTimeo(int sockfd, struct timeval *tv,char mode);

// SO_RCVTIMEO
int ABU_NET_opSockOpt_RcvTimeo(int sockfd, struct timeval *tv,char mode);

// fcntl(fd,F_SETFL,flags|O_NONBLOCK)
int ABU_NET_setSock2Nonblock(int fd);
// fcntl(fd,F_SETFL,flags&~O_NONBLOCK)
int ABU_NET_setSock2Block(int fd);

// ioctl(fd, FIONREAD, (char*)&size); 
ssize_t ABU_NET_getRecvQueueSize(int fd);

// IPv4 Addr filling
int ABU_NET_fillIPv4sa(struct sockaddr_in *sin,char *ip,int port);

// close socket
int ABU_NET_close(int sock);
int ABU_NET_shutdown(int sock, int howto);


// resovle domain

/**
	@desc the MT safe version of getostbyname. A wrapper of gnu version.
	
*/
int ABU_NET_gethostbyname_r(const char *hostname,struct hostent *result_buf,char *buf,int buflen,struct hostent **result,int *h_err);






/**
	@desc the MT safe version to get one ip addr for a domain name
	@return 
		Char * asc_str:  ip string when mode=0; net addr when mode=1
		
*/
int ABU_NET_gethostipbyname_r(const char *hostname,struct sockaddr_in *sin,int mode /* 0: ascii or 1: net */,char	**asc_str);
/**
	@desc the non-MT safe version. The returned buffer is a static char*.
*/
int ABU_NET_gethostipbyname(const char *hostname,struct sockaddr_in *sin); 

/**
	@desc the MT safe version to get multiple IP addres for a domain name.
*/
int ABU_NET_gethostallipbyname_r(const char *hostname,struct sockaddr_in *sin,int mode,char **asc_str, IP_GROUP_S *sip) ;  // get a group of IP adress for a host


/**
 @name ABU_UTILTITY_NET_inet_ntoa_r
 @desc get ip string from uint32 ip
 @param buf the dot string address, out param
 @param nBufLen the length of buf
 @param in struct in_addr, the network address, in param
 @return 0 success, <0 failed
*/
int ABU_UTILITY_NET_inet_ntoa_r(struct in_addr in, char * buf, int nBufLen);



#endif


