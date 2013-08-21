#include "imagehelper.h"
#include "util.h"
#include "base3/logging.h"

bool CImageHelper::SampleImage(Image& image, int maxWidth, int maxHeight){
  if(maxWidth==0 && maxHeight==0)//0,0 means no need to resize image
    return true;
  int columns = image.columns();
  int rows = image.rows();

  int width = 0; // the width of destination image
  int height = 0;// the height of destination image

  if(0 == maxWidth){//width is adjust according to height
    height = maxHeight;
    width = columns * height / rows;
  }else if(0 == maxHeight){//hegiht is adjust according to width
    width = maxWidth;
    height = rows * width / columns;
  }else{
    if((double)rows/columns > (double)maxHeight/maxWidth){
      height = maxHeight;
      width = columns * height / rows;
    }else{
      width = maxWidth;
      height = rows * width / columns;
    }
  }
  
  if(columns <= width)
    return true;

  char size[20];
  snprintf(size, sizeof(size), "%dx%d!", width, height);
  image.sample(size);
  return true;
}

bool CImageHelper::CropImage(const Image& source, int x, int y, int width, int height, Image& image) {
  image = source;
  image.crop(Geometry(width, height, x, y));
  return true;
}

bool CImageHelper::CropImage(Image& image, int x, int y, int width, int height) {
  image.crop(Geometry(width, height, x, y));
  return true;
}

bool CImageHelper::ResizeImage(const Image &source, int width, int height, Image &image){
  image = source;
  char size[20];
  snprintf(size, sizeof(size),"%dx%d!", width, height);
  image.zoom(size);
  return true;
}

bool CImageHelper::ResizeImage(Image& image, int width, int height){
  char size[20];
  snprintf(size, sizeof(size),"%dx%d!", width, height);
  image.zoom(size);
  return true;
}

bool CImageHelper::RotateImage(Image& image, double deg, int *width, int *height){
  image.rotate(deg);
  if (width)
    *width = image.columns();
  if (height)
    *height = image.rows();
  return true;
}

bool CImageHelper::GetFirstImage(const string& data, int maxWidth, int maxHeight, Image &image){
  Image pingImage;
  Blob blob((void*)data.c_str(), data.length());
  pingImage.ping(blob);
  int srcWidth = pingImage.columns();
  int srcHeight = pingImage.rows();

  int desWidth = 0; // the width of destination image
  int desHeight = 0;// the height of destination image
  if((double)srcHeight/srcWidth > (double)maxHeight/maxWidth){
    desHeight = maxHeight;
    desWidth = srcWidth * desHeight / srcHeight;
  }else{
    desWidth = maxWidth;
    desHeight = srcHeight * desWidth / srcWidth;
  }

  char size[20];
  snprintf(size, sizeof(size), "%dx%d!", desWidth, desHeight);
  if(srcWidth <= desWidth){// no need to resize image
    image.read(blob);
  }else if (srcWidth/2 < desWidth){//need to resize, but can't use size algorithm
    image.read(blob);
    image.zoom(size);
  }else{//use size algorithm
    image.size(size);
    image.read(blob);
    image.zoom(size);
  }

  return true;
}

bool CImageHelper::ImageData(const Image &source, int quality, std::string *buf, const std::string &filetype) {
  Image image = source;
  if (quality != 0)
    image.quality(quality);
  image.interlaceType(Magick::PlaneInterlace);
  //image.magick("JPEG");
  /*为什么生成的图的文件名里面会有-0?*/
  //image.write(path);
  Blob blob;
  image.write(&blob, filetype);//"JPEG");
  if (buf) {
    buf->assign((char*)blob.data(), blob.length());
  }
  return true;
}

bool CImageHelper::TransferCMYKImage(Image &image)
{
  Log("cmyk image");
  /*static Blob cmykBlob;
  static Blob rgbBlob;
  static bool isInitialized = false;
  if(!isInitialized){
    string strIccRGBFile = "./icc/RGB.icc";
    string strIccCMYKFile = "./icc/CMYK.icc";
    string strIccRGBContent = LoadFile(strIccRGBFile);
    string strIccCMYKContent = LoadFile(strIccCMYKFile);
    if((strIccRGBContent.length() != 0) && (strIccCMYKContent.length() != 0)){
      rgbBlob.update(strIccRGBContent.c_str(), strIccRGBContent.length());
      cmykBlob.update(strIccCMYKContent.c_str(), strIccCMYKContent.length());
      isInitialized = true;
    }
    
  }
  image.iccColorProfile(cmykBlob);
  image.iccColorProfile(rgbBlob);*/
  image.colorSpace(Magick::RGBColorspace);
  //image.profile("ICM", rgbBlob);
  return true;
}

