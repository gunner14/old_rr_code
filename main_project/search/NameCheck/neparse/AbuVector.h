#ifndef __AbuVector_H__
#define __AbuVector_H__

#include <vector>
#include <iostream>
#include <algorithm>
//#include <assert>
using namespace std;
using namespace __gnu_cxx;

#ifndef UINT32
typedef unsigned int UINT32;
#endif

//set the obj del function
typedef void (*_OBJ_DEL_FUNC_PTR_)(void *p_obj);

template <typename T>
class AbuVector
{
    public:
        AbuVector(_OBJ_DEL_FUNC_PTR_ _p_del_func); 
        ~AbuVector(); //free the container.
        UINT32 appendItem(T *_p_item); //add just one result per call
        T const* getItem(const UINT32 _index);
        int releaseItem(const UINT32 _index, bool need_release=true);
        void resetContainer();
        size_t getAvailableNum() {
            return available_elements.size();}
        size_t getResultNum() {
            return parse_results.size()-available_elements.size();}
        size_t getSize() {return parse_results.size();}
    private:
        vector <T*> parse_results;
        vector <UINT32> available_elements;//store the available array element.
        _OBJ_DEL_FUNC_PTR_ p_del_func;
};

template <typename T>
AbuVector<T>::AbuVector(_OBJ_DEL_FUNC_PTR_ _p_del_func) {
    p_del_func = _p_del_func;
    parse_results.reserve(500000);
}

template <typename T>
T const* AbuVector<T>::getItem(UINT32 _index) {
   // assert(_index < parse_results.size());
    return parse_results[_index];
}

template <typename T>
UINT32 AbuVector<T>::appendItem(T *_p_item) {
    /* 
     * ===  FUNCTION  ======================================================================
     *         Name:  addParseResult
     *  Description:  add parse result to vector
     *  return the index.
     * =====================================================================================
     */
    UINT32 available_index;   
    //new parse result
    //check if there is an available_element
    //if so, copy the obj to parse_results[available_element]
    //else, push it to the parse_results.
    //cerr << "add new result: " << _rid << endl;
    if(available_elements.size() > 0) {
        available_index = available_elements.back();
        available_elements.pop_back();
        parse_results[available_index] = _p_item;
    }
    else {
        available_index = parse_results.size();
        parse_results.push_back(_p_item);
    }
    return available_index;
}

template <typename T>
int AbuVector<T>::releaseItem(const UINT32 _index, bool need_release) {
    /* 
     * ===  FUNCTION  ======================================================================
     *         Name:  releaseParseResults
     *  Description:  
     *  return value description.
     * =====================================================================================
     */
    //assert(_index < parse_results.size());
    if(need_release) {
        (*p_del_func)(parse_results[_index]);
        parse_results[_index] = NULL;
    }
    available_elements.push_back(_index);
    return 0; //clean successfully..
}

template <typename T>
void AbuVector<T>:: resetContainer() {
    for(typename vector<T*>::iterator iter = parse_results.begin();
            iter != parse_results.end(); iter++) {
        if(*iter)
            (*p_del_func)(*iter);
    }
    parse_results.clear();
    available_elements.clear();
}

template <typename T>
AbuVector<T>:: ~AbuVector() {
    resetContainer();
}
#endif //__AbuVector_H__
