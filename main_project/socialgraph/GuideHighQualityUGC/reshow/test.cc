/*#include "guideuserUGCreshow.h"

using namespace xce::socialgraph;

int main(){
    
          GuideUserUGCReshow  show;
          Ice::Current        current; 
    
          FILE* fp = fopen("guide_uid_list", "r");
          if (NULL == fp){
              cout << "Userid File is not exist\n";
              return false;
          }
          char* charptr=NULL;
          ssize_t  read;
          size_t   len;
          while ((read = getline(&charptr, &len, fp)) != -1){   
                 std::string  buffer = charptr;
                 size_t    bufferlen = buffer.size();
                 if (bufferlen == 0) {   
                     continue;
                 }
                 if (buffer[bufferlen - 1] == '\r') {
                     buffer.resize (bufferlen - 1);
                 }
                 //do test
                 int  userid = (int)atoi(buffer.c_str());
                 show.NotifyUserLogin(userid, current);

                 if (charptr) {
                     free(charptr);
                     charptr = NULL;
                 }
          }
          fclose(fp);
} */