/*

* function description: resize the input image to the specified size

* input parameters:
*    image: the input image
*    vecImgSize: the vector which contains the specified size

*output parameters:
*    vecImg: the vector which contains the resized images

* return: true if success, false if failed

*/

bool CImageHelper::IsJPEG(const string &data){
  if ((data.length() > 2) && (data[0] == '\xFF') && (data[1] == '\xD8'))
    return true;
  else
    return false;
}

bool CImageHelper::IsGIF(const string &data){
  if ((data.length() > 3)){
    if ( (data[0] == 'G') && (data[1] == 'I') && (data[2] == 'F')){
      return true;
    }
      
  }
  return false;
}

bool CImageHelper::IsPNG(const string &data){
  if (data.length() > 8){
    if ((data[0] =='\x89') && (data[1] == '\x50') && (data[2] == '\x4E') &&
      (data[3] == '\x47') && (data[4] == '\x0D') && (data[5] == '\x0A') &&
      (data[6] == '\x1A') && (data[7] == '\x0A')){
        return true;
    }
  }
  return false;
}

bool CImageHelper::IsBMP(const string &data){
  if ((data.length() > 2)){
    /*if ( ((data[0] == 'B') && (data[1] == 'M')) ||  
       ((data[0] == 'B') && (data[1] == 'A')) ||
       ((data[0] == 'C') && (data[1] == 'I')) ||
       ((data[0] == 'C') && (data[1] == 'P')) ||
       ((data[0] == 'I') && (data[1] == 'C')) ||
       ((data[0] == 'P') && (data[1] == 'T')) ){
        return true;
    }*/
    //BA, CI, CP, IC, PT are used for OS/2 operation system, no need to use
    if ((data[0] == 'B') && (data[1] == 'M')){
      return true;
    }
  }
  return false;
}

/*

* function description: identify if the image is a support file type(support jpg, gif, png and bmp)

* input parameters:
*    data: the blob of the image


* return: if the image is jpeg image, return true, otherwise return false
*/
bool CImageHelper::IsSupportImage(const string &data){
  return ( IsJPEG(data) || IsPNG(data) || IsGIF(data) || IsBMP(data));
}


