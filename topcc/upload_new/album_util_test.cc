/*
 * album_util_test.cc
 *
 *  Created on: 2012-2-16
 *      Author: Administrator
 *      Copyright: renren-inc
 */

#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <gtest/gtest.h>
#include "upload_new/album_util.h"


TEST(ALBUM_UTIL, fmn) {
  const char* paths[] = {
      "fmn066/20120910/0845/original_0E9d_05fb0000842c118d.gif",
      "fmn065/20120910/1855/main_c5qf_6cfd00009b25125b.jpg",
  };
  const char* urls[] = {
      "http://fmn.rrimg.com/fmn066/20120910/0845/original_0E9d_05fb0000842c118d.gif",
      "http://fmn.rrimg.com/fmn065/20120910/1855/main_c5qf_6cfd00009b25125b.jpg",
  };
  for (size_t i = 0; i < sizeof(paths) / sizeof(paths[0]); ++i) {
    const std::string& url = album_util::AlbumUtil::getPhotoURL(paths[i]);
    EXPECT_EQ(url, std::string(urls[i]));
  }
}
