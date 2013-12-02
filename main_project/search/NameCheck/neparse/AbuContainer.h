#ifndef __AbuContainer_H__
#define __AbuContainer_H__

#include <ext/hash_map>
#include <vector>
#include <iostream>
#include <algorithm>

extern "C" {
#include "AB_UTILITY_HASH.h"
}

using namespace std;
using namespace __gnu_cxx;

#ifndef UINT32
typedef unsigned int UINT32;
#endif

#ifndef UINT64
typedef unsigned long long UINT64;
#endif

#ifndef SIGN_BUF_SIZE
#define SIGN_BUF_SIZE 1024
#endif

//set the obj->string converting function
typedef void (*_OBJ_SIGN_FUNC_PTR_)(const void *p_obj, char *p_sign_buf, size_t *p_buf_size);
//set the obj del function
typedef void (*_OBJ_DEL_FUNC_PTR_)(void *p_obj);

//ok, since stl doesn's support uint64 as hash_map key,
//we need provide hash/equal functions
typedef struct _unit64_hash {
    size_t operator () (const UINT64& _uint64) const {
        return _uint64 & 0xffffffff; //use lower-32 bit.
    }
}uint64_hash;

typedef struct _uint64_compare {
    bool operator() (const UINT64& _uint64_i, const UINT64& _uint64_j) const {
        return _uint64_i == _uint64_j;
    }
}uint64_compare;

//define the following hash_map
typedef pair <UINT64, UINT32> LongPair;
typedef pair <UINT32, UINT32> IntPair;
typedef hash_map <UINT32, UINT32> IntIntMap; //index-refcnt
typedef hash_map <UINT64, LongPair, uint64_hash, uint64_compare> LongLongMap; //rid-sign
typedef hash_map <UINT64, IntPair, uint64_hash, uint64_compare> LongIntMap; //rid-index

template <typename T>
class AbuContainer
{
    public:
        AbuContainer(_OBJ_SIGN_FUNC_PTR_ _p_sign_func,
                _OBJ_DEL_FUNC_PTR_ _p_del_func); 
        ~AbuContainer(); //free the container.
        int addParseResult(const UINT64 _rid, T *_parse_result); //add just one result per call
        T const* getParseResult(UINT64 rid);
        int releaseParseResult(const UINT64 _rid, bool need_release=true);
        void resetContainer();
        size_t getAvailableNum() {
            return available_elements.size();}
        size_t getResultNum() {
            return parse_results.size()-available_elements.size();}
        size_t getSize() {return parse_results.size();}
    private:
        vector <T*> parse_results;
        vector <UINT32> available_elements;//store the available array element.
        char sign_buf[SIGN_BUF_SIZE];
        size_t sign_buf_size;
        LongLongMap rid_sign_map;
        LongIntMap sign_index_map;
        _OBJ_SIGN_FUNC_PTR_ p_sign_func;
        _OBJ_DEL_FUNC_PTR_ p_del_func;
};


template <typename T>
AbuContainer<T>::AbuContainer(_OBJ_SIGN_FUNC_PTR_ _p_sign_func, 
        _OBJ_DEL_FUNC_PTR_ _p_del_func) {
    p_sign_func = _p_sign_func;
    p_del_func = _p_del_func;
    parse_results.reserve(500000);
}

template <typename T>
T const* AbuContainer<T>::getParseResult(UINT64 _rid) {
    LongLongMap::iterator rid_iter = rid_sign_map.find(_rid);
    if( rid_iter == rid_sign_map.end())
        return NULL;
    UINT64 sign = rid_iter->second.first;
    LongIntMap::iterator sign_iter = sign_index_map.find(sign);
    if (sign_iter == sign_index_map.end())
        return NULL;
    size_t index = sign_iter->second.first;
    assert(index < parse_results.size());
    return parse_results[index];
}


