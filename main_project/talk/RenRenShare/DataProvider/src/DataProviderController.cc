#include "DataProviderController.h"
#include <sstream>
#include <json/json.h>
#include <openssl/md5.h>
#include <iomanip>
#include "LogWrapper.h"
#include "DataProviderDB.h"
#include "DataProviderBuffer.h"
#include "XMLGenerator.h"
#include "UserUrlAdapter.h"
#include "TimeHandler.h"

using namespace std;
using namespace MyUtil;
using namespace xce::dataprovider;
using namespace xce::adapter::userurl;

const char *kOnlineVideoFilePath = "/data/xce/XNTalk/etc/HotShare/videolist";
const char *kConfigFilePath = "/data/xce/XNTalk/etc/HotShare/config";
const size_t kFetchOnlineVideoFrequency = 1;

bool GetDataFromFile(const string &file_path, const string &division, map<string,string> &info) {
  string line;
  ifstream ifile(file_path.c_str()); 
  if (!ifile) { 
    MCE_WARN("GetDateFromFile   Fail to open file ,path:" << file_path);
    return false;
  }
  while (getline(ifile, line)) {    
    if (line=="")  
      continue;
    int note_pos = line.find("//");
    if (note_pos == string::npos)
      note_pos = line.length();    
    string content = line.substr(0, note_pos);

    MCE_DEBUG("GetDateFromFile   content:"<< content);
    int eq_pos = content.find(division);
    if (eq_pos != string::npos) {
      string key = content.substr(0,eq_pos);
      string value =content.substr(eq_pos+1, content.length()-eq_pos);
      if (key!="" && value!="")
        info[key] = value;
    }
  }
  return true;
}

void DataProviderTimer::handle() {
  MCE_INFO("***********start timer*********");

  DataProviderController::instance().LoadConfig(timer_count_);
  DataProviderController::instance().LoadHotShareInfo(timer_count_);
  if (timer_count_++ == kFetchOnlineVideoFrequency)
    timer_count_ = 0;
}

DataProviderController::DataProviderController() {
  max_db_id_ = -1;
  curl_code_ = curl_global_init(CURL_GLOBAL_ALL);
}

DataProviderController::~DataProviderController() {
  curl_global_cleanup();
}

string DataProviderController::GetOnlineCatagory(const string &post_body) {
  if (!GetDataFromCatagoryRequestBody(post_body))
    return "";
  MCE_DEBUG("DataProviderController::GetOnlineCatagory   post_body:" << post_body << "   xml:"
      << DataProviderBuffer::instance().GetOnlineVideoCatagory()); 
  return DataProviderBuffer::instance().GetOnlineVideoCatagory();
}

string DataProviderController::GetOnlineProgram(const std::string &post_body) {
  string program_id, program_size;
  if (!GetDataFromProgramRequestBody(post_body, program_id, program_size))
    return "";
  MCE_DEBUG("DataProviderController::GetOnlineProgram   post_body:"<<post_body << "   xml:"
      << DataProviderBuffer::instance().GetOnlineVideoProgram(program_id, atoi(program_size.c_str())));
  return DataProviderBuffer::instance().GetOnlineVideoProgram(program_id, atoi(program_size.c_str()));
}