bool CImageHelper::GetExif(Image& image, map<string, string>& mapExif)
{
//http://www.imagemagick.org/api/MagickCore/property_8c-source.html 搜索EXIFTag
static const string arrExifKey[]={
  "exif:InteroperabilityIndex" ,
  "exif:InteroperabilityVersion" ,
  "exif:ImageWidth" ,
  "exif:ImageLength" ,
  "exif:BitsPerSample" ,
  "exif:Compression" ,
  "exif:PhotometricInterpretation" ,
  "exif:FillOrder" ,
  "exif:DocumentName" ,
  "exif:ImageDescription" ,
  "exif:Make" ,
  "exif:Model" ,
  "exif:StripOffsets" ,
  "exif:Orientation" ,
  "exif:SamplesPerPixel" ,
  "exif:RowsPerStrip" ,
  "exif:StripByteCounts" ,
  "exif:XResolution" ,
  "exif:YResolution" ,
  "exif:PlanarConfiguration" ,
  "exif:PageName" ,
  "exif:XPosition" ,
  "exif:YPosition" ,
  "exif:MinSampleValue" ,
  "exif:MaxSampleValue" ,
  "exif:FreeOffsets" ,
  "exif:FreeByteCounts" ,
  "exif:GrayResponseUnit" ,
  "exif:GrayResponseCurve" ,
  "exif:T4Options" ,
  "exif:T6Options" ,
  "exif:ResolutionUnit" ,
  "exif:TransferFunction" ,
  "exif:Software" ,
  "exif:DateTime" ,
  "exif:Artist" ,
  "exif:WhitePoint" ,
  "exif:PrimaryChromaticities" ,
  "exif:ColorMap" ,
  "exif:HalfToneHints" ,
  "exif:TileWidth" ,
  "exif:TileLength" ,
  "exif:TileOffsets" ,
  "exif:TileByteCounts" ,
  "exif:SubIFD" ,
  "exif:InkSet" ,
  "exif:InkNames" ,
  "exif:NumberOfInks" ,
  "exif:DotRange" ,
  "exif:TargetPrinter" ,
  "exif:ExtraSample" ,
  "exif:SampleFormat" ,
  "exif:SMinSampleValue" ,
  "exif:SMaxSampleValue" ,
  "exif:TransferRange" ,
  "exif:ClipPath" ,
  "exif:XClipPathUnits" ,
  "exif:YClipPathUnits" ,
  "exif:Indexed" ,
  "exif:JPEGTables" ,
  "exif:OPIProxy" ,
  "exif:JPEGProc" ,
  "exif:JPEGInterchangeFormat" ,
  "exif:JPEGInterchangeFormatLength" ,
  "exif:JPEGRestartInterval" ,
  "exif:JPEGLosslessPredictors" ,
  "exif:JPEGPointTransforms" ,
  "exif:JPEGQTables" ,
  "exif:JPEGDCTables" ,
  "exif:JPEGACTables" ,
  "exif:YCbCrCoefficients" ,
  "exif:YCbCrSubSampling" ,
  "exif:YCbCrPositioning" ,
  "exif:ReferenceBlackWhite" ,
  "exif:ExtensibleMetadataPlatform" ,
  "exif:Gamma" ,
  "exif:ICCProfileDescriptor" ,
  "exif:SRGBRenderingIntent" ,
  "exif:ImageTitle" ,
  "exif:ResolutionXUnit" ,
  "exif:ResolutionYUnit" ,
  "exif:ResolutionXLengthUnit" ,
  "exif:ResolutionYLengthUnit" ,
  "exif:PrintFlags" ,
  "exif:PrintFlagsVersion" ,
  "exif:PrintFlagsCrop" ,
  "exif:PrintFlagsBleedWidth" ,
  "exif:PrintFlagsBleedWidthScale" ,
  "exif:HalftoneLPI" ,
  "exif:HalftoneLPIUnit" ,
  "exif:HalftoneDegree" ,
  "exif:HalftoneShape" ,
  "exif:HalftoneMisc" ,
  "exif:HalftoneScreen" ,
  "exif:JPEGQuality" ,
  "exif:GridSize" ,
  "exif:ThumbnailFormat" ,
  "exif:ThumbnailWidth" ,
  "exif:ThumbnailHeight" ,
  "exif:ThumbnailColorDepth" ,
  "exif:ThumbnailPlanes" ,
  "exif:ThumbnailRawBytes" ,
  "exif:ThumbnailSize" ,
  "exif:ThumbnailCompressedSize" ,
  "exif:ColorTransferFunction" ,
  "exif:ThumbnailData" ,
  "exif:ThumbnailImageWidth" ,
  "exif:ThumbnailImageHeight" ,
  "exif:ThumbnailBitsPerSample" ,
  "exif:ThumbnailCompression" ,
  "exif:ThumbnailPhotometricInterp" ,
  "exif:ThumbnailImageDescription" ,
  "exif:ThumbnailEquipMake" ,
  "exif:ThumbnailEquipModel" ,
  "exif:ThumbnailStripOffsets" ,
  "exif:ThumbnailOrientation" ,
  "exif:ThumbnailSamplesPerPixel" ,
  "exif:ThumbnailRowsPerStrip" ,
  "exif:ThumbnailStripBytesCount" ,
  "exif:ThumbnailResolutionX" ,
  "exif:ThumbnailResolutionY" ,
  "exif:ThumbnailPlanarConfig" ,
  "exif:ThumbnailResolutionUnit" ,
  "exif:ThumbnailTransferFunction" ,
  "exif:ThumbnailSoftwareUsed" ,
  "exif:ThumbnailDateTime" ,
  "exif:ThumbnailArtist" ,
  "exif:ThumbnailWhitePoint" ,
  "exif:ThumbnailPrimaryChromaticities" ,
  "exif:ThumbnailYCbCrCoefficients" ,
  "exif:ThumbnailYCbCrSubsampling" ,
  "exif:ThumbnailYCbCrPositioning" ,
  "exif:ThumbnailRefBlackWhite" ,
  "exif:ThumbnailCopyRight" ,
  "exif:LuminanceTable" ,
  "exif:ChrominanceTable" ,
  "exif:FrameDelay" ,
  "exif:LoopCount" ,
  "exif:PixelUnit" ,
  "exif:PixelPerUnitX" ,
  "exif:PixelPerUnitY" ,
  "exif:PaletteHistogram" ,
  "exif:RelatedImageFileFormat" ,
  "exif:RelatedImageLength" ,
  "exif:RelatedImageWidth" ,
  "exif:ImageID" ,
  "exif:Matteing" ,
  "exif:DataType" ,
  "exif:ImageDepth" ,
  "exif:TileDepth" ,
  "exif:CFARepeatPatternDim" ,
  "exif:CFAPattern2" ,
  "exif:BatteryLevel" ,
  "exif:Copyright" ,
  "exif:ExposureTime" ,
  "exif:FNumber" ,
  "exif:IPTC/NAA" ,
  "exif:IT8RasterPadding" ,
  "exif:IT8ColorTable" ,
  "exif:ImageResourceInformation" ,
  "exif:ExifOffset" ,
  "exif:InterColorProfile" ,
  "exif:ExposureProgram" ,
  "exif:SpectralSensitivity" ,
  "exif:GPSInfo" ,
  "exif:ISOSpeedRatings" ,
  "exif:OECF" ,
  "exif:Interlace" ,
  "exif:TimeZoneOffset" ,
  "exif:SelfTimerMode" ,
  "exif:ExifVersion" ,
  "exif:DateTimeOriginal" ,
  "exif:DateTimeDigitized" ,
  "exif:ComponentsConfiguration" ,
  "exif:CompressedBitsPerPixel" ,
  "exif:ShutterSpeedValue" ,
  "exif:ApertureValue" ,
  "exif:BrightnessValue" ,
  "exif:ExposureBiasValue" ,
  "exif:MaxApertureValue" ,
  "exif:SubjectDistance" ,
  "exif:MeteringMode" ,
  "exif:LightSource" ,
  "exif:Flash" ,
  "exif:FocalLength" ,
  "exif:FlashEnergy" ,
  "exif:SpatialFrequencyResponse" ,
  "exif:Noise" ,
  "exif:ImageNumber" ,
  "exif:SecurityClassification" ,
  "exif:ImageHistory" ,
  "exif:SubjectArea" ,
  "exif:ExposureIndex" ,
  "exif:TIFF-EPStandardID" ,
  "exif:MakerNote" ,
  "exif:WinXP-Title" ,
  "exif:WinXP-Comments" ,
  "exif:WinXP-Author" ,
  "exif:WinXP-Keywords" ,
  "exif:WinXP-Subject" ,
  "exif:UserComment" ,
  "exif:SubSecTime" ,
  "exif:SubSecTimeOriginal" ,
  "exif:SubSecTimeDigitized" ,
  "exif:FlashPixVersion" ,
  "exif:ColorSpace" ,
  "exif:ExifImageWidth" ,
  "exif:ExifImageLength" ,
  "exif:RelatedSoundFile" ,
  "exif:InteroperabilityOffset" ,
  "exif:FlashEnergy" ,
  "exif:SpatialFrequencyResponse" ,
  "exif:Noise" ,
  "exif:FocalPlaneXResolution" ,
  "exif:FocalPlaneYResolution" ,
  "exif:FocalPlaneResolutionUnit" ,
  "exif:SubjectLocation" ,
  "exif:ExposureIndex" ,
  "exif:TIFF/EPStandardID" ,
  "exif:SensingMethod" ,
  "exif:FileSource" ,
  "exif:SceneType" ,
  "exif:CFAPattern" ,
  "exif:CustomRendered" ,
  "exif:ExposureMode" ,
  "exif:WhiteBalance" ,
  "exif:DigitalZoomRatio" ,
  "exif:FocalLengthIn35mmFilm" ,
  "exif:SceneCaptureType" ,
  "exif:GainControl" ,
  "exif:Contrast" ,
  "exif:Saturation" ,
  "exif:Sharpness" ,
  "exif:DeviceSettingDescription" ,
  "exif:SubjectDistanceRange" ,
  "exif:ImageUniqueID" ,
  "exif:PrintImageMatching" ,
  "exif:GPSVersionID" ,
  "exif:GPSLatitudeRef" ,
  "exif:GPSLatitude" ,
  "exif:GPSLongitudeRef" ,
  "exif:GPSLongitude" ,
  "exif:GPSAltitudeRef" ,
  "exif:GPSAltitude" ,
  "exif:GPSTimeStamp" ,
  "exif:GPSSatellites" ,
  "exif:GPSStatus" ,
  "exif:GPSMeasureMode" ,
  "exif:GPSDop" ,
  "exif:GPSSpeedRef" ,
  "exif:GPSSpeed" ,
  "exif:GPSTrackRef" ,
  "exif:GPSTrack" ,
  "exif:GPSImgDirectionRef" ,
  "exif:GPSImgDirection" ,
  "exif:GPSMapDatum" ,
  "exif:GPSDestLatitudeRef" ,
  "exif:GPSDestLatitude" ,
  "exif:GPSDestLongitudeRef" ,
  "exif:GPSDestLongitude" ,
  "exif:GPSDestBearingRef" ,
  "exif:GPSDestBearing" ,
  "exif:GPSDestDistanceRef" ,
  "exif:GPSDestDistance" ,
  "exif:GPSProcessingMethod" ,
  "exif:GPSAreaInformation" ,
  "exif:GPSDateStamp" ,
  "exif:GPSDifferential"
};
  for(size_t i=0; i<sizeof(arrExifKey)/sizeof(string); i++){
    const string& key=arrExifKey[i];
    const string& value=image.attribute(key);
    if(!value.empty())    
      mapExif[key.substr(5)]=value;
  }
  return true;
}