template <typename T>
int AbuContainer<T>::addParseResult(const UINT64 _rid, T *_p_parse_result) {
    /* 
     * ===  FUNCTION  ======================================================================
     *         Name:  addParseResult
     *  Description:  add parse result to vector
     *  waring: we need to copy the result to our vector.
     *  thus, the upper can reuse the _parse_results array..
     *  return 0, nothing to do for caller, return 1, caller need to free _parse_result.
     *  return 2, rid is duplicate.. a warning??
     * =====================================================================================
     */
    int ret = 0;
    UINT32 available_index;   
    UINT64 sign;
    LongLongMap::iterator rid_iter;
    LongIntMap::iterator sign_iter;
    //try to get first..
    rid_iter = rid_sign_map.find(_rid);
    if (rid_iter != rid_sign_map.end()) {
        rid_iter->second.second++; //increase (rid,sign) map refcnt.
        sign = rid_iter->second.first;
        sign_iter = sign_index_map.find(sign);
        sign_iter->second.second++; //increase (sign,index) map refcnt.
        //cerr << "encounter duplicate rid: " << _rid << "-> " << sign \
        //    << ", rid refcnt: " << rid_iter->second.second \
        //    << ", sign refcnt: " << sign_iter->second.second << endl;
        return 2;
    }
    //check if the parse result is unique..
    sign_buf_size = SIGN_BUF_SIZE;
    memset(sign_buf, 0, sign_buf_size);
    if(_p_parse_result)
        (*p_sign_func)((void *)(_p_parse_result), sign_buf, &sign_buf_size);
    //cerr << string(sign_buf) << _rid << sign_buf_size << endl;
    ret = ABU_HASH_create64bSign(const_cast<char *>(sign_buf), sign_buf_size, 
            (UINT32 *)((char *)(&sign)+4), (UINT32 *)(&sign));
    if(ret != 0)
        return -1;
    sign_iter = sign_index_map.find(sign);
    if (sign_iter != sign_index_map.end()) {
        //parse result is duplicate..
        //just insert (rid, sign) to rid_sign_map,
        rid_sign_map[_rid] = make_pair(sign, 1); //add (rid,sign) map
        sign_iter->second.second++;
        //cerr << "encounter duplicate sign: " << sign \
        //    << "refcnt: " << sign_iter->second.second << endl;
        return 1; //tell caller to free p_parse_results
    }
    //new parse result
    //check if there is an available_element
    //if so, copy the obj to parse_results[available_element]
    //else, push it to the parse_results.
    //cerr << "add new result: " << _rid << endl;
    if(available_elements.size() > 0) {
        available_index = available_elements.back();
        available_elements.pop_back();
        parse_results[available_index] = _p_parse_result;
    }
    else {
        available_index = parse_results.size();
        parse_results.push_back(_p_parse_result);
    }
    rid_sign_map[_rid] = make_pair(sign, 1);
    sign_index_map[sign] = make_pair(available_index, 1);
    return 0;
}

template <typename T>
int AbuContainer<T>::releaseParseResult(const UINT64 _rid, bool need_release) {
    /* 
     * ===  FUNCTION  ======================================================================
     *         Name:  releaseParseResults
     *  Description:  
     *  return value description.
     *  -1 : rid doesn't exist
     *  -2 : sign doesn't exist
     *  >=0: the reference count..
     * =====================================================================================
     */
    LongLongMap::iterator rid_iter; 
    LongIntMap::iterator sign_iter;
    UINT64 sign;
    UINT32 index, rid_refcnt, sign_refcnt;
    //cout << "try to release " << _rid << endl;
    rid_iter = rid_sign_map.find(_rid);
    if(rid_iter == rid_sign_map.end()) {
        return -1; //rid doesn't exist..
    }
    rid_iter->second.second--;
    rid_refcnt = rid_iter->second.second;
    sign = rid_iter->second.first;
    if(rid_refcnt == 0) {
        //cerr << "del _rid: " << _rid << endl;
        rid_sign_map.erase(_rid);
    }
    sign_iter = sign_index_map.find(sign);
    if(sign_iter == sign_index_map.end()) {
        return -2; //sign doesn't exist..
    }
    sign_iter->second.second--;
    sign_refcnt = sign_iter->second.second;
    index = sign_iter->second.first;
    assert(index < parse_results.size());
    if(sign_refcnt > 0) {
        //cerr << "return " << sign_refcnt << endl;
        return (int)sign_refcnt;
    }
    //cerr << "del sign" << sign << endl;
    //call free function to free the memory.
    if(need_release) {
        (*p_del_func)(parse_results[index]);
        parse_results[index] = NULL;
    }
    //remove the rid_sign_map element
    //update available_index, sign_index_map.
    sign_index_map.erase(sign);
    available_elements.push_back(index);
    return 0; //clean successfully..
}

template <typename T>
void AbuContainer<T>:: resetContainer() {
    for(typename vector<T*>::iterator iter = parse_results.begin();
            iter != parse_results.end(); iter++) {
        if(*iter)
            (*p_del_func)(*iter);
    }
    parse_results.clear();
    available_elements.clear();
    rid_sign_map.clear();
    sign_index_map.clear();
}

template <typename T>
AbuContainer<T>:: ~AbuContainer() {
    resetContainer();
}
#endif //__AbuContainer_H__
