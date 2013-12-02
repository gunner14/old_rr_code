/***************************************************************************
 * 
 * Copyright (c) 2012 RenRen.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file netclient.cpp
 * @author lei.yang3(lei.yang3@renren-inc.com)
 * @date 2012/08/13 09:24:12
 * @version $Revision$ 
 * @brief 
 *  
 **/

#include <iostream>
#include <string>
#include <string.h>
#include <assert.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <errno.h>
#include "global.h"
#include "netclient.h"

using namespace std;

client::client()
{
	bzero((char *)&_server_addr, sizeof(_server_addr));
	_server_addr.sin_family = AF_INET;

	_socketfd = 0;
}

client::~client()
{
	if(_socketfd)
		close(_socketfd);

}

bool client::init(struct sockaddr_in *server)
{
	try
	{
		if(NULL == server)
		{
			throw "client::init client init the server failed.";
		}

		_server_addr.sin_port = server->sin_port;
		_server_addr.sin_addr = server->sin_addr;
	}
	catch(const char *err)
	{
		cout << err << endl; 
		return false;
	}

	return true;
}

int client::getsocket()
{
	return _socketfd;
}

bool client::setserver(struct sockaddr_in *server)
{
	try
	{
		if( NULL == server )
		{
			throw "client::setserver client set server failed.";
		}
		
		_server_addr.sin_family = server->sin_family;
		_server_addr.sin_port = server->sin_port;
		_server_addr.sin_addr = server->sin_addr;
	}
	catch(const char *err)
	{
		cout << err << endl;
		return false;
	}

	return true;
}

struct sockaddr_in* client::getserver()
{
	return &_server_addr;
}

bool client::setsockfd(int socketfd)
{
	try
	{
		if( -1 == socketfd )
		{
			throw "client::setsockfd socketfd is error.";
		}

		_socketfd = socketfd;
		return true;
	}
	catch(const char *err)
	{
		cout << err << endl;
		return false;
	}

}

NetClient::NetClient()
{
	_max_retry_time = 0;
	_max_wait_time = 0;
	pthread_rwlock_init( &_client_map_lock, NULL );
}

NetClient::~NetClient()
{
	pthread_rwlock_destroy(&_client_map_lock);
}

client* NetClient::getclient(const string &clientname)
{
	try
	{
		if(clientname.empty())
		{
			throw "NetClient::getclient client name is NULL.";
		}
		map<string, client*>::iterator iter = _client_map.find(clientname);
		if(iter == _client_map.end())
		{
			throw "NetClient::getclient find clientname failed.";
		}

		return iter->second;
	}
	catch(const char *err)
	{
		cout << err << endl;
		return NULL;
	}
}

bool NetClient::createsocket(const string &clientname)
{
	try
	{
		if(clientname.empty())
		{
			throw "NetClient::createsocket clientname is empty.";
		}

		AutoRWlock createsocketlock(&_client_map_lock);
		map<string, client*>::iterator iter = _client_map.find(clientname);
		if(iter == _client_map.end())
		{
			throw "NetClient::createsocket clinetname is not in _client_map.";
		}

		int socketfd;
		if((socketfd = socket(AF_INET,SOCK_STREAM, 0)) == -1)
		{
			throw "NetClient::createsocket socketfd create failed.";
		}
		iter->second->setsockfd(socketfd);

		return true;
	}
	catch(const char *err)
	{
		cout << err << endl;
		return false;
	}
}

bool NetClient::senddata(const string &clientname, const char *msgdata)
{
	try
	{
		if(clientname.empty())
		{
			throw "NetClient::senddata clientname is NULL.";
		}
		if(NULL == msgdata)
		{
			throw "NetClient::senddata msgdata is NULL.";
		}

		AutoRWlock senddatalock(&_client_map_lock);
		map<string, client*>::iterator iter = _client_map.find(clientname); 
		if(iter == _client_map.end())
		{
			throw "NetClinet::senddate clientname is not in _client_map.";
		}

		size_t needlen = strlen(msgdata);
		size_t totallen = 0;
		while(totallen < needlen)
		{
			size_t sendlen = send(iter->second->getsocket(), msgdata, needlen, 0);
			if(sendlen == -1)
			{
				throw "NetClient::senddata send error.";
			}
			totallen += sendlen;
		}

		return true;

	}
	catch(const char *err)
	{
		cout << err << endl;
		return false;
	}
}

#define MAX_EXTEND_BUFFER_SIZE 1024 * 10 * 10

char* NetClient::recvdata(const string &clientname)
{
	try
	{
		if(clientname.empty())
		{
			throw "NetClient::recvdata clientname is NULL.";
		}

		AutoRWlock recvdata_lock(&_client_map_lock);
		map<string, client*>::iterator iter = _client_map.find(clientname);
		if(iter == _client_map.end())
		{
			throw "NetClient::recvdata clientnane is not in _client_map.";
		}

		char tempbuffer[MAX_EXTEND_BUFFER_SIZE];
		char *buffer = extend_buffer(MAX_EXTEND_BUFFER_SIZE);
		int socketfd = iter->second->getsocket();
		size_t recvlen = 0;
		while((recvlen = recv(socketfd, tempbuffer, MAX_EXTEND_BUFFER_SIZE, 0)) != 0)
		{
			strncat(buffer, tempbuffer, recvlen);
		}

		return buffer;
	}
	catch(const char *err)
	{
		cout << err << endl;
		return NULL;
	}
}