string DataProviderController::GetHotShareData(const string &post_body) {
  vector<Ice::Long> high_id_vector, low_id_vector;
  size_t size, type;
  Ice::Long max_id;

  if (!GetDataFromHotShareRequestBody(post_body, high_id_vector, low_id_vector, size, type, max_id))
    return ""; 
  if (type != 2)
    return "";

  MCE_DEBUG("DataProviderController::GetHotShareData  post_body:"<<post_body<<"    high_id_vector.size:"<<high_id_vector.size()
      <<"   low_id_vector.szie:"<<low_id_vector.size()<<"    size:"<<size<<"   type:"<<type<<"    maxi_id:"<<max_id);
  ostringstream output;          
  int temp_size = (int)size;                    
  vector<string> hotshare_xml_vector;
  vector<Ice::Long> ret_low_id_vector, ret_high_id_vector;

  if (size==0 || size>kRequestLimitSize)
    size = kRequestLimitSize  ;
                                          
  if (high_id_vector.size() == 0) {
    Ice::Long ret_low_id=0, ret_high_id=0;
    if (DataProviderBuffer::instance().GetHotShareXML(hotshare_xml_vector, max_id, size, ret_low_id, ret_high_id)) {
      ret_low_id_vector.push_back(ret_low_id);
      ret_high_id_vector.push_back(ret_high_id);
    }
  } else { 
    size_t count = 0;
    while (temp_size > 0) {
      if (high_id_vector.size() <= count || low_id_vector.size() <= count )
        break;

      Ice::Long ret_low_id=0, ret_high_id=0;
      const size_t kTempXmlVecSize = hotshare_xml_vector.size(); 
      if (DataProviderBuffer::instance().GetHotShareXML(hotshare_xml_vector, low_id_vector[count], high_id_vector[count], temp_size, ret_low_id, ret_high_id)) {     
        if (hotshare_xml_vector.size()-kTempXmlVecSize > 0) {       
          temp_size -= (int)(ret_high_id - ret_low_id +1);

          ret_low_id_vector.push_back(ret_low_id);       
          ret_high_id_vector.push_back(ret_high_id);     

          MCE_DEBUG("DataProviderController::GetHotShareData  Message---->     count:"<<count<<"   lowIDVec:"<<low_id_vector[count]<<"    highID:"<<high_id_vector[count]
              <<"   tempSize:"<<temp_size<<"     retLowID:"<<ret_low_id<<"   retHighID:"<<ret_high_id);
        } else {
          MCE_DEBUG("HotShareController::GetData  Message---->     count:"<<count<<"   lowIDVec:"<<low_id_vector[count]<<"    highID:"<<high_id_vector[count]
              <<"   tempSize:"<<temp_size<<"     retLowID:"<<ret_low_id<<"   retHighID:"<<ret_high_id);
          break;
        }
      } 
      count++;
    }
  }

  MCE_DEBUG("DataProviderController::GetHotShareData  Message---->      ret_low_id_vector:"<<ret_low_id_vector.size()<<"    ret_high_id_vector:"<<ret_high_id_vector.size());
  output << XMLGenerator::OutputXMLHotShare(hotshare_xml_vector, ret_low_id_vector, ret_high_id_vector);
//  MCE_DEBUG("DataProviderController::GetHotShareData  Message---->      output:\n"<<output.str());
  return output.str();
}

string DataProviderController::GetPhotoListData(const std::string &post_body) {
  string album_id, owner_id;
  if (!GetDataFromPhotoListRequestBody(post_body, album_id, owner_id) )
    return false;
  return DataProviderBuffer::instance().GetPhoto(atol(album_id.c_str())); 
}

bool DataProviderController::LoadConfig(const int timer_count) {
  if (timer_count != kFetchOnlineVideoFrequency)
    return true;
  map<string, string> config;
  GetDataFromFile(kConfigFilePath, "=", config);
  IceUtil::RWRecMutex::WLock lock(config_info_mutex_);
  config_info_.reload = config["reload"];
  MCE_DEBUG("DataProviderController::LoadConfig    reload="<<config_info_.reload);
}

string DataProviderController::GetConfig() {
  IceUtil::RWRecMutex::RLock lock(config_info_mutex_);
  return config_info_.reload;
}

