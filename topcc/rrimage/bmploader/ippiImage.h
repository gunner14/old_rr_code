#ifndef _IPPIIMAGE_H_
#define _IPPIIMAGE_H_

#include <ippi.h>
#include <string>
#include "cfile.h"

typedef struct tagRGBQUAD {
  unsigned char rgbBlue;
  unsigned char rgbGreen;
  unsigned char rgbRed;
  unsigned char rgbReserved;
} RGBQUAD;

#pragma pack(push,2)
typedef struct tagBITMAPFILEHEADER {
  unsigned short bfType;
  unsigned int bfSize;
  unsigned short bfReserved1;
  unsigned short bfReserved2;
  unsigned int bfOffBits;
} BITMAPFILEHEADER;
#pragma pack(pop)

typedef struct tagBITMAPINFOHEADER {
  unsigned int biSize;
  int biWidth;
  int biHeight;
  unsigned short biPlanes;
  unsigned short biBitCount;
  unsigned int biCompression;
  unsigned int biSizeImage;
  int biXPelsPerMeter;
  int biYPelsPerMeter;
  unsigned int biClrUsed;
  unsigned int biClrImportant;
} BITMAPINFOHEADER;

// IPP Data Types definition

#define PP_MASK  0x0FF
#define PP_SIGN  0x100
#define PP_FLOAT 0x200
#define PP_CPLX  0x400

typedef enum {
  ppNONE = 0,
  pp8u = 8,
  pp8s = 8 | PP_SIGN,
  pp8sc = 8 | PP_SIGN | PP_CPLX,
  pp16u = 16,
  pp16s = 16 | PP_SIGN,
  pp16sc = 16 | PP_SIGN | PP_CPLX,
  pp32u = 32,
  pp32s = 32 | PP_SIGN,
  pp32sc = 32 | PP_SIGN | PP_CPLX,
  pp32f = 32 | PP_FLOAT,
  pp32fc = 32 | PP_FLOAT | PP_CPLX,
  pp64s = 64 | PP_SIGN,
  pp64sc = 64 | PP_SIGN | PP_CPLX,
  pp64f = 64 | PP_FLOAT,
  pp64fc = 64 | PP_FLOAT | PP_CPLX,
} ppType;

// Image header structure
class CImgHeader {
public:
  int m_width;
  int m_height;
  ppType m_type;
  int m_channels;
  bool m_plane;
};

// Image class
class CIppiImage: public CImgHeader {
public:
  CIppiImage();
  CIppiImage(const CIppiImage& img);
  virtual ~CIppiImage();
  CIppiImage& operator =(const CIppiImage& image);

// General Operations
  // Set image header and allocate data
  bool CreateImage(int width, int height, int nChannels = 3, ppType type = pp8u,
                   bool bPlane = false);
  bool CreateImage(CImgHeader header);
  // Load image from bmp file
  bool LoadImage(CFile* pFile);
  // Save image to bmp file
  bool SaveImage(CFile* pFile);

// Attributes
  // Get image header
  CImgHeader GetHeader() const {
    return *(CImgHeader*) this;
  }
  // Get pointer to data
  void* DataPtr();
  // Get image size
  IppiSize Size() const {
    IppiSize size = { Width(), Height() };
    return size;
  }
  // Get scanline size in bytes
  int Step() const;
  // Get data size in bytes
  int DataSize() const;

  // Get image width
  int Width() const {
    return m_width;
  }
  // Get image height
  int Height() const {
    return m_height;
  }
  // Get number of image channels
  int Channels() const {
    return m_channels;
  }
  // Get plane order flag
  bool Plane() const {
    return m_plane;
  }
  // Get data type
  ppType Type() const {
    return m_type;
  }
  // Get size of data element in bits
  int Depth() const {
    return m_type & PP_MASK;
  }
  // Get signed data flag
  bool Sign() const {
    return m_type & PP_SIGN ? true : false;
  }
  // Get float data flag
  bool Float() const {
    return m_type & PP_FLOAT ? true : false;
  }
  // Get complex data flag
  bool Complex() const {
    return m_type & PP_CPLX ? true : false;
  }

  // Get string with data type description
  std::string TypeString() const;
  // Get string with channels and plane description
  std::string ChannelString() const;

  // Set data changed / unchanged flag
  // This flag must be set to true if data has been changed
  // Then CView::OnDraw function will update bitmap and
  // set this flag to false
  void IsUpdated(bool updated) {
    m_Updated = updated;
  }
  // Get data changed / unchanged flag
  // Used by CView::OnDraw function
  bool IsUpdated() const {
    return m_Updated;
  }
protected:
  void* m_pData;
  bool m_Updated;

  void Reset();
  void AllocateData();
  void FreeData();
};

#endif 
