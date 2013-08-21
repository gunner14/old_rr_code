#include "upload.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <boost/thread/thread.hpp>

//using namespace net;

bool GetFileBuffer(const char *file, char **buf, int* len) {
  bool b = true;
  int f = open(file, O_RDONLY);
  if (f < 0)
    return false;
  const int sz = 1024*1024*10;
  if (buf) {
    if (!len) {
      b = false;
    } else {
      *buf = new char [sz];
      ssize_t res = read(f, *buf, sz);
      if (res >= 0) {
        *len = res;
      } else {
        b = false;
      }
    }
  }
  close(f);
  return b;
}

int main(int argc, char* argv[]) {

  printf("start test\n");
  net::WebPost wp; 
  std::vector<net::Address> v;
  v.push_back(std::make_pair("10.22.206.180", 80));
  wp.Start(v);

  bool b;
  int sz;
  char *buf;
  net::HtmlData hd;

  hd.AppendHead("Cookie: storyguide=1; printphoto=1; _r01_=1; depovince=BJ; p=16bfefd72c416f2dd90250d34b65d9e06; ap=200609396; t=86b212392873f2b94f795cd7191f5c696; societyguester=76b212392873f2b94f795cd7191f5c696; id=200609396; xnsid=d3beb59f; XNESSESSIONID=5616b958802f; _aul_200609396=3; copyLinkDayShow=0"); //这次的t票
  hd.AppendBody("albumId=216301843");
  //hd.body.push_back("hostid=200609396");
  //hd.body.push_back("ishead=0");
  //hd.body.push_back("largeUrl=fmn037/20100810/1005/p_large_nnA9_1357000011bc2d14.jpg");
  hd.AppendBody("largeUrl=fmn037/20100810/1005/p_large_nnA9_1357000011bc2d14.jpg");

  /*b = GetFileBuffer("test.jpg", &buf, &sz);
  if (b)
    hd.length = sz;
  hd.buffer = buf;
  b = wp.PostFile(hd); */
 
  b = GetFileBuffer("1.jpg", &buf, &sz);
  if (b)
    hd.length = sz;
  hd.buffer = buf;
  b = wp.PostFile(hd);
  sleep(1);

  b = GetFileBuffer("2.jpg", &buf, &sz);
  if (b)
    hd.length = sz;
  hd.buffer = buf;
  b = wp.PostFile(hd);
  sleep(3);

  b = GetFileBuffer("3.jpg", &buf, &sz);
  if (b)
    hd.length = sz;
  hd.buffer = buf;
  b = wp.PostFile(hd);
  
  sleep(1115);
  printf("<>upload_test will exit\n");
  wp.Close();
  //delete buf;
  return 0;
}
