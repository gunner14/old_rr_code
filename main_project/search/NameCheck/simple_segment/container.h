#ifndef __N_SEGMENT_CONTANIOR_H__
#define __N_SEGMENT_CONTANIOR_H__

#include <string>
#include <vector>
#include <map>
#include <ext/hash_map>
#include <set>
#include <ext/hash_set>

using namespace std;
using namespace __gnu_cxx;

namespace segment
{
	struct lgstr
        {       
                bool operator()(const char* s1, const char* s2) const
                {       
                        //return  strcmp(s1, s2) > 0;
			return *(unsigned short*)s1 >  *(unsigned short*)s2;
                }       
        };      

        struct eqstr
        {       
                bool operator()(const char* s1, const char* s2) const
                {       
                        //return  strcmp(s1, s2) == 0;
			return *(unsigned short*)s1 ==  *(unsigned short*)s2;
                }       
        };      

        typedef vector<string> StrAry; 
        typedef hash_map<const char*, StrAry, hash<const char*>, eqstr> StrHashMap;
        typedef map<const char*, StrHashMap , lgstr> DoubleHashMap;

        typedef hash_set<const char*, hash<const char*>, eqstr> StrSet;

        typedef map<const char*,float , lgstr> HashFMap;
        typedef map<const char*,int , lgstr> HashIMap;
}

#endif