bool DataProviderController::LoadHotShareInfo(const size_t timer_count) {
  vector<HotShareInfo> hotshare_vector;
  if ( !DataProviderDB::GetHotShare(max_db_id_, hotshare_vector) )
    return false;
  GetHeadUrl(hotshare_vector); 
  GetAlbumCoverLargeUrl(hotshare_vector);
  /*
  for (size_t i=0; i<hotshare_vector.size(); i++) {
    MCE_DEBUG("DataProviderController::LoadHotShareInfo  i:"<<i<<"  id:"<<hotshare_vector[i].id <<"   title:"<<hotshare_vector[i].title 
        << "   url:"<<hotshare_vector[i].url<<"    summary:"<<hotshare_vector[i].summary<<"    creation_date:"<<hotshare_vector[i].creation_date
        <<"    release_date:"<<hotshare_vector[i].release_date<<"    user_name:"<<hotshare_vector[i].user_name << "   thumb_url:"<<hotshare_vector[i].thumb_url
         << "   thumb_url1:"<<hotshare_vector[i].thumb_url1<<"    head_url:"<<hotshare_vector[i].head_url<<"    catagory:"<<hotshare_vector[i].catagory);
  }
 */
  StoreHotShareIntoBuffer(hotshare_vector);

  if (timer_count == kFetchOnlineVideoFrequency) {
    vector<OnlineVideoCatagory> online_video_catagory_vector;
    vector<OnlineVideoProgram> online_video_program_vector;
    GetOnlineInfoFromFile(online_video_catagory_vector, online_video_program_vector);
    MCE_DEBUG("DataProviderController::LoadHotShareInfo    online_video_catagory_vector.size:"<< online_video_catagory_vector.size()
        << "    online_video_program_vector.size:"<< online_video_program_vector.size());

    StoreOnlineVideoIntoBuffer(online_video_catagory_vector, online_video_program_vector);
  }
  return true;
}

void DataProviderController::StoreOnlineVideoIntoBuffer(std::vector<OnlineVideoCatagory> &online_video_catagory_vector, 
    std::vector<OnlineVideoProgram> &online_video_program_vector) {
  string catagory_xml = XMLGenerator::OutputXMLOnlineVideoCatagory(online_video_catagory_vector, online_video_program_vector);
  if (catagory_xml != "") {
    MCE_DEBUG("DataProviderController::StoreOnlineVideoIntoBuffer   catagory_xml:"<<catagory_xml);
    DataProviderBuffer::instance().AddOnlineVideoCatagory(catagory_xml);
  }
  for (vector<OnlineVideoProgram>::iterator online_video_program_vector_iter=online_video_program_vector.begin();
      online_video_program_vector_iter!=online_video_program_vector.end(); online_video_program_vector_iter++ ) {
    vector<OnlineVideoProgramDBInfo> online_video_program_dbinfo_vector;
    if (DataProviderDB::GetOnlineVideo(online_video_program_dbinfo_vector, online_video_program_vector_iter->catagory_id)) {
      string program_xml = XMLGenerator::OutputXMLOnlineVideoProgramList(online_video_program_dbinfo_vector, online_video_program_vector_iter->catagory_id);
      MCE_DEBUG("DataProviderController::StoreOnlineVideoIntoBuffer   catagory_id:"<<online_video_program_vector_iter->catagory_id
          << "     program_xml:"<<program_xml);
      DataProviderBuffer::instance().AddOnlineVideoProgram(online_video_program_vector_iter->catagory_id, program_xml);
    }
  }
  if (online_video_program_vector.size() > 0)
    DataProviderBuffer::instance().AddOnlineVideoProgramDone();
}

