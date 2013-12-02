#include "utility.h"

bool toLower(std::string *s) {
  for (size_t i = 0; i < s->size(); ++i) {
    char c = (*s)[i];
    if ((c >= 'A') && (c <= 'Z')) {
      c += 'a' - 'A';
      (*s)[i] = c;
    }
  }
  return true;
}

size_t splitTxt(const std::string src, std::vector<std::string> &ary, const char* sep)
{
	size_t src_len=src.length();
	size_t sep_len=1;//strlen(sep);
	size_t pos=0,fpos;

	//while(pos<src.length()-1-sep_len)
	while(pos<src_len-sep_len+1)
	{
		//fpos=src.find_first_of(sep,pos);
		fpos=src.find(sep,pos);
		if(fpos==std::string::npos)
		{
			ary.push_back(src.substr(pos));
#ifdef	_DEBUG_
	printf("find:%s\n",src.substr(pos).c_str());
#endif
			break;
		}
		if(fpos>pos)
			ary.push_back(src.substr(pos,fpos-pos));
#ifdef	_DEBUG_
	printf("find:%s\n",src.substr(pos,fpos-pos).c_str());
#endif
		pos=fpos+sep_len;
	}
	return ary.size();
}


