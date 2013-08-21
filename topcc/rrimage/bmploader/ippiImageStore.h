#ifndef IPPIMAGE_STORE_H
#define IPPIMAGE_STORE_H

#include "ippiImage.h"

class CIppiImageStore {
public:
  // Constructor attaches image
  CIppiImageStore(CIppiImage* pImage) :
      m_pImage(pImage), m_pFile(NULL), m_bSave(false) {
  }
  virtual ~CIppiImageStore() {
  }

  // Save attached image to bmp file
  bool Save(CFile* pFile);
  // Load attached image from bmp file
  bool Load(CFile* pFile);
protected:
  CIppiImage* m_pImage;
  bool m_bSave;
  CFile* m_pFile;

  BITMAPFILEHEADER m_FileHeader;
  BITMAPINFOHEADER m_InfoHeader;
  RGBQUAD m_Palette[256];

  bool Error(std::string message);

  bool ValidImage(CIppiImage* pImage);
  unsigned int FileHeaderSize() {
    return sizeof(BITMAPFILEHEADER);
  }
  unsigned int InfoHeaderSize() {
    return sizeof(BITMAPINFOHEADER);
  }
  BITMAPFILEHEADER* GetFileHeader(CIppiImage* pImage);
  BITMAPINFOHEADER* GetInfoHeader(CIppiImage* pImage);
  int GetPaletteNum(CIppiImage* pImage);
  int GetPaletteSize(CIppiImage* pImage) {
    return GetPaletteNum(pImage) * sizeof(RGBQUAD);
  }
  RGBQUAD* GetGrayPalette(CIppiImage* pImage);
  int GetPaletteNum();
  int GetPaletteSize() {
    return GetPaletteNum() * sizeof(RGBQUAD);
  }
  int GetDataStep();
  int GetDataSize();
  bool IsGrayPalette();
  bool IsCompressed();

  bool ValidFileHeader();
  bool CreateImage();
  bool ValidCompression();
  bool SetImageWidth(int& width);
  bool SetImageHeight(int& height);
  bool SetImageType(ppType& type);
  bool SetImagePlane(bool& plane);
  bool SetImageChannels(int& channels);
  bool SetImageData(Ipp8u* buffer);
  bool SetImageData_1(Ipp8u* buffer);
  bool SetImageData_4(Ipp8u* buffer);
  bool SetImageData_8(Ipp8u* buffer);
  void SetPixel(Ipp8u src, Ipp8u* pDst, int x);
};

#endif // !defined(AFX_IMAGESTORE_H__84B80914_1314_4D48_933F_A141F6FA5A64__INCLUDED_)