bool DataProviderController::StoreHotShareIntoBuffer(vector<HotShareInfo> &hotshare_vector) {
  for (vector<HotShareInfo>::const_iterator hotshare_vector_iter=hotshare_vector.begin(); hotshare_vector_iter!=hotshare_vector.end(); 
      hotshare_vector_iter++) {
    string hotshare_xml;
    size_t hotshare_item_type = 0;
    if (hotshare_vector_iter->catagory == "000100010001") {
      hotshare_item_type = 1;
      hotshare_xml = XMLGenerator::OutputXMLVideo(*hotshare_vector_iter);
    } else if (hotshare_vector_iter->catagory=="000100010002" && hotshare_vector_iter->thumb_url1!="") {
      hotshare_item_type = 2;
      string album_id, owner_id;
      GetAlbumId(hotshare_vector_iter->url, album_id, owner_id);
      hotshare_xml = XMLGenerator::OutputXMLAlbum(*hotshare_vector_iter, atol(album_id.c_str()));
      
      vector<string> photo_list_vector;
      GetPhotoList(album_id, owner_id, photo_list_vector);
      string photo_list = XMLGenerator::OutputXMLPhotoList(photo_list_vector);
      DataProviderBuffer::instance().AddPhoto(atol(album_id.c_str()), photo_list);
      DataProviderBuffer::instance().AddAlbumXML(hotshare_xml, atol(album_id.c_str()));
    }
    if (hotshare_item_type != 2)
      DataProviderBuffer::instance().AddHotShareXML(hotshare_xml);
  }
  return true;
}

void DataProviderController::RemoveSpace(string& str) {
  string buff(str);
  char space = ' ';
  str.assign(buff.begin() + buff.find_first_not_of(space),
      buff.begin() + buff.find_last_not_of(space) + 1);
}

void DataProviderController::GetOnlineInfoFromFile(vector<OnlineVideoCatagory> &online_video_catagory_vector, 
    vector<OnlineVideoProgram> &online_video_program_vector) {
  string catagory_name;
  ifstream fin(kOnlineVideoFilePath);
  string s;
  while(getline(fin,s)) {
    RemoveSpace(s);
    int pos = s.find('#',0);
    if (pos == 0) {
      OnlineVideoCatagory ovc;
      int space_pos = s.find(' ',1);
      if (space_pos == string::npos) {
        catagory_name = s.substr(1, s.length()-1);
        ovc.catagory_name = catagory_name;
        ovc.new_push = 0;
      } else {
        catagory_name = s.substr(1, space_pos-1);
        while (s[space_pos++] == ' '); space_pos--;
        string temp_date = s.substr(space_pos, s.length()-space_pos+1);
        string push_date;
        for (int i=0; i<temp_date.length()&&push_date.length()<=14; ++i)
          if (temp_date[i]>='0' && temp_date[i]<='9')
            push_date += temp_date[i];
        ovc.catagory_name = catagory_name;
        ovc.push_date = push_date;
        if (TimeHandler::CompareCurrentTime(push_date) > 0)
          ovc.new_push = 1;
        else 
          ovc.new_push = 0;
      }
      online_video_catagory_vector.push_back(ovc);
    } else if (pos == string::npos) {
      OnlineVideoProgram ovp;
      int space_pos1 = s.find(' ',1);
      if (space_pos1 == string::npos) {
        MCE_WARN("DataProviderController::GetOnlineInfoFromFile    " <<s <<":program error!");
      } else {
        string title = s.substr(0, space_pos1);
        ovp.catagory_name = catagory_name;
        ovp.program_name = title;
        while (s[space_pos1++] == ' ');   space_pos1--;
        size_t program_id_length = 0;
        int space_pos2 = s.find(' ',space_pos1+1);
        if (space_pos2 == string::npos) {
          program_id_length = s.length()-space_pos1+1;
        } else {
          program_id_length = space_pos2 - space_pos1;
        }
        string temp_program_id = s.substr(space_pos1, program_id_length);
        string program_id;
        for (int i=0; i<temp_program_id.length()&&program_id.length()<=12; ++i)
          if (temp_program_id[i]>='0' && temp_program_id[i]<='9')
            program_id += temp_program_id[i];
        ovp.catagory_id = program_id;
        ovp.new_push = 0;
        if (space_pos2 != string::npos) {
          while (s[space_pos2++] == ' ');   space_pos2--;
          string temp_push_date = s.substr(space_pos2, s.length()-space_pos2);
          string push_date;
          for (int i=0; i<temp_push_date.length()&&push_date.length()<=14; ++i)
            if (temp_push_date[i]>='0' && temp_push_date[i]<='9')
              push_date += temp_push_date[i];
          ovp.push_date = push_date;
        if (TimeHandler::CompareCurrentTime(push_date) > 0)
          ovp.new_push = 1;
        }
      }
      MCE_DEBUG("DataProviderController::GetOnlineInfoFromFile    OnlineVideoProgram: ovp.catagory_name:"<< ovp.catagory_name 
          << "   ovp.program_name:"<<ovp.program_name << "   ovp.catagory_id:"<<ovp.catagory_id<<"    ovp.new_push:"<<ovp.new_push
          << "   ovp.push_date:"<< ovp.push_date );
      online_video_program_vector.push_back(ovp);
    }
  }
}

