#ifndef _DATAPROVIDERBUFFER_H
#define _DATAPROVIDERBUFFER_H

#include <string>
#include <vector>
#include <queue>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include "Singleton.h"

namespace xce {
namespace dataprovider {

const size_t kHotShareXMLBufferSize = 100000;
const size_t kPhotoXMLBufferSize = 100000;
class DataProviderBuffer: public MyUtil::Singleton<DataProviderBuffer> {
  typedef std::map<std::string, std::string>* StringMapPtr; 
  public:
    DataProviderBuffer();
    void AddHotShareXML(std::string hotshare_xml);
    void AddAlbumXML(std::string hotshare_xml, Ice::Long album_id);
    bool AddOnlineVideoProgram(std::string &program_id, std::string &online_video_pragram_xml);
    void AddOnlineVideoProgramDone();
    void AddOnlineVideoCatagory(std::string &catagory_xml);
    bool GetHotShareXML(std::vector<std::string> &hotshare_xml_vector, Ice::Long low_id, Ice::Long high_id,size_t size, Ice::Long &ret_low_id, Ice::Long &ret_high_id);
    bool GetHotShareXML(std::vector<std::string> &hotshare_xml_vector, Ice::Long max_id, size_t size, Ice::Long &ret_low_id, Ice::Long &ret_high_id);
    bool AddPhoto(Ice::Long album_id, std::string photo_xml);
    std::string GetPhoto(Ice::Long album_id);
    std::string GetOnlineVideoCatagory();
    std::string GetOnlineVideoProgram(const std::string &program_id, const size_t program_size);
  private:
    int GetDBIDPos(Ice::Long DBID);
    StringMapPtr online_video_program_xml_mapptr_;
    StringMapPtr online_video_program_xml_mapptr_temp_;
    std::string *online_video_catagory_ptr_;
    std::vector<std::string> hotshare_xml_vector_;
    std::map<Ice::Long, std::string> photo_xml_map_;
    std::queue<Ice::Long> album_id_queue_;
    Ice::Long bottom_id_;
    Ice::Long top_id_;
    IceUtil::RWRecMutex online_video_program_mutex_;
    IceUtil::RWRecMutex online_video_catagory_mutex_;
};
}
}

#endif
