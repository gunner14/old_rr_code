#ifndef _FT_STRING_H_
#define _FT_STRING_H_
#include <assert.h>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <sys/stat.h>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace rtimage {

class FreeType {
private:
  FT_Library library_;
  FT_Face face_;
  int char_width_;
  int char_height_;
  std::string fontfile_;
  std::string font_name_;

public:
  FreeType(const std::string& file, const std::string& name) {
    if (file.empty()) {
      std::cerr << "create Font fail: file is empty" << std::endl;
    }
    fontfile_ = file;
    font_name_ = name;
    char_width_ = 0;
    char_height_ = 0;
    FT_Init_FreeType(&library_);
    FT_New_Face(library_, file.c_str(), 0, &face_);
  }
  ~FreeType() {
    FT_Done_Face(face_);
    FT_Done_FreeType(library_);
  }
  void SetCharSize(int width, int height) {
    if (width != char_width_ || height != char_height_) {
      char_width_ = width;
      char_height_ = height;
      FT_Set_Pixel_Sizes(face_, width, height);
    }
  }

  bool RenderString(const std::wstring& str, unsigned char* canvas_buf,
                    int canvas_width, int canvas_height, int &string_width,
                    int& string_height) {
    int canvas_start_x = 0;
    int current_canvas_start_y = 0;
    int min_canvas_start_y = INT_MAX;
    int canvas_end_y = 0;
    int pen_x = 0;
    if (str.empty() || str.length() == 0 || canvas_buf == NULL
        || canvas_width == 0 || canvas_height == 0)
      return false;
    string_width = 0;
    string_height = 0;
    // memset(canvas_buf, 0xff, canvas_width * canvas_height);
    memset(canvas_buf, 0x00, canvas_width * canvas_height);

    int str_len = str.length();
    for (int char_index = 0; char_index < str_len; char_index++) {
      FT_Error error = FT_Load_Char(face_, str[char_index], FT_LOAD_RENDER);
      if (error)
        return false;
      current_canvas_start_y = char_height_ - face_->glyph->bitmap_top;

      if (current_canvas_start_y < min_canvas_start_y) {
        min_canvas_start_y = current_canvas_start_y;
      }

      int iDrawingX = pen_x + face_->glyph->bitmap_left;
      int iDrawingY = current_canvas_start_y;

      int x_max = iDrawingX + face_->glyph->bitmap.width;
      int y_max = current_canvas_start_y + face_->glyph->bitmap.rows;

      if (y_max > canvas_end_y) {
        canvas_end_y = y_max;
      }
      int index = 0;
      const unsigned char* bitmap_buffer = face_->glyph->bitmap.buffer;
#if 0
      for (int i = iDrawingY; i < y_max; i++) {
        for (int j = iDrawingX; j < x_max; j++) {
          if (bitmap_buffer[index] > 128) {
            index++;
            canvas_buf[canvas_width * i + j] = 0;
          } else {
            index++;
          }

        }
      }
#else
      for (int i = iDrawingY; i < y_max; i++) {
        for (int j = iDrawingX; j < x_max; j++) {
          canvas_buf[canvas_width * i + j] = bitmap_buffer[index++];
        }

      }
#endif
      pen_x += face_->glyph->advance.x >> 6;
    }
    string_width = pen_x;
    string_height = canvas_end_y;
    return true;
  }

};

class FreeTypeManager {
private:
  std::map<std::string, FreeType*> name_font_map;
public:
  FreeTypeManager() {
  }
  bool Init(const std::string& strFile) {
#ifdef WIN32
    FreeType* p = new FreeType("c:\\windows\\fonts\\msyh.ttf", "msyh"); //路径,名字
    if (p) {
      name_font_map.insert(std::make_pair("msyh", p));
    }
    return true;
#else
    static time_t tLastModifyTime = 0;
    struct stat st;
    if (stat(strFile.c_str(), &st) != 0) {
      std::cerr << "file not found:" << strFile << std::endl;
      return false;
    }
    if (st.st_mtime <= tLastModifyTime) {
      return true;
    }

    tLastModifyTime = st.st_mtime;
    std::ifstream fin(strFile.c_str());
    if (!fin) {
      std::cerr << "open file fail:" << strFile << std::endl;
      return false;
    }

    std::string strLine;
    while (getline(fin, strLine)) {
      size_t pos = strLine.find_first_of(':', 0);
      if (pos != std::string::npos) {
        std::string name = strLine.substr(pos + 1);
        FreeType* p = new FreeType(strLine.substr(0, pos), name); //路径,名字
        if (p) {
          name_font_map.insert(std::make_pair(name, p));
        } else {
          std::cerr << "new FreeType fail:" << std::endl;
        }
      } else {
        std::cerr << "wrong font file format " << std::endl;
        return false;
      }
    }
    return true;
#endif
  }

  FreeType* GetFont(const std::string& font_name) {
    std::map<std::string, FreeType*>::iterator iter;
    iter = name_font_map.find(font_name);
    if (iter != name_font_map.end()) {
      return iter->second;
    } else {
      return NULL;
    }
  }
  ~FreeTypeManager() {
    printf("~FreeTypeManager\n");
    std::map<std::string, FreeType*>::iterator iter;
    for (iter = name_font_map.begin(); iter != name_font_map.end(); iter++) {
      if (iter->second != NULL)
        delete iter->second;
    }

  }

};

//整个水印的对齐方式，包括图标
typedef enum enum_aligntype {
  align_type_start = 0,
  align_bottom_left,
  align_bottom_center,
  align_bottom_right,
  align_top_left,
  align_top_center,
  align_top_right,
  align_type_end,
} aligntype;

typedef struct logo_param_t {
  unsigned char* data_;
  int logo_width;
  int logo_height;
  int distance_; //和文字的距离
  unsigned char srckey[4];
  int logo_offset_x;
  int logo_offset_y;
} logo_param;

class FreeTypeString {
public:
  std::wstring text_;
  std::string font_name_;
  int char_width_;
  int char_height_;
  unsigned int color_;
  FreeTypeString() {
  }
  FreeTypeString(const std::string& fontname, const std::wstring& text,
                 int char_width, int char_height, unsigned int color) :
      font_name_(fontname), text_(text), char_width_(char_width),
          char_height_(char_height), color_(color) {

  }
};

} //namespace
#endif
