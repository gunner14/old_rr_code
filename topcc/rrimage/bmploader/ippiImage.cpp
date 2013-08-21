#include <stdlib.h>
#include <string.h>
#include "ippiImage.h"
#include "ippiImageStore.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIppiImage::CIppiImage() {
  m_pData = NULL;
  Reset();
}

void CIppiImage::Reset() {
  m_Updated = true;
  m_type = ppNONE;
  m_width = 0;
  m_height = 0;
  m_channels = 0;
  m_plane = 0;
  FreeData();
}

CIppiImage::CIppiImage(const CIppiImage& img) {
  m_pData = NULL;
  *this = img;
}

CIppiImage::~CIppiImage() {
  FreeData();
}

CIppiImage& CIppiImage::operator =(const CIppiImage& image) {
  Reset();
  m_type = image.m_type;
  m_width = image.m_width;
  m_height = image.m_height;
  m_channels = image.m_channels;
  m_plane = image.m_plane;
  m_Updated = image.m_Updated;
  if (image.m_pData) {
    AllocateData();
    memcpy(m_pData, image.m_pData, DataSize());
  }
  return *this;
}

bool CIppiImage::CreateImage(int width, int height, int channels, ppType type,
                             bool plane) {
  Reset();
  m_type = type;
  m_channels = channels;
  m_width = width;
  m_height = height;
  m_plane = plane;
  AllocateData();
  return m_pData ? true : false;
}

bool CIppiImage::CreateImage(CImgHeader header) {
  return CreateImage(header.m_width, header.m_height, header.m_channels,
                     header.m_type, header.m_plane);
}

bool CIppiImage::SaveImage(CFile* pFile) {
  CIppiImageStore store(this);
  return store.Save(pFile);
}

bool CIppiImage::LoadImage(CFile* pFile) {
  CIppiImageStore store(this);
  return store.Load(pFile);
}

int CIppiImage::Step() const {
  if (m_plane)
    return ((m_width * (Depth() >> 3) + 3) >> 2) << 2;
  else if (m_type & PP_CPLX)
    return ((m_width * m_channels * 2 * (Depth() >> 3) + 3) >> 2) << 2;
  else
    return ((m_width * m_channels * (Depth() >> 3) + 3) >> 2) << 2;
}

int CIppiImage::DataSize() const {
  if (m_plane)
    return Step() * m_height * m_channels;
  else
    return Step() * m_height;
}

void CIppiImage::AllocateData() {
  Ipp8u value = 0;
  m_pData = malloc(DataSize());
  if (!m_pData)
    return;
  memset(m_pData, value, DataSize());
}

void CIppiImage::FreeData() {
  if (m_pData)
    free (m_pData);
  m_pData = NULL;
}

void* CIppiImage::DataPtr() {
  return m_pData;
}

std::string CIppiImage::TypeString() const {
  switch (m_type) {
  case pp8u:
    return "8u";
  case pp8s:
    return "8s";
  case pp8sc:
    return "8sc";
  case pp16u:
    return "16u";
  case pp16s:
    return "16s";
  case pp16sc:
    return "16sc";
  case pp32u:
    return "32u";
  case pp32s:
    return "32s";
  case pp32sc:
    return "32sc";
  case pp32f:
    return "32f";
  case pp32fc:
    return "32fc";
  case pp64s:
    return "64s";
  case pp64sc:
    return "64sc";
  case pp64f:
    return "64f";
  case pp64fc:
    return "64fc";
  default:
    return "";
  }
}

std::string CIppiImage::ChannelString() const {
  std::string C = Plane() ? "P" : "C";
  switch (m_channels) {
  case 1:
    return C + "1";
  case 2:
    return C + "2";
  case 3:
    return C + "3";
  case 4:
    return "A" + C + "4";
  default:
    return "";
  }
}
