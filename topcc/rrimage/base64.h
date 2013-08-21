#pragma once

#include <string.h>

#include <string>
#include <sstream>

#include <boost/scoped_array.hpp>

class Alphabet {
public:
  Alphabet() :
      encode((unsigned char*) "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789"
             "+/"), decode(new unsigned char[257]) {
    memset(decode.get(), 0, 256);
    for (int i = 0; i < 64; i++) {
      decode[encode[i]] = i;
    }

  }

  char e(int x) {
    return encode[x & 0x3f];
  }

private:
  const unsigned char * encode;
public:
  boost::scoped_array<unsigned char> decode;
};

extern Alphabet alphabet;

void encode(std::stringstream& ss, const char * data, int size);
std::string encode(const char * data, int size);
std::string encode(const std::string& s);

void decode(std::stringstream& ss, const std::string& s);
std::string decode(const std::string& s);

void testAlphabet();

