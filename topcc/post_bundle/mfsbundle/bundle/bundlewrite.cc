#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <vector>
#include <sstream>
#include <iostream>
#include"../mooseclient/moose_c.h"

#include "bundle/bundle.h"

std::string Write(const char *prefix, const char *postfix, const char *storage
  , long *tallocate, long *twrite) {
  int loop =1;  
  char ss[1024];
/*  std::ostringstream fcontent;
  while(!feof(stdin)) {
    int ret = fread(ss, 1, sizeof(ss), stdin);
    fcontent.write(ss, ret);
  }
 
  */
//  std::cout<<"start write:\n";
  char fcontent[64*1024];
  memset(fcontent,'a',64*1024);
  int file_size = 64*1024;

//  struct timeval t_start, t_enda, t_endw;
//  gettimeofday(&t_start, NULL);
  bundle::Writer* writer = bundle::Writer::Allocate(prefix, postfix
    , file_size, storage);
 // gettimeofday(&t_enda, NULL);
//  *tallocate = (t_enda.tv_sec*1000 + t_enda.tv_usec/1000)
 //   - (t_start.tv_sec*1000 + t_start.tv_usec/1000);
  std::string url;
// std::vector<std::string> v_url;
  
  if (writer) {
    size_t written;
    for(int i=0;i<1000;i++){
      int ret = writer->BatchWrite(fcontent, file_size, &written,&url);
     // sleep(1);
     // url = writer->EnsureUrl();
     // v_url.push_back(url);
     }
   // writer.Release();
    delete writer;
/*
    for(int k=0;k<v_url.size();k++){
      std::string buf;
   
    std::cout<<"v_url size"<<v_url.size()<<v_url[k]<<std::endl;
   int ret = bundle::Reader::Read(v_url[k], &buf, storage);
        std::cout<<ret<<"read ret\n";
    //  if(buf!=fcontent.str())
      std::cout<<"buf: "<<buf<<"  unequal  fcontent: "<<fcontent.str()<<'\n';

    }*/

   // gettimeofday(&t_endw, NULL);
   // *twrite = (t_endw.tv_sec*1000 + t_endw.tv_usec/1000) - (t_enda.tv_sec*1000 + t_enda.tv_usec/1000);
    return "end write";
  }
  return "";
}

#if 1
int main(int argc, char *argv[]) {

 


char tm_buf[32];
  time_t now = time(NULL);
  struct tm* ts = localtime(&now);
  snprintf(tm_buf, sizeof(tm_buf), "photo_test1/%04d%02d%02d",
    ts->tm_year + 1900, ts->tm_mon + 1, ts->tm_mday);

  const char *storage = ""; // cwd
  if (argc > 1)
    storage = argv[1];

  struct timeval t_start, t_end;
  gettimeofday(&t_start, NULL);


  for(int i=0;i<120;i++){
    pid_t child=fork();
    if(child==0){
      // sleep(5);
     //  mfs_connect("10.7.17.22:9421");
       if(mfs_connect("10.7.17.22:9421")){
        std::cout<<"connect master success!!\n";
      }
      else{
        std::cout<<"can not connect\n";
        exit(0); 
      }
      
      long dur_a, dur_w;
      std::string surl = Write(tm_buf, ".jpg", storage, &dur_a, &dur_w);
      std::cout<<surl<<std::endl;
      exit(0);
    }else if(child>0){

      std::cout<<"child progress start: "<<child<<std::endl;
    }
  }

  while(wait(NULL) != -1)
    ;
  gettimeofday(&t_end, NULL);
  long duration = (t_end.tv_sec*1000 + t_end.tv_usec/1000) 
    - (t_start.tv_sec*1000 + t_start.tv_usec/1000);

  if (argc > 2) {
    std::cout <<  " " << argv[2] << "total time: " << duration<< std::endl;
  }
  else
    std::cout <<  "total time: " << duration << " " << std::endl;
  return 0;
}
#endif
