#include <sys/types.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <openssl/md5.h>
#include "md5_finger.h"

using namespace std;

const int MD5_CHAR_SIZE = 32;
const int MD5_HEX_SIZE = 16;

int MD5Finger::genMD5FingerHex(const string& src,unsigned char* pfinger)
{
	unsigned char tk[MD5_HEX_SIZE] = {0};

	MD5_CTX      tctx;

	MD5_Init(&tctx);
	MD5_Update(&tctx,(unsigned char*)src.c_str(), src.size());
	MD5_Final(tk, &tctx);


	memcpy(pfinger, tk, MD5_HEX_SIZE);

	return 1;
}

int MD5Finger::genMD5FingerChar(const string& src,unsigned char* pfinger)
{
	unsigned char sMd5Res[MD5_CHAR_SIZE + 1] = {0};

	MD5((unsigned char *)src.c_str(),src.size(),sMd5Res);

	memcpy(pfinger, sMd5Res , MD5_CHAR_SIZE);
	
	return 1;
}


int MD5Finger::genMD5FingerInt(const string& src,long long &md5_ret)
{
	unsigned char sMd5Res[MD5_CHAR_SIZE + 1] = {0};

	MD5((unsigned char *)src.c_str(),src.size(),sMd5Res);

	memcpy((long long*)&md5_ret, sMd5Res , sizeof(long long) );

	
	return 1;
}