bool CImageHelper::GetSize(const string& data, int& nWidth, int& nHeight, string& magick) {
  Image pingImage;
  try {
    Blob blob((void*)data.c_str(), data.length());
    pingImage.ping(blob);
    nWidth = pingImage.columns();
    nHeight = pingImage.rows();
    magick = pingImage.magick();
    if((nWidth==0) || (nHeight==0)) { //通过ping没有读出来，尝试全部读取
      Image readImage;
      readImage.read(blob);
      nWidth = readImage.columns();
      nHeight = readImage.columns();
      magick = readImage.magick();
    }
  } catch (...) {
    nWidth = 0;
    nHeight = 0;
    return false;
  }
  return true;
}

bool CImageHelper::ReadImage(const string& data, Image& image, int width, int height) {
  try {
    Blob blob((void*)data.c_str(), data.length());
    if(width == 0 || height == 0) {
      image.read(blob);
      return true;
    }
    char size[20];
    snprintf(size, sizeof(size), "%dx%d!", width, height);
    image.size(size);
    image.read(blob);
  } catch (...) {
    return false;
  }
  return true;
}

bool CImageHelper::ResizeGifData(const std::string &data, int width, int height, std::string *outdata) {
    if (!outdata)
      return false;

    Blob blob((void*)data.c_str(), data.length());
    list<Image> imageList;
    list<Image> coalescedList;
    list<Image> desList;
    readImages(&imageList, blob);
    coalesceImages(&coalescedList, imageList.begin(), imageList.end());
    for (list<Image>::iterator it = coalescedList.begin(); it != coalescedList.end(); ++it) {
      Image& image = (*it);
      image.filterType(BoxFilter);//using a easy filter
      ResizeImage(image, width, height);
      //image.quality(quality);
    } // thumbnailImage  optimizeImageLayers
    try {
      //尝试使用记录变化量的方法来存储gif图，但有可能出错
      deconstructImages(&desList, coalescedList.begin(), coalescedList.end());
      //optimizeImageLayers 它删除重复像素内容, quantizeImages 可以进一步优化
      Blob saveBlob;   
      writeImages(desList.begin(), desList.end(), &saveBlob);
      outdata->assign((char*)blob.data(), blob.length());
    } catch(Exception& e) {
      string errorMsg = e.what();
      return false;
    }
    return true;
}
