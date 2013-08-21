#include <stdio.h>
#include <iostream>
#include "syncdownload.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "like wget,plz input get url" << std::endl;
    return 1;
  }
  net::SyncDownload sync;
  std::string data, head;
  int code = sync.Get(argv[1],&data, &head);
  std::cout << data;
  return code;
}
