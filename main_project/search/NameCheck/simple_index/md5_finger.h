#ifndef __MD5_FINGER_H_
#define __MD5_FINGER_H_

#include <string>
#include <sys/types.h>

using namespace std;

class MD5Finger
{
	public:
		static int genMD5FingerHex(const string& src,unsigned char *finger_buf);
		static int genMD5FingerChar(const string& src,unsigned char *finger_buf);
		static int genMD5FingerInt(const string& src,long long& md5_ret);
};


#endif