bool DataProviderController::GetAlbumId(const string &url, string &album_id, string &owner_id) {
  size_t getalbum_pos = url.find("getalbum");
  if (getalbum_pos != string::npos) {
    size_t id_pos = url.find("id=");
    if (id_pos == string::npos)
      return false;
    size_t owner_pos = url.find("&owner=");
    if (owner_pos == string::npos)
      return false;
    size_t i = 0;
    for (i=id_pos+3; i<url.length(); ++i) {
      if (url[i]>='0' && url[i]<='9')
        album_id += url[i];
      else
        break;
    }
    for (i=owner_pos+7; i<url.length(); ++i) {
      if (url[i]>='0' && url[i]<='9')
        owner_id += url[i];
      else
        break;
    }
  } else {
    size_t photo_pos = url.find("photo");
    if (photo_pos != string::npos) {
      size_t photo_pos = url.find("photo/");
      if (photo_pos == string::npos)
        return false;
      size_t album_pos = url.find("album-");
      if (album_pos == string::npos)
        return false;
      size_t i = 0;

      for (i=album_pos+6; i<url.length(); ++i) {
        if (url[i]>='0' && url[i]<='9')
          album_id += url[i];
        else
          break;
      }
      for (i=photo_pos+6; i<url.length(); ++i) {
      if (url[i]>='0' && url[i]<='9')
        owner_id += url[i];
      else
        break;
      }
    }
  }
  return true;
}

void DataProviderController::ParsePhotoJSON(const JSONNode &n,  vector<PhotoInfo> &photoInfoVec) { 
  PhotoInfo pi;
  JSONNode::const_iterator i = n.begin();
  while (i != n.end()){
    if (i -> type() == JSON_ARRAY || i -> type() == JSON_NODE){
      ParsePhotoJSON(*i, photoInfoVec);
    }
    std::string node_name = i -> name();
    if (node_name == "headUrl")
      pi.headUrl = i->as_string();
    else if (node_name == "largeUrl")
      pi.largeUrl = i->as_string();
    else if (node_name == "atTitle")
      pi.atTitle = i->as_string();
    else if (node_name == "linkUrl")
      pi.linkUrl = i->as_string();
    else if (node_name == "id")
      pi.photoId = i->as_string();
    ++i;
  }
  if (pi.headUrl!="" && pi.largeUrl!="" && pi.linkUrl!="" && pi.photoId!="")
    photoInfoVec.push_back(pi);
}
  
void DataProviderController::GetPhotoInfo(const string photo_list_json, vector<PhotoInfo> &photoInfoVec) {
  JSONNode n = libjson::parse(photo_list_json);
  ParsePhotoJSON(n, photoInfoVec);
}

void DataProviderController::ParseAlbumJSON(const JSONNode &n,  string &thumb_url1, string &description, string &owner_id) { 
  JSONNode::const_iterator i = n.begin();
  while (i != n.end()){
    if (i -> type() == JSON_ARRAY || i -> type() == JSON_NODE){
      ParseAlbumJSON(*i, thumb_url1, description, owner_id);
    }
    std::string node_name = i -> name();
    if (node_name == "mainUrl")
      thumb_url1 = i->as_string();
    else if (node_name == "description") 
      description = i->as_string();
    else if (node_name == "owner")
      owner_id = i->as_string();
    /*
    else if (node_name == "errorCode")
      thumb_url1 == "error";
    else if (node_name == "message")
      description = i->as_string();
      */
    ++i;
  }
}
  
