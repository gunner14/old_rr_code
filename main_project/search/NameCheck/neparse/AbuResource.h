#ifndef __AbuResource_H__
#define __AbuResource_H__
/**
 *
 */
#include <map>
#include <string>
#include <vector>

using namespace std;

typedef pair<int, float> CalcResult;
typedef map<string, int> StringIntMap;
typedef vector<string> ParsedAddr;

typedef struct _abu_parse_result{
    int u_addr_len;
    unsigned short *u_addr;
    ParsedAddr *p_parsed_addr;
}ABU_PARSE_RESULT;

typedef vector<string> StringVector;
typedef vector<float> FloatVector;
typedef vector<int> IntVector;
typedef pair<int, int> StartLengthPair;

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define DEVIATION 1.0e-6 //for float comparision

// Define SIMI
#define MAX_SIMI 100
#define MIN_SIMI 0
// 
#define MIN_DAMPING_FACTOR 0.0f
#define MAX_DAMPING_FACTOR 1.0f
//
#define FIELD_NUM 16

//extern char *STREET_UNI_FEATS[ ]; 
extern int STREET_FEATS_NUM;

//extern char *BUILDING_UNI_FEATS[ ]; 
extern int BUILDING_FEATS_NUM;

typedef struct _u_feat{
    unsigned short *u_feat;
    int u_feat_len; //in 2 bytes
}U_FEAT;

//singleton class.
//a simple implementation, without thread-safety.
//meanwhile, it's not an ideal base class..
//
class AbuResource
{
    public:
        //resource 1, field name: field index map.
        static AbuResource* Instance()
        {
            if (!pInstance_)
            {
                Create();
                //AbuResource::InitAllFields();
            }
            return pInstance_;
        }
        //get _all_fields.
        StringIntMap* GetFieldMap()
        {
            return &(_field_map);
        }
        StringVector* GetFieldList()
        {
            return &(_field_list);
        }
        U_FEAT* GetStreetFeats()
        {
            return p_u_street_feats;
        }
        U_FEAT* GetBuildingFeats()
        {
            return p_u_building_feats;
        }
        void DumpAllFields();
        void DumpParsedAddr(const ParsedAddr& parsed_addr);
    private:
        //disable the following functions..
        //
        AbuResource();
        AbuResource(const AbuResource&);
        AbuResource& operator=(const AbuResource&);
        ~AbuResource()
        {
            pInstance_ = 0;
        }
        //
        static void Create()
        {
            static AbuResource theInstance;
            pInstance_ = &theInstance;
        }
        void InitFieldMap();
        void InitFieldList();
        U_FEAT* InitUniFeats(char **feats, int feat_num);
        //Logic Related Methods;
        //static void InitAllFields();
        //Data
        static AbuResource *pInstance_;
        StringIntMap _field_map;
        StringVector _field_list;
        U_FEAT *p_u_street_feats;
        U_FEAT *p_u_building_feats;
};

//field compare functor..
class AbuFieldCmp
{
    private:
        StringIntMap *p_field_map;
    public:
        AbuFieldCmp(StringIntMap *p_in_field_map):p_field_map(p_in_field_map){};
        bool operator () (const string& field_i, const string& field_j) const{
            return (p_field_map->find(field_i)->second < p_field_map->find(field_j)->second); 
        }
};
#endif
