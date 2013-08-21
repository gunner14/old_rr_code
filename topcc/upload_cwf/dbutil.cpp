/*
 * DbUtil.cpp
 *
 *  Created on: 2011-10-27
 *      Author: benjamin
 */

#include "dbutil.h"
#include "time.h"
#include <string>
#include "base3/once.h"
#include "base3/logging.h"
#include "util.h"

static DbUtil *instance_ = NULL;

static void InitDbUtil()
{
  instance_ = new DbUtil();
}

DbUtil::DbUtil()
{
  // TODO Auto-generated constructor stub
  com::xiaonei::xce::ConnectionPoolManager::instance().initialize();
}

DbUtil::~DbUtil()
{
  // TODO Auto-generated destructor stub
}

DbUtil &DbUtil::getInstance()
{
  using namespace com::xiaonei::xce;
  static BASE_DECLARE_ONCE(once_guard_);
  base::BaseOnceInit(&once_guard_, &InitDbUtil);
  return *instance_;
}

std::vector<Photo> DbUtil::getPhotoListByAlbum(int host, long album)
{
  com::xiaonei::xce::Statement sql;
  std::string datasource = "album";
  std::string pattern = "album_" + ToString<int>(host%100);
  std::vector<Photo> photo_list_;
  sql
      << "SELECT id,owner,album,filesize,filename,time,image_data_large,image_data_main,image_data_head,image_data_tiny,image_data_xlarge,image_data_ext,exif FROM photo_to_save_" + ToString<int>(host%100) + " WHERE album = "
      << album << " ORDER BY time DESC";

  try
  {
    com::xiaonei::xce::QueryRunner(datasource,
        com::xiaonei::xce::CDbRServer,pattern).query(sql, PhotoResultHandler(
        photo_list_));
  } catch (...)
  {
    LOG(ERROR)<< "getPhotoList error, host: " << host << ", album: " << album;
  }
  return photo_list_;
}

bool DbUtil::savePhotoInfo(std::vector<Photo> &photos)
{
  using namespace std;
  for (vector<Photo>::iterator it = photos.begin(); it != photos.end(); it++)
  {
    Photo photo = *it;
    MyUtil::Str2StrMap argMap;

    string ownerStr = ToString<int>(photo.owner);
    string albumStr = ToString<long>(photo.album);
    string filesizeStr = ToString<int>(photo.filesize);

    argMap.insert(pair<string, string> ("owner", ownerStr));
    argMap.insert(pair<string, string> ("album", albumStr));
    argMap.insert(pair<string, string> ("filesize", filesizeStr));
    argMap.insert(pair<string, string> ("filename", photo.filename));
    argMap.insert(pair<string, string> ("time", TimeToStr(time(NULL))));
    argMap.insert(pair<string, string> ("image_data_large",
        photo.image_data_large));
    argMap.insert(pair<string, string> ("image_data_main",
        photo.image_data_main));
    argMap.insert(pair<string, string> ("image_data_head",
        photo.image_data_head));
    argMap.insert(pair<string, string> ("image_data_tiny",
        photo.image_data_tiny));
    argMap.insert(pair<string, string> ("image_data_ext",
        photo.image_data_ext));
    argMap.insert(pair<string, string> ("image_data_xlarge",
        photo.image_data_xlarge));
    argMap.insert(pair<string, string> ("exif", photo.exif));

    string datasource = "album";
    std::string pattern = "album_" + ToString<int>(photo.owner%100);
    string database = "photo_to_save_" + ToString<int>(photo.owner%100);
    try{
      com::xiaonei::xce::Statement sql =
        com::xiaonei::xce::Statement::buildInsertIgnore(database,
            argMap);
      com::xiaonei::xce::QueryRunner(datasource,
        com::xiaonei::xce::CDbWServer,pattern).store(sql);
    }catch(...){
      LOG(ERROR)<< "savePhoto error, host: " << ownerStr << ", album: " << albumStr;
    }
  }
}

bool DbUtil::deletePhotoInfo(int host, long album)
{
  com::xiaonei::xce::Statement sql;
  std::string datasource = "album";
  std::string pattern = "album_" + ToString<int>(host%100);
  std::string str = "DELETE FROM photo_to_save_" + ToString<int>(host%100) + " WHERE album = " + ToString<long>(
      album);
  sql << str;
  try{
    com::xiaonei::xce::QueryRunner(datasource, com::xiaonei::xce::CDbWServer,pattern).execute(
      sql, str);
  }catch(...){
    LOG(ERROR)<< "deletePhoto error, host: " << host << ", album: " << album;
  }

}

