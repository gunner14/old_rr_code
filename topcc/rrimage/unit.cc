#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <sys/time.h>

#include "jpg.h"
#include "ftstring.h"
#include <errno.h>
bool SaveFileBuffer(const char*file, const char *buf, int len) {
  int f = open(file, O_RDWR | O_CREAT | O_LARGEFILE,
               S_IRUSR | S_IWUSR | S_IRGRP);
  if (f != -1) {
    int res = pwrite(f, (const void*) buf, len, 0);
    printf("\nres=%d\n", res);
    char *lErrString = strerror(errno);
    close(f);
    return true;
  }
  return false;
}

bool GetFileBuffer(const char *file, char **buf, int* len) {
  bool b = true;
  int f = open(file, O_RDONLY);
  if (f < 0)
    return false;
  const int sz = 1024 * 1024 * 20;
  if (buf) {
    if (!len) {
      b = false;
    } else {
      *buf = new char[sz];
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

static bool AppendFile(const char *path, const unsigned char* content,
                       size_t sz) {
  FILE* f = fopen(path, "wb");
  if (f == NULL) {
    if (path != "")
      printf("Open file error: %s\n", path);
    return false;
  } else {
    int res = fwrite(content, 1, sz, f);
    fclose(f);
    return true;
  }
}

void Log(const char * log) {
  AppendFile("./unit.log", log, strlen(log));
}

rtimage::JpgHandler jpg;
int fun(unsigned char* filedata, int filesize, int count, unsigned char**outbuf,
        int *size) {
  int w, h;
  wchar_t text1[] = { 38472, 0x79ef, 21033, 0 };
  wchar_t text2[] = { 'w', 'e', 'i', '.', 'c', 'o', 'm', 0 };
  int char_width = 15;
  int char_height = 15;
  int color = 0;
  std::vector<rtimage::FreeTypeString> vec;
  rtimage::FreeTypeString str1("msyh", text1, char_width, char_height, color);
  vec.push_back(str1);
  const std::string logoname = "";
  bool b = jpg.Attach(filedata, filesize, &w, &h);
  b = jpg.Decode(w, h);
  //b = jpg.Resize(90, 0, 540, 540, 100, 100);
  b = jpg.Watermark(w, h, vec, rtimage::align_bottom_center, 0, 0, logoname);
  b = jpg.EncodeJpg(85, "", outbuf, size);
  return 0;
}

inline double timesub(const struct timeval *t1, const struct timeval *t2) {
  return (double) ((t2->tv_sec * 1000000 + t2->tv_usec)
      - (t1->tv_sec * 1000000 + t1->tv_usec)) / 1000;
}

int main() {

  //IppStatus status = ippStsNoErr;
  /* print the version of ipp being used  */
  //const IppLibraryVersion* lib = ippiGetLibVersion();
  //printf("%s %s %d.%d.%d.%d\n", lib->Name, lib->Version,lib->major, lib->minor, lib->majorBuild, lib->build);
  unsigned char *filedata;
  int filesize;
  if (!GetFileBuffer("test.jpg", (char**) &filedata, &filesize)) {
    printf("get file is error\n");
    return 8;
  } else {
    printf("get file ok.\n");
  }
  int size;
  unsigned char *buf;
  struct timeval t1, t2;
  gettimeofday(&t1, 0);
  int res = fun(filedata, filesize, 10000, &buf, &size);
  gettimeofday(&t2, 0);
  printf("this time is %.0f\n", timesub(&t1, &t2));

  //if (!SaveFileBuffer("out.jpg", (const char*)buf, size)) {
  //  printf("save file is error\n");
  // return 4;
  // }
  AppendFile("out.jpg", buf, size);
  if (res != 0)
    printf("error\n");
  return 0;

}
