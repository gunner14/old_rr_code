/*
 * DbUtil.h
 *
 *  Created on: 2011-10-27
 *      Author: benjamin
 */

#ifndef DBUTIL_H_
#define DBUTIL_H_

#include <string>
#include <vector>
#include "dbpool/queryrunner.h"
#include <json/json.h>
//#include <boost/date_time/posix_time/posix_time.hpp>

struct Photo
{
	long id;
	int owner;
	long album;
	int filesize;
	std::string filename;
	std::string time;
	std::string image_data_large;
	std::string image_data_main;
	std::string image_data_head;
	std::string image_data_tiny;
	std::string image_data_xlarge;
	std::string image_data_ext;
	std::string exif;
};
class DbUtil
{
public:
	DbUtil();
	static DbUtil &getInstance();
	virtual ~DbUtil();
	std::vector<Photo> getPhotoListByAlbum(int host, long album);
	bool savePhotoInfo(std::vector<Photo> &photos);
	bool deletePhotoInfo(int host, long album);
	std::vector<Photo> Json2Photos(const std::string &str, int host, long album);
	std::string Photos2Json(const std::vector<Photo> &photos);
	std::string Photo2Json(const Photo &photo);
};

class PhotoResultHandler: public com::xiaonei::xce::ResultHandler
{
public:
	PhotoResultHandler(std::vector<Photo> &photo_list) :
		photo_list_(photo_list)
	{
	}
	;
	virtual bool handle(mysqlpp::Row &row) const;
private:
	std::vector<Photo> &photo_list_;
};

#endif /* DBUTIL_H_ */
