#ifndef __AbuAddrPattern_H__
#define __AbuAddrPattern_H__

#include <list>
#include "AbuResource.hpp"

using namespace std;

typedef list<string> FIELDS;
typedef pair<int, FIELDS> LEVEL_FIELDS;
typedef pair<string, LEVEL_FIELDS> NAME_LEVEL_FIELDS;
typedef list<NAME_LEVEL_FIELDS> PATTERN_LIST;

class AbuAddrPattern
{
    private:
        PATTERN_LIST _pattern_list;
        StringIntMap *p_field_map;
    public:
        AbuAddrPattern(const char *config_file_url);
        ~AbuAddrPattern();
        bool GetAddrPattern(ParsedAddr *p_parsed_addr, 
                char **p_pattern_name, 
                int *p_detail_level);
};

#endif
