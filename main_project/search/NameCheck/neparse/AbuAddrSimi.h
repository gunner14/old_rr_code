#ifndef __AbuAddrSimi_H__
#define __AbuAddrSimi_H__

#include <map>
#include <string>
#include <vector>
#include <list>

#include "AbuResource.hpp"
#include "AbuAddrPattern.hpp"

using namespace std;

// Brief Related.
#define SAME_BRIEF_LEN_THRESHOLD 5
#define SAME_BRIEF_SIMI_THRESHOLD 80
#define SIMI_ADD_PER_WORD 4
#define SIMI_MINUS_PER_FIELD 25

// If Simi < SIMI_THRESHOLD:
// Calc Again with Edit Distance Only.
#define SIMI_THRESHOLD 25
// Define the Weights of Each Fields
#define TOWN_WEIGHT 0.05f
#define VILLAGE_WEIGHT 0.05f
#define STREET_WEIGHT 0.25f
#define SUBDISTRICT_WEIGHT 0.25f
#define BUILDING_WEIGHT 0.25f
#define HOTSPOT_WEIGHT 0.1f
#define LEFT_WEIGHT 0.05f

// Define Damping Factor of Each Fields
// We will shift 1-damping_factor Weight if need.
// The More Weight It Shifts to other Fields,
// The Less Important Role It Plays.
// Thus, The More Damping Factor is, The More Important It is.

// If Both Don't Have, Shift More Weight.
#define NEITHER_TOWN_DAMPING_FACTOR 0.1f
#define NEITHER_VILLAGE_DAMPING_FACTOR 0.1f
#define NEITHER_STREET_DAMPING_FACTOR 0.1f
#define NEITHER_SUBDISTRICT_DAMPING_FACTOR 0.1f
#define NEITHER_BUILDING_DAMPING_FACTOR 0.1f
#define NEITHER_HOTSPOT_DAMPING_FACTOR 0.1f
#define NEITHER_LEFT_DAMPING_FACTOR 0.1f

// If Just One Have, Shift Less Weight.
#define EITHER_TOWN_DAMPING_FACTOR 0.3f
#define EITHER_VILLAGE_DAMPING_FACTOR 0.3f
#define EITHER_STREET_DAMPING_FACTOR 0.3f
#define EITHER_SUBDISTRICT_DAMPING_FACTOR 0.3f
#define EITHER_BUILDING_DAMPING_FACTOR 0.3f
#define EITHER_HOTSPOT_DAMPING_FACTOR 0.3f
#define EITHER_LEFT_DAMPING_FACTOR 0.3f

// If Both Have, Shift None Weight.
#define BOTH_TOWN_DAMPING_FACTOR 1.0f
#define BOTH_VILLAGE_DAMPING_FACTOR 1.0f
#define BOTH_STREET_DAMPING_FACTOR 1.0f
#define BOTH_SUBDISTRICT_DAMPING_FACTOR 1.0f
#define BOTH_BUILDING_DAMPING_FACTOR 1.0f
#define BOTH_HOTSPOT_DAMPING_FACTOR 1.0f
#define BOTH_LEFT_DAMPING_FACTOR 1.0f

typedef struct _street_info{
    int start;
    int len;
    const char *street_code;
    int street_code_len;
}STREET_INFO;

class AbuAddrSimi
{
    /**
     * Data Declaration Section
     *
     */
    public:
        typedef CalcResult (AbuAddrSimi:: * _SIMI_CALC_FUNC_) (
                const ABU_PARSE_RESULT& parse_result_i, const ABU_PARSE_RESULT& parse_result_j);
        typedef pair<float, _SIMI_CALC_FUNC_> WeightMethodPair;
        typedef map<string, WeightMethodPair> StringPairMap;
        
    private:
        AbuAddrPattern *p_addr_pattern;
        StringIntMap *p_field_map;
        StringPairMap _field_method_map;
        list<string> _calc_fields;

    public:
        AbuAddrSimi(const char *addr_pattern_config_fileurl);
        ~AbuAddrSimi();

        int CalcSimi(
                const ABU_PARSE_RESULT& parse_result_i, const ABU_PARSE_RESULT& parse_result_j);
        int IsColl(
                const ABU_PARSE_RESULT& parse_result_i, const ABU_PARSE_RESULT& parse_result_j);
        void DumpSelfContent();

    private:
        /*
         * The following methods is used to calc each fields simi
         * */
        CalcResult _CalcTownSimi(
                const ABU_PARSE_RESULT& parse_result_i, const ABU_PARSE_RESULT& parse_result_j);
        CalcResult _CalcVillageSimi(
                const ABU_PARSE_RESULT& parse_result_i, const ABU_PARSE_RESULT& parse_result_j);
        CalcResult _CalcHotspotSimi(
                const ABU_PARSE_RESULT& parse_result_i, const ABU_PARSE_RESULT& parse_result_j);
        CalcResult _CalcStreetSimi(
                const ABU_PARSE_RESULT& parse_result_i, const ABU_PARSE_RESULT& parse_result_j);
        CalcResult _CalcSubdistrictSimi(
                const ABU_PARSE_RESULT& parse_result_i, const ABU_PARSE_RESULT& parse_result_j);
        CalcResult _CalcBuildingSimi(
                const ABU_PARSE_RESULT& parse_result_i, const ABU_PARSE_RESULT& parse_result_j);
        CalcResult _CalcLeftSimi(
                const ABU_PARSE_RESULT& parse_result_i, const ABU_PARSE_RESULT& parse_result_j);
        int _CalcBriefSimi(
                const ABU_PARSE_RESULT& parse_result_i, const ABU_PARSE_RESULT& parse_result_j);
        int _CalcSameAddrSimi(
                const ABU_PARSE_RESULT& parse_result_i, const ABU_PARSE_RESULT& parse_result_j);
};

#endif

