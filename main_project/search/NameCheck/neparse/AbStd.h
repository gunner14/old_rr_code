#ifndef  __AB_STD_H__
#define  __AB_STD_H__

#include <string>
#include <queue>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <ext/hash_map>
#include <list>
#include <utility>
#include <fstream>
#include <algorithm>
#include <set>
#include <map>
#include <assert.h>
#include <pthread.h>
#include "AbTypeDef.h"


using namespace std;
using namespace __gnu_cxx;

template<typename T> class AbHashPoint{
public:
	size_t operator()(T const *key)const  { return key->hash();}
	bool operator()(T const *item1, T const *item2) const { return *item1 == *item2;}
};

template<typename T> class AbHash{
public:
	size_t operator()(T const& key)const  { return key.hash();}
	bool operator()(T const &item1, T const &item2) const { return item1 == item2;}
};


class AbHashChar{
public:
	size_t operator()(const char* str) const { return __stl_hash_string(str);}
	bool operator()(const char* item1, const char* item2) const { return strcmp(item1,item2)==0;}
};

class AbHashCaseChar{
public:
	size_t operator()(const char* str) const { return __stl_hash_string(str); }
	bool operator()(const char* item1, const char* item2) const{ return strcasecmp(item1,item2)==0;}
};

template <typename T> class AbPointLess{
public:
	bool operator()(T const *item1, T const *item2) const { return *item1<*item2;}
};

class AbCharLess{
public:
	bool operator()(const char* item1, const char* item2) const{ return strcmp(item1,item2)<0;}
};

class AbCharCaseLess{
public:
	bool operator()(const char* item1, const char* item2) const{ return strcasecmp(item1,item2)<0;}
};


class AbHashUint64{
public:
	size_t operator()(UINT64 key)const  { return key&0xFFFFFF;}
	size_t operator()(UINT64 key1, UINT64 key2)const  { return key1==key2;}
};
class AbUint64Less{
public:
	int operator()(UINT64 item1, UINT64 item2) const { return item1<item2;}
};

class AbHashString{
public:
	size_t operator()(const string& str) const { return __stl_hash_string(str.c_str()); }
};

class AbRwLockRead{
public:
	AbRwLockRead (pthread_rwlock_t* pLock):m_pLock(pLock){
		if (m_pLock){
			pthread_rwlock_rdlock(m_pLock);
		}
	}
	~AbRwLockRead (){
		if (m_pLock) pthread_rwlock_unlock(m_pLock);
	}
private:
	pthread_rwlock_t*      m_pLock;
};

class AbRwLockWrite{
public:
    AbRwLockWrite (pthread_rwlock_t* pLock):m_pLock(pLock){
		if (m_pLock){
			pthread_rwlock_wrlock(m_pLock);
		}
	}
	~AbRwLockWrite (){
		if (m_pLock) pthread_rwlock_unlock(m_pLock);
	}
private:
	pthread_rwlock_t*      m_pLock;
};

class AbMutexLock{
public:
	AbMutexLock(pthread_mutex_t* pLock):m_pLock(pLock){
		if (m_pLock){
			pthread_mutex_lock(m_pLock);
		}
	}
	~AbMutexLock(){
		if (m_pLock) pthread_mutex_unlock(m_pLock);
	}
private:
	pthread_mutex_t*      m_pLock;
};

#endif