bool NetClient::addclient(const string &clientname)
{
	try
	{
		if(clientname.empty())
		{
			throw "NetClient::addclient clientname is NULL.";
		}

		AutoRWlock addclient_lock(&_client_map_lock);
		pair<map<string,client*>::iterator,bool> pr = _client_map.insert(pair<string, client*>(clientname, new client));
		if(pr.second)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	catch(const char *err)
	{
		cout << err << endl;
		return false;
	}
}

bool NetClient::deleteclient(const string &clientname)
{
	try
	{
		if(clientname.empty())
		{
			throw "NetClient::deleteclient clientname is NULL.";
		}

		AutoRWlock deleteclient_lock(&_client_map_lock);
		map<string, client*>::iterator iter = _client_map.find(clientname);
		if(iter == _client_map.end())
		{
			return false;
		}
		else
		{
			_client_map.erase(iter);
			return true;
		}
	}
	catch(const char *err)
	{
		cout << err << endl;
		return false;
	}
}

void NetClient::setretry(uint32_t retry_time, uint32_t wait_time)
{
	_max_retry_time = retry_time;
	_max_wait_time = wait_time;
}

bool NetClient::connect_retry(const string &clientname)
{
	try
	{
		if(clientname.empty())
		{
			throw "NetClient::connect_retry clientname is NULL.";
		}

		AutoRWlock connect_retry_lock(&_client_map_lock);

		map<string, client*>::iterator iter = _client_map.find(clientname);
		if(iter == _client_map.end())
		{
			throw "NetClient::connect_retry clientname is not in _client_map.";
		}

		int socketfd = iter->second->getsocket();
		struct sockaddr_in *addr;
		map<string, struct sockaddr_in>::iterator _server_iter  = _server_map.find(clientname);
		if(_server_iter == _server_map.end())
		{
			addr = iter->second->getserver();
			if(NULL == addr)
			{
				throw "NetClient::connect_retry clientname is not in _server_map and _server_addr.";
			}
		}
		else
		{
			addr = &(_server_iter->second);
		}

		for(int sec=1; sec <= _max_retry_time; sec <<= 1)
		{
			if(connect(socketfd, (struct sockaddr *)(addr), sizeof(struct sockaddr_in)) == 0)
			{
				return true;
			}

			if(sec <= _max_wait_time / 2)
			{
				sleep(sec);
			}
		}

		return false;
	}
	catch(const char *err)
	{
		cout << err << endl;
		return false;
	}
}

void NetClient::parseweb(const char *server, struct hostent *host)
{
	try
	{
		if(NULL == server)
		{
			throw "NetClient::parseweb server info is NULL.";
		}

		if((host = gethostbyname(server)) == NULL)
		{
			throw "NetClient::parseweb gethostbyname is failed.";
		}

	}
	catch(const char *err)
	{
		cout << err << endl;
	}
}

bool NetClient::setserverinfo(const string &clientname, const char *server, uint32_t port)
{
	try
	{
		if(clientname.empty())
		{
			throw "NetClient::setserverinfo clientname is NULL.";
		}
		if(NULL == server)
		{
			throw "NetClient::setserverinfo server is NULL.";
		}

		struct hostent *host;
		if((host = gethostbyname(server)) == NULL)
		{
			throw "NetClient::setserverinfo gethostbyname failed.";
		}
		struct sockaddr_in serverinfo;
		serverinfo.sin_family  = AF_INET;
		serverinfo.sin_port = htons(port);
		serverinfo.sin_addr = *((struct in_addr*)host->h_addr);

		pair<map<string, struct sockaddr_in>::iterator, bool> pr = _server_map.insert(pair<string, struct sockaddr_in>(clientname, serverinfo));
		if(pr.second)
		{
			return true;
		}
		else
		{
			throw "NetClient::setserverinfo insert _server_map failed.";
		}
	}
	catch(const char *err)
	{
		cout << err << endl;
		return false;
	}
}

bool NetClient::setserverinfo(const char *server, uint32_t port)
{
	try
	{
		if(NULL == server)
		{
			throw "NetClient::setserverinfo server is NULL.";
		}

		struct hostent *host;
		if((host = gethostbyname(server)) == NULL)
		{
			throw "NetClient::setserverinfo gethostbyname failed.";
		}
		struct sockaddr_in serverinfo;
		serverinfo.sin_family  = AF_INET;
		serverinfo.sin_port = htons(port);
		serverinfo.sin_addr = *((struct in_addr*)host->h_addr);

		AutoRWlock setserver_lock(&_client_map_lock);
		for(map<string, client*>::iterator iter = _client_map.begin(); iter != _client_map.end(); ++iter)
		{
			pair<map<string, struct sockaddr_in>::iterator, bool> pr = _server_map.insert(pair<string, struct sockaddr_in>(iter->first, serverinfo));
			assert(pr.second);
		}

		return true;
	}
	catch(const char *err)
	{
		cout << err << endl;
		return false;
	}
}

char *NetClient::extend_buffer(size_t extend_size)
{
	char *newptr = new char[extend_size];
	return newptr;
}










/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
