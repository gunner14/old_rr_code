#ifndef _WATERMARK_PARAM_H_
#define _WATERMARK_PARAM_H_
#include <utility>
#include <vector>
#include <iconv.h>
#include<boost/tokenizer.hpp>
#include <errno.h>
#include "ftstring.h"

namespace rtimage {

std::wstring UTF8toWCHAR(const char* src, size_t size) {
  size_t res;
  size_t nInputLen = size;
  char* input = new char[nInputLen];
  if (input == NULL)
    return std::wstring('\0');
  memcpy(input, src, nInputLen);
  size_t nOutputLen = nInputLen * 4;
  char *output = new char[nOutputLen];
  if (output == NULL) {
    delete[] input;
    return std::wstring('\0');
  }
  char *pInput = input;
  char *pOutput = output;
  iconv_t cd = iconv_open("WCHAR_T", "UTF-8");
  if (cd == (iconv_t) - 1)
    return std::wstring('\0');
  if (res = iconv(cd, &pInput, &nInputLen, &pOutput, &nOutputLen)
      == (size_t) (-1)) {
    delete[] input;
    delete[] output;
    return std::wstring('\0');
  }
  iconv_close(cd);
  int nConverted = (pOutput - output) / sizeof(wchar_t);
  std::wstring result((wchar_t*) output, nConverted);
  delete[] output;
  delete[] input;
  return result;
}

inline long int safe_atoi(const char* str) {
  if (str == NULL)
    return -1;
  for (int i = 0; str[i] != '\0'; i++) {
    if (!isdigit(str[i]))
      return -1;
  }
  char *endptr;
  long int val;
  errno = 0;
  val = strtol(str, &endptr, 10);
  if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
      || (errno != 0 && val == 0)) {
    perror("strtol");
    return -1;
  }

  if (endptr == str) {
    fprintf(stderr, "No digits were found\n");
    return -1;
  }
  return val;
}

typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

class WatermarkParam {
public:
  enum {
    WIDTH_HEIGHT, COLORS, ALIGNTYPE, HOR_DIS, VER_DIS, LOGO_NAME, FILENAME
  };

  bool Init(const std::string& param) {
    return ParseWatermarkParam(param);
  }
  bool GetWatermarkString(std::vector<std::wstring>& strings) {
    if (watermark.empty())
      return false;
    boost::char_separator<char> sep("\n");
    tokenizer tokens(watermark, sep);
    for (tokenizer::iterator tok_iter = tokens.begin();
        tok_iter != tokens.end(); ++tok_iter) {
      strings.push_back(UTF8toWCHAR(tok_iter->c_str(), tok_iter->size()));
    }
    return true;
  }
  bool GetWidthHeight(std::vector<std::pair<int, int> >& vec) {
    boost::char_separator<char> sep(",");
    tokenizer tokens(vec_strings[WIDTH_HEIGHT], sep);
    int dis = std::distance(tokens.begin(), tokens.end());
    if (dis <= 0 || 0 != dis % 2) {
      return false;
    }
    int pair_count = dis / 2;
    int i = 0;
    for (tokenizer::iterator tok_iter = tokens.begin(); i < pair_count;
        i++, ++tok_iter) {

      int w = safe_atoi(tok_iter->c_str());
      ++tok_iter;
      int h = safe_atoi(tok_iter->c_str());
      if (w == -1 || h == -1)
        return false;
      vec.push_back(std::make_pair(w, h));
    }
    return true;
  }
  bool GetColors(std::vector<unsigned int>& vec) {
    boost::char_separator<char> sep(",");
    tokenizer tokens(vec_strings[COLORS], sep);
    int dis = std::distance(tokens.begin(), tokens.end());
    if (dis <= 0)
      return false;
    for (tokenizer::iterator tok_iter = tokens.begin();
        tok_iter != tokens.end(); ++tok_iter) {
      unsigned int color = safe_atoi(tok_iter->c_str());
      //if(color > rgba都取255)
      //return false;
      vec.push_back(color);
    }
    return true;
  }
  int GetAlignType() {
    return safe_atoi(vec_strings[ALIGNTYPE].c_str());

  }
  int GetHorDistance() {
    return safe_atoi(vec_strings[HOR_DIS].c_str());
  }
  int GetVerDistance() {
    return safe_atoi(vec_strings[VER_DIS].c_str());

  }
  std::string GetLogoFileName() {
    return vec_strings[LOGO_NAME];

  }
  std::string GetFileName() {
    return vec_strings[FILENAME];
  }
  bool MakeFreeTypeString(std::vector<rtimage::FreeTypeString>& ftstrings) {
    std::vector < std::wstring > strings;
    std::vector < std::pair<int, int> > width_height;
    std::vector<unsigned int> colors;
    bool b = GetWatermarkString(strings);
    if (!b)
      return false;
    b = GetWidthHeight(width_height);
    if (!b || width_height.size() != strings.size())
      return false;
    b = GetColors(colors);
    if (!b || colors.size() != strings.size())
      return false;

    if (strings.size() == 1 && width_height[0].first == 0
        && width_height[0].second == 0 && strings[0] == L"0"
        && colors[0] == 0) {
      return true;
    }
    for (int i = 0; i < strings.size(); i++) {
      ftstrings.push_back(
          rtimage::FreeTypeString("msyh", strings[i], width_height[i].first,
                                  width_height[i].second, colors[i]));
    }

    return true;
  }
private:
  std::string watermark;
  std::vector<std::string> vec_strings;
  bool ParseWatermarkParam(const std::string& param) {
    /*
     水印utf8字符串|
     第1到第n行freetype字符宽度和高度，以逗号分隔|
     第1到第n行的颜色，以逗号分隔|
     水印对齐方式|
     与图片边界的水平距离(单位像素)|
     与图片边界的垂直距离(单位像素)|
     水印图片文件名(不包括扩展名)|
     原图文件名(不包括扩展名)
     */

    if (param.empty())
      return false;
    size_t len = param.length();
    if (len < 15)
      return false;
    int i;
    int count = 0;
    for (i = len - 1; i >= 0; i--) {
      if (param[i] == '|')
        count++;
      if (count == 7)
        break;
    }

    watermark = param.substr(0, i);

    boost::char_separator<char> sep("|");
    std::string str = param.substr(i);
    tokenizer tokens(str, sep);
    if (7 != std::distance(tokens.begin(), tokens.end()))
      return false;
    vec_strings.assign(tokens.begin(), tokens.end());
    return true;

  }
};
}

#endif