void DataProviderController::GetAlbumInfo(const string album_json, string &thumb_url1, string &description, string &owner_id) {
  JSONNode n = libjson::parse(album_json);
  ParseAlbumJSON(n, thumb_url1, description, owner_id);
}

void DataProviderController::GetAlbumCoverLargeUrl(vector<HotShareInfo> &hotshare_vector) {
  for (size_t i=0; i<hotshare_vector.size(); ++i) {
    if (hotshare_vector[i].catagory == "000100010002") {
      string owner_id, album_id;
      if (!GetAlbumId(hotshare_vector[i].url, album_id, owner_id)) {
       // MCE_DEBUG("DataProviderController::GetAlbumCoverLargeUrl  ---> thumb_url:"<<hotshare_vector[i].thumb_url <<"   album_id:"<<album_id<<"   owner_id:"<<owner_id);
        continue;
      }
      ostringstream ss;
      ss << "http://gadget.talk.renren.com/photo/" << owner_id <<"/album-" << album_id;
      string thumb_url_json = GetJsonFromUGC(ss.str());
      if (thumb_url_json != "") {
        string thumb_url1,  description, owner_id;
        GetAlbumInfo(thumb_url_json, thumb_url1, description, owner_id);
        MCE_DEBUG("DataProviderController::GetAlbumCoverLargeUrl  ---> thumb_url_json:"<<thumb_url_json
            <<"    hotshare_vecotr.id:"<<hotshare_vector[i].id<<"    thumb_url1:"<<thumb_url1 
            <<"    descrption:"<<description<<"  url:"<<ss.str() );
        hotshare_vector[i].thumb_url1 = thumb_url1;
        hotshare_vector[i].summary = description;
        hotshare_vector[i].owner_id = atoi(owner_id.c_str());
      }
    }
  }
}

void DataProviderController::GetPhotoList(string album_id, string owner_id, vector<string> &photo_list_vector) {
  vector<PhotoInfo> temp_photo_list_vector;
  size_t page_pos = 0;
  const size_t page_size = 20;

  while (true) {
    ostringstream ss;
    ss << "http://gadget.talk.renren.com/photo/"<<owner_id<<"/album-"<<album_id<<"/photo?pattern=page&page="<<page_pos<<"&count="<<page_size;
//    MCE_DEBUG("DataProviderController::GetPhotoList  ---> url:" << ss.str());
    string photo_json = GetJsonFromUGC(ss.str());
    if (photo_json.length() < 10)
      break;
//    MCE_DEBUG("DataProviderController::GetPhotoList  ---> photo_json:" << photo_json);
    GetPhotoInfo(photo_json, temp_photo_list_vector);
//    MCE_DEBUG("DataProviderController::GetPhotoList  ---> temp_photo_list_vector:" << temp_photo_list_vector.size());
    page_pos++;
  }
  for (vector<PhotoInfo>::const_iterator temp_photo_list_vector_iter=temp_photo_list_vector.begin(); temp_photo_list_vector_iter!=temp_photo_list_vector.end();
      temp_photo_list_vector_iter++ ) {
    string xml = XMLGenerator::OutputXMLPhoto(*temp_photo_list_vector_iter);
    if (xml != "")
      photo_list_vector.push_back(xml);  
  }
}

size_t WriteCallback(void *ptr, size_t size, size_t nmemb, void *data) {
  CurlBuffer *buffer  = (CurlBuffer *)data;
  memcpy(buffer->buffer_ptr_+buffer->buffer_size_, ptr, size*nmemb);
  buffer->buffer_size_ += size*nmemb;
  buffer->buffer_ptr_[buffer->buffer_size_] = 0;
  return size*nmemb;
}

