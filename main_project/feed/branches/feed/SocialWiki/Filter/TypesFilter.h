
#ifndef __SOCIALWIKI_TYPESFILTER_H__
#define __SOCIALWIKI_TYPESFILTER_H__

#include <set>

namespace xce { namespace feed {

using namespace MyUtil;
using namespace std;

struct WeiredType {
    unsigned type       : 16;
    unsigned version    : 8;
    unsigned reversed0  : 2;
    unsigned real_type  : 2;  
    unsigned mini_merge : 2;
    unsigned news_merge : 2;

    WeiredType(unsigned int v) {
        BOOST_STATIC_ASSERT(sizeof(WeiredType) == sizeof(int));
        type = v&0xffff;
        v >>= 16;
        version = v & 0xff;
        v >>= 8;
        reversed0 = v & 0x3;
        v >>= 2;
        real_type = v & 0x3;
        v >>= 2;
        mini_merge = v & 0x3;
        v >>= 2;
        news_merge = v & 0x3;
    }
};

struct TypesFilter {
    std::set<Ice::Int> types;
    TypesFilter(const MyUtil::IntSeq& _types) : types(_types.begin(), _types.end()) {};

    //return: true types中包含type; false: 反之
    inline bool operator()(const Ice::Int type){
        return types.empty() || 
            types.find(type) != types.end() || 
            types.find(WeiredType(type).type) != types.end() ||
            types.find(int(WeiredType(type).type/100)*100) != types.end();
    }
};

}}  //namespace end

#endif