std::vector<Photo> DbUtil::Json2Photos(const std::string &str, int host,
    long album)
{
  std::vector<Photo> photos;
  Json::Value value;
  Json::Reader reader;
  Json::FastWriter fast_writer;
  if (!reader.parse(str, value))
    return photos;
  for (int i = 0; i < value["files"].size(); i++)
  {
    Photo photo;
    Json::Value val = value["files"][i];

    Json::Value large;
    Json::Value main;
    Json::Value head;
    Json::Value tiny;
    large["url"] = val["url0"];
    large["height"] = val["height0"];
    large["width"] = val["width0"];

    main["url"] = val["url1"];
    main["height"] = val["height1"];
    main["width"] = val["width1"];

    head["url"] = val["url2"];
    head["height"] = val["height2"];
    head["width"] = val["width2"];

    tiny["url"] = val["url3"];
    tiny["height"] = val["height3"];
    tiny["width"] = val["width3"];

    photo.filename = val["fileName"].asString();
    photo.filesize = val["fileSize"].asInt();
    photo.exif = fast_writer.write(val["exif"]);
    photo.image_data_ext = "";
    photo.image_data_xlarge = "";
    photo.image_data_large = fast_writer.write(large);
    photo.image_data_main = fast_writer.write(main);
    photo.image_data_head = fast_writer.write(head);
    photo.image_data_tiny = fast_writer.write(tiny);
    photo.album = album;
    photo.owner = host;
    photos.push_back(photo);
  }
  return photos;
}

std::string DbUtil::Photos2Json(const std::vector<Photo> &photos)
{
  Json::Value root;
  Json::FastWriter fast_writer;

  root["code"] = Json::Value(1);
  root["errorCount"] = Json::Value(0);
  root["msg"] = Json::Value("");
  root["totalCount"] = Json::Value((int) photos.size());

  for (std::vector<Photo>::const_iterator it = photos.begin(); it
      != photos.end(); it++)
  {
    Json::Value val;
    Photo photo = *it;
    Json::Value large;
    Json::Value main;
    Json::Value head;
    Json::Value tiny;
    Json::Reader reader;
    if (!reader.parse(photo.image_data_large, large) || !reader.parse(
        photo.image_data_main, main) || !reader.parse(
        photo.image_data_head, head) || !reader.parse(
        photo.image_data_tiny, tiny))
      continue;

    val["code"] = Json::Value(1);
    val["fileName"] = Json::Value(photo.filename);
    val["fileSize"] = Json::Value(photo.filesize);
    val["url0"] = large["url"];
    val["url1"] = main["url"];
    val["url2"] = head["url"];
    val["url3"] = tiny["url"];
    val["height0"] = large["height"];
    val["height1"] = main["height"];
    val["height2"] = head["height"];
    val["height3"] = tiny["height"];
    val["width0"] = large["width"];
    val["width1"] = main["width"];
    val["width2"] = head["width"];
    val["width3"] = tiny["width"];
    val["exif"] = Json::Value(photo.exif);
    root["files"].append(val);
  }
  LOG(INFO)<< fast_writer.write(root);
  return fast_writer.write(root);
}

std::string DbUtil::Photo2Json(const Photo &photo)
{
  Json::Value root;
  Json::FastWriter fast_writer;

  root["code"] = Json::Value(1);
  root["errorCount"] = Json::Value(0);
  root["msg"] = Json::Value("");
  root["totalCount"] = Json::Value(1);

  Json::Value val;
  Json::Value large;
  Json::Value main;
  Json::Value head;
  Json::Value tiny;
  Json::Reader reader;
  if (!reader.parse(photo.image_data_large, large) || !reader.parse(
      photo.image_data_main, main) || !reader.parse(
      photo.image_data_head, head) || !reader.parse(
      photo.image_data_tiny, tiny))
    return "";

  val["code"] = Json::Value(1);
  val["fileName"] = Json::Value(photo.filename);
  val["fileSize"] = Json::Value(photo.filesize);
  val["url0"] = large["url"];
  val["url1"] = main["url"];
  val["url2"] = head["url"];
  val["url3"] = tiny["url"];
  val["height0"] = large["height"];
  val["height1"] = main["height"];
  val["height2"] = head["height"];
  val["height3"] = tiny["height"];
  val["width0"] = large["width"];
  val["width1"] = main["width"];
  val["width2"] = head["width"];
  val["width3"] = tiny["width"];
  val["exif"] = Json::Value(photo.exif);
  root["files"].append(val);
  LOG(INFO)<< fast_writer.write(root);
  return fast_writer.write(root);
}

bool PhotoResultHandler::handle(mysqlpp::Row &row) const
{
  if (!row.empty())
  {
    Photo photo;
    photo.id = (long) row["id"];
    photo.owner = (int) row["owner"];
    photo.album = (long) row["album"];
    photo.filesize = (int) row["filesize"];
    photo.filename = (std::string) row["filename"];
    photo.time = (std::string) row["time"];
    photo.image_data_large = (std::string) row["image_data_large"];
    photo.image_data_main = (std::string) row["image_data_main"];
    photo.image_data_head = (std::string) row["image_data_head"];
    photo.image_data_tiny = (std::string) row["image_data_tiny"];
    photo.image_data_xlarge = (std::string) row["image_data_xlarge"];
    photo.image_data_ext = (std::string) row["image_data_ext"];
    photo.exif = (std::string) row["exif"];

    photo_list_.push_back(photo);
  }
  return true;
}