string DataProviderController::GetJsonFromUGC(const string url) {
  if (curl_code_ != CURLE_OK) {       
      MCE_WARN("DataProviderController::GetJsonFromUGC  Message---->    Fail to curl_global_init");    
     return "";    
  }

  url_buffer_.buffer_size_ = 0;
  CURLcode ret ;
  CURL *curl = curl_easy_init();
  if (curl) {
//    curl_easy_setopt(curl, CURLOPT_POST, 0);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&url_buffer_);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 15000);         
    struct curl_slist *slist=NULL;
    slist = curl_slist_append(slist, "user-id:221666396");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
    ret = curl_easy_perform(curl);
  }
  long response_code = 0;
  ret = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
  double download_size = 0;
  ret = curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &download_size);

  curl_easy_cleanup(curl);
  string json;
  if (response_code==200 && download_size>0)
    json = string(url_buffer_.buffer_ptr_, url_buffer_.buffer_size_);
  
  return json;
}

bool DataProviderController::GetHeadUrl(std::vector<HotShareInfo> &hotshare_vector) {
  for (vector<HotShareInfo>::iterator iter=hotshare_vector.begin(); iter!=hotshare_vector.end(); ++iter) {
    UserUrlInfoPtr u;
    try {
      u = UserUrlAdapter::instance().getUserUrl(iter->user_id);
    } catch (Ice::Exception &) {
      MCE_WARN("DataProviderController::GetUserUrl Message---->    Fail to get users' urls");
      continue;
    }
    if (u) {
      iter->head_url = "http://hdn.xnimg.cn/photos/"+u->tinyUrl();
    }
  }
  return true;
}

bool DataProviderController::GetDataFromHotShareRequestBody(const string &_post_body, vector<Ice::Long> &high_id_vector, vector<Ice::Long> &low_id_vector, 
    size_t &size , size_t &type, Ice::Long &max_id ) {
  if (_post_body == "")
    return false;
     
  MCE_DEBUG("DataProviderController::GetDataFromHotShareRequestBody(  Message---->    post_body:"<<_post_body.c_str());
  size = 0;
  max_id = 0;
  type = 0;     
  std::string source , md5;
  
  vector<string> data_vector;
  const char *data = _post_body.c_str();
  boost::split(data_vector, data, boost::is_any_of("&"), boost::token_compress_on);
  if ( data_vector.size() > 0) {
    for (vector<string>::const_iterator iter=data_vector.begin(); iter!=data_vector.end(); ++iter) {
      if ((*iter).length() > 0) {
        size_t pos = (*iter).find_first_of('=');
        if (pos!=string::npos && (*iter).substr(pos+1).length()>0) {
          string key = (*iter).substr(0, pos);
          if (key == "highid")
            high_id_vector.push_back( atoll(((*iter).substr(pos+1, (*iter).length()-pos-1)).c_str()));
          else if (key == "lowid")
            low_id_vector.push_back( atoll( (*iter).substr(pos+1, (*iter).length()-pos-1).c_str() ));
          else if (key=="size" )
            size = static_cast<size_t>(atoi((*iter).substr(pos+1, (*iter).length()-pos-1).c_str() ));
          else if (key=="maxid")
            max_id = atoll( (*iter).substr(pos+1, (*iter).length()-pos-1).c_str());
          else if (key=="type")
            type = static_cast<size_t>(atoi((*iter).substr(pos+1, (*iter).length()-pos-1).c_str() ));
          else if (key == "bottom")
            source = (*iter).substr(pos+1, (*iter).length()-pos-1);  
          else if (key == "top")
            md5 = (*iter).substr(pos+1, (*iter).length()-pos-1);  
        }
      }
    }
  }

//  if ( !CheckMD5(source, md5) )
//   return false;
  if (high_id_vector.size() != low_id_vector.size())
    return false;

  return true;
}

bool DataProviderController::GetDataFromPhotoListRequestBody(const string &_post_body, string &album_id, string &owner_id ) {
  if (_post_body == "")
    return false;

  std::string source , md5;
  vector<string> data_vector;
  const char *data = _post_body.c_str();
  boost::split(data_vector, data, boost::is_any_of("&"), boost::token_compress_on);
  if ( data_vector.size() > 0) {
    for (vector<string>::const_iterator iter=data_vector.begin(); iter!=data_vector.end(); ++iter) {
      if ((*iter).length() > 0) {
        size_t pos = (*iter).find_first_of('=');
        if (pos!=string::npos && (*iter).substr(pos+1).length()>0) {
          string key = (*iter).substr(0, pos);
          if (key=="albumid" )
            album_id = (*iter).substr(pos+1, (*iter).length()-pos-1) ;
          else if (key=="ownerid")
            owner_id = (*iter).substr(pos+1, (*iter).length()-pos-1) ;
          else if (key == "bottom")
            source = (*iter).substr(pos+1, (*iter).length()-pos-1);  
          else if (key == "top")
            md5 = (*iter).substr(pos+1, (*iter).length()-pos-1);  
        }
      }
    }
  }
  if (album_id == "")
    return "";
//  if ( !CheckMD5(source, md5) )
//   return false;

  return true;
}

bool DataProviderController::GetDataFromCatagoryRequestBody(const string &_post_body) {
  if (_post_body == "")
    return false;

  std::string source , md5;
  vector<string> data_vector;
  const char *data = _post_body.c_str();
  boost::split(data_vector, data, boost::is_any_of("&"), boost::token_compress_on);
  if ( data_vector.size() > 0) {
    for (vector<string>::const_iterator iter=data_vector.begin(); iter!=data_vector.end(); ++iter) {
      if ((*iter).length() > 0) {
        size_t pos = (*iter).find_first_of('=');
        if (pos!=string::npos && (*iter).substr(pos+1).length()>0) {
          string key = (*iter).substr(0, pos);
          if (key == "bottom")
            source = (*iter).substr(pos+1, (*iter).length()-pos-1);  
          else if (key == "top")
            md5 = (*iter).substr(pos+1, (*iter).length()-pos-1);  
        }
      }
    }
  }
//  if ( !CheckMD5(source, md5) )
//   return false;
  return true;
}

bool DataProviderController::GetDataFromProgramRequestBody(const string &_post_body, string &program_id, string &program_size) {
  if (_post_body == "")
    return false;

  std::string source , md5;
  vector<string> data_vector;
  const char *data = _post_body.c_str();
  boost::split(data_vector, data, boost::is_any_of("&"), boost::token_compress_on);
  if ( data_vector.size() > 0) {
    for (vector<string>::const_iterator iter=data_vector.begin(); iter!=data_vector.end(); ++iter) {
      if ((*iter).length() > 0) {
        size_t pos = (*iter).find_first_of('=');
        if (pos!=string::npos && (*iter).substr(pos+1).length()>0) {
          string key = (*iter).substr(0, pos);
          if (key == "bottom")
            source = (*iter).substr(pos+1, (*iter).length()-pos-1);  
          else if (key == "top")
            md5 = (*iter).substr(pos+1, (*iter).length()-pos-1);  
          else if (key == "programid")
            program_id = (*iter).substr(pos+1, (*iter).length()-pos-1);  
          else if (key == "programsize")
            program_size = (*iter).substr(pos+1, (*iter).length()-pos-1);  
        }
      }
    }
  }
  if (program_id==""|| program_size=="")
    return false;
//  if ( !CheckMD5(source, md5) )
//   return false;
  return true;
}

bool DataProviderController::CheckMD5(string uid, const string & key) {
  uid += kMd5Key;
  unsigned char* md5 = MD5((unsigned char*) uid.c_str(), uid.size(), NULL);
  stringstream res;
  for (size_t i = 0; i < MD5_DIGEST_LENGTH; ++i) {
    res << hex << setw(2) << setfill('0') << (unsigned int) md5[i];
  }
  MCE_DEBUG("ClubDispatchKey md5 : " << res.str());
  MCE_DEBUG("ClubDispatchKey key : " << key);
  return res.str() == key;
}
