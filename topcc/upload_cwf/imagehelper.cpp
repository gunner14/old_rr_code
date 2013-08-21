#include "imagehelper.h"
#include "util.h"
#include "base3/logging.h"
#include <exiv2/exiv2.hpp>


extern "C" {
#include "jpeglib.h"
#include "jerror.h"
}

/*

* function description: resize the input image using maxWidth & maxHeight

* input parameters:
*		image: the input image which need to be resized
*		maxWidth: the widht of destination image must be less the this parameter
*		maxHeight; the height of destination image must be less than this paramter

* output parameters:
*		image: the resized image if it need to be resized

* return: true if success, false if failed

*/
bool CImageHelper::ResizeImage(Image& image, int maxWidth, int maxHeight){
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
			if((double)rows > columns &&  maxHeight >= 900){
				maxHeight = 900;
			}
			height = maxHeight;
			width = columns * height / rows;
		}else{
			if((double)rows > columns &&  maxWidth >= 600){
				maxWidth = 600;
			}
			width = maxWidth;
			height = rows * width / columns;
		}
	}
	
	if(columns <= width)
		return true;

	char size[20];
	snprintf(size, sizeof(size), "%dx%d!", width, height);
	image.zoom(size);
	return true;
}

/*

* function description: sample the input image using maxWidth & maxHeight

* input parameters:
*		image: the input image which need to be resized
*		maxWidth: the widht of destination image must be less the this parameter
*		maxHeight; the height of destination image must be less than this paramter

* output parameters:
*		image: the resized image if it need to be resized

* return: true if success, false if failed

*/
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

/*
* function description: crop the input image according to width & height

*input parameters:
*		image: the image which need to be cropped
*		width: width of the destination image
*		height: height of the destination image

*output parameters:
*		image: the cropped image

*return: true if success, false is failed

*/
bool CImageHelper::CropImage(Image& image, int width, int height)
{
	int columns = image.columns();
	int rows = image.rows();
	if((columns <= width) && (rows <= height))
		return true;
	if(columns <= width){//crop height
		int yoffset = (rows - height) / 2;
		image.crop(Geometry(columns, height, 0, yoffset));
		return true;
	}else if(rows <= height){//crop width
		int xoffset = (columns - width) / 2;
		image.crop(Geometry(width, rows, xoffset, 0));
		return true;
	}else{
		int dstWidth = 0;
		int dstHeight = 0;
		int xoffset = 0;
		int yoffset = 0;
		if((double)rows/columns > (double)height/width){
			dstWidth = columns;
			dstHeight = (height*dstWidth)/width;
			xoffset = 0;
			yoffset = (rows - dstHeight) / 2;
		}else{
			dstHeight = rows;
			dstWidth = (width*dstHeight)/height;
			yoffset = 0;
			xoffset = (columns - dstWidth) / 2;
		}
		image.crop(Geometry(dstWidth, dstHeight, xoffset, yoffset));
		char size[20];
		snprintf(size, sizeof(size), "%dx%d!", width, height);
		image.zoom(size);
		return true;
	}
	return true;
}


/*

* function description: resize the input image using maxWidth & maxHeight

* input parameters:
*		source: the input image which need to be resized
*		maxWidth: the widht of destination image must be less the this parameter
*		maxHeight; the height of destination image must be less than this paramter

* output parameters:
*		image: the resized image

* return: true if success, false if failed

*/
bool CImageHelper::ResizeImage(const Image &source, int maxWidth, int maxHeight, Image &image){
	int columns = source.columns();
	int rows = source.rows();

	int width = 0; // the width of destination image
	int height = 0;// the height of destination image

	if((double)rows/columns > (double)maxHeight/maxWidth){
		if((double)rows > columns &&  maxHeight >= 900){
			maxHeight = 900;
		}
		
		height = maxHeight;
		width = columns * height / rows;
	}else{
		if((double)rows > columns &&  maxWidth >= 600){
				maxWidth = 600;
		}

		width = maxWidth;
		height = rows * width / columns;
	}

	//if(columns <= width)
	//	return false;

	char size[20];
	snprintf(size, sizeof(size),"%dx%d!", width, height);
	image.zoom(size);
	return true;
}

/*

* function description: Get the First resized Image, Using size algorithm

* input parameters:
*		data: the string which contains the blob information of image
*		maxWidth: the widht of destination image must be less the this parameter
*		maxHeight; the height of destination image must be less than this paramter
*       image: the object reference to the returned image

* return: true if success, false if failed

*/
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


/*

* function description: save the resized images to disk,format is JPEG

* input parameters:
*		vecImg: the vector which contains the resized images
*		vecImgPath: the vector which contains the save path of resized images
*		vecSaveImageConf: the vector which contains save informations

* return: true if success, false if failed

*/
bool CImageHelper::SaveJPEGImage(vector<Image> &vecImg, const vector<string> &vecImgPath, 
                                 const vector<TSaveImageConf>& vecSaveImageConf,
                                 std::vector<std::string> *post_buf) {
                     //char ** outbuf, int *outsz) {
  if (vecImg.size() != vecImgPath.size())
    return false;
  std::string buf;
  int size = vecImg.size();
  for(int i=0; i<size; ++i) {
    Image& image = vecImg[i];
    const string &path = vecImgPath[i];
    int quality = vecSaveImageConf[i].quality;
    if (quality != 0)
      image.quality(quality);
    image.interlaceType(Magick::PlaneInterlace);
    //image.magick("JPEG");
    /*为什么生成的图的文件名里面会有-0?*/
    //image.write(path);
    Blob blob;
    image.write(&blob, "JPEG");
    if (post_buf) {
      buf.assign((char*)blob.data(), blob.length());
      post_buf->push_back(buf);
    }
    if (!path.empty()) {
      if(!SaveFile(path, blob.data(), blob.length()))
        return false;
    }
  }
  return true;
}

/*给新版控件用的保存图片的方法，跟其他上传比唯一的区别是大图质量为100*/
bool CImageHelper::SaveJPEGImageX(vector<Image> &vecImg, const vector<string> &vecImgPath, const vector<TSaveImageConf>& vecSaveImageConf,
		                 char ** outbuf, int *outsz) {
	if (vecImg.size() != vecImgPath.size())
		return false;
	int size = vecImg.size();
	for(int i=0; i<size; ++i)
	{
		Image& image = vecImg[i];
		const string &path = vecImgPath[i];
		int quality = vecSaveImageConf[i].quality;
		if(i == 0)
			quality = 100;
		image.quality(quality);
		image.interlaceType(Magick::PlaneInterlace);
		//image.magick("JPEG");
		/*为什么生成的图的文件名里面会有-0?*/
		//image.write(path);
		Blob blob;
		image.write(&blob, "JPEG");
		if (i == 0 && outbuf) {
		  *outsz = blob.length();
                  *outbuf = new char[*outsz];
		  memcpy(*outbuf, blob.data(), *outsz);
		}
		if(!SaveFile(path, blob.data(), blob.length()))
			return false;
	}
	return true;
}

/*

* function description: save the resized images to disk, format is based on the image format

* input parameters:
*		vecImg: the vector which contains the resized images
*		vecImgPath: the vector which contains the save path of resized images
*		vecSaveImageConf: the vector which contains save informations

* return: true if success, false if failed

*/
bool CImageHelper::SaveImage(vector<Image> &vecImg, const vector<string> &vecImgPath, const vector<TSaveImageConf>& vecSaveImageConf){
	if (vecImg.size() != vecImgPath.size())
		return false;
	int size = vecImg.size();
	for(int i=0; i<size; ++i)
	{
		Image& image = vecImg[i];
		const string &path = vecImgPath[i];
		int quality = vecSaveImageConf[i].quality;
		image.quality(quality);
		image.interlaceType(Magick::PlaneInterlace);
		Blob blob;
		image.write(&blob);
		if(!SaveFile(path, blob.data(), blob.length()))
			return false;
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
*		image: the input image
*		vecImgSize: the vector which contains the specified size

*output parameters:
*		vecImg: the vector which contains the resized images

* return: true if success, false if failed

*/

bool CImageHelper::ResizeImage(Image &image, const vector<TSaveImageConf> &vecSaveImageConf, vector<Image> &vecImg){
	
	for(size_t i=0; i<vecSaveImageConf.size(); i++){
		TSaveImageConf saveImageConf = vecSaveImageConf[i];
		if(saveImageConf.bCrop){
			if(CropImage(image, saveImageConf.nWidth, saveImageConf.nHeight))
				vecImg.push_back(image);
			else
				return false;
		}else{
			if(ResizeImage(image, saveImageConf.nWidth, saveImageConf.nHeight))
				vecImg.push_back(image);
			else
				return false;
		}
	}
	return true;
}

bool CImageHelper::SharpenImage(const vector<TSaveImageConf> &vecSaveImageConf, vector<Image> &vecImg, int org_width){
  
  for(size_t i=0; i<vecSaveImageConf.size(); i++){
    //TSaveImageConf saveImageConf = vecSaveImageConf[i];
    Image &img = vecImg[i];
    int width = img.columns(); // TODO: Test
    int height = img.rows(); // TODO: test ,too
    if (org_width > img.columns()) // 说明缩过
      img.sharpen(0, 0.65);
    else {
    }
  }
  return true;
}
/*

* function description: Resize the input gif image to the specified size and Save

* input parameters:
*		data: the input blob
*		vecSaveImageConf: the vector contains save information
*		imgPath:path to save image
*		bResize: whether need to resize

* return: true if success, false if failed

*/

bool CImageHelper::SaveGIFImage(const string& data, const TSaveImageConf& saveImageConf, const string& imgPath, bool bResize, int *width, int *height){
	if(!bResize){
		return SaveFile(imgPath, data);
	}

	Log("handle a gif file");
	int nWidth = saveImageConf.nWidth;
	int nHeight = saveImageConf.nHeight;
	int quality = saveImageConf.quality;
	Blob blob((void*)data.c_str(), data.length());
	list<Image> imageList;
        list<Image> coalescedList;
	list<Image> desList;
        readImages(&imageList, blob);
        coalesceImages(&coalescedList, imageList.begin(), imageList.end());
        list<Image>::iterator it = coalescedList.begin();
        while(it != coalescedList.end()){
                Image& image = (*it);
		image.filterType(BoxFilter);//using a easy filter
		ResizeImage(image, nWidth, nHeight);
		image.quality(quality);
                ++it;
        }
	try{
		//尝试使用记录变化量的方法来存储gif图，但有可能出错
		deconstructImages(&desList, coalescedList.begin(), coalescedList.end());
		//save
		Blob saveBlob;   
    writeImages(desList.begin(), desList.end(), &saveBlob);
    if (width)
      *width = nWidth;
    if (height)
      *height = nHeight;
    return SaveFile(imgPath, saveBlob.data(), saveBlob.length());	
	}catch(Exception& e){
		string errorMsg = e.what();
		Log("deconstruct gif file failed %s", errorMsg.c_str());
		Blob saveBlob;   
  	writeImages(coalescedList.begin(), coalescedList.end(), &saveBlob);
		return SaveFile(imgPath, saveBlob.data(), saveBlob.length());	
	}
}

/*

* function description: identify if the image is a jpeg file

* input parameters:
*		data: the blob of the image


* return: if the image is jpeg image, return true, otherwise return false
*/
bool CImageHelper::IsJPEG(const string &data){
	if ((data.length() > 2) && (data[0] == '\xFF') && (data[1] == '\xD8'))
		return true;
	else
		return false;
}


/*

* function description: identify if the image is a gif file

* input parameters:
*		data: the blob of the image


* return: if the image is jpeg image, return true, otherwise return false
*/
bool CImageHelper::IsGIF(const string &data){
	if ((data.length() > 3)){
		if ( (data[0] == 'G') && (data[1] == 'I') && (data[2] == 'F')){
			return true;
		}
			
	}
	return false;
}

/*

* function description: identify if the image is a png file

* input parameters:
*		data: the blob of the image


* return: if the image is jpeg image, return true, otherwise return false
*/
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

/*

* function description: identify if the image is a bmp file

* input parameters:
*		data: the blob of the image


* return: if the image is jpeg image, return true, otherwise return false
*/
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
*		data: the blob of the image


* return: if the image is jpeg image, return true, otherwise return false
*/
bool CImageHelper::IsSupportImage(const string &data){
	return ( IsJPEG(data) || IsPNG(data) || IsGIF(data) || IsBMP(data));
}


bool CImageHelper::GetExif(const std::string& data,
                           std::map<string, string>& mapExif) {
  mapExif.clear();
  if (!IsJPEG(data))
    return true;
  static const string arrExifKey[] = { "Exif.Iop.InteroperabilityIndex",
      "Exif.Iop.InteroperabilityVersion", "Exif.Image.ImageWidth",
      "Exif.Image.ImageLength", "Exif.Image.BitsPerSample",
      "Exif.Image.Compression", "Exif.Image.PhotometricInterpretation",
      "Exif.Image.FillOrder", "Exif.Image.DocumentName",
      "Exif.Image.ImageDescription", "Exif.Image.Make", "Exif.Image.Model",
      "Exif.Image.StripOffsets", "Exif.Image.Orientation",
      "Exif.Image.SamplesPerPixel", "Exif.Image.RowsPerStrip",
      "Exif.Image.StripByteCounts", "Exif.Image.XResolution",
      "Exif.Image.YResolution", "Exif.Image.PlanarConfiguration",
      "Exif.Image.SMinSampleValue", "Exif.Image.SMaxSampleValue",
      "Exif.Image.GrayResponseUnit", "Exif.Image.GrayResponseCurve",
      "Exif.Image.T4Options", "Exif.Image.T6Options",
      "Exif.Image.ResolutionUnit", "Exif.Image.TransferFunction",
      "Exif.Image.Software", "Exif.Image.DateTime", "Exif.Image.Artist",
      "Exif.Image.WhitePoint", "Exif.Image.PrimaryChromaticities",
      "Exif.Image.ColorMap", "Exif.Image.HalftoneHints", "Exif.Image.TileWidth",
      "Exif.Image.TileLength", "Exif.Image.TileOffsets",
      "Exif.Image.TileByteCounts", "Exif.Image.SubIFDs", "Exif.Image.InkSet",
      "Exif.Image.InkNames", "Exif.Image.NumberOfInks", "Exif.Image.DotRange",
      "Exif.Image.TargetPrinter", "Exif.Image.ExtraSamples",
      "Exif.Image.SampleFormat", "Exif.Image.TransferRange",
      "Exif.Image.ClipPath", "Exif.Image.XClipPathUnits",
      "Exif.Image.YClipPathUnits", "Exif.Image.Indexed",
      "Exif.Image.JPEGTables", "Exif.Image.OPIProxy", "Exif.Image.JPEGProc",
      "Exif.Image.JPEGInterchangeFormat",
      "Exif.Image.JPEGInterchangeFormatLength",
      "Exif.Image.JPEGRestartInterval", "Exif.Image.JPEGLosslessPredictors",
      "Exif.Image.JPEGPointTransforms", "Exif.Image.JPEGQTables",
      "Exif.Image.JPEGDCTables", "Exif.Image.JPEGACTables",
      "Exif.Image.YCbCrCoefficients", "Exif.Image.YCbCrSubSampling",
      "Exif.Image.YCbCrPositioning", "Exif.Image.ReferenceBlackWhite",

      "Exif.Iop.RelatedImageFileFormat", "Exif.Iop.RelatedImageLength",
      "Exif.Iop.RelatedImageWidth", "Exif.Image.ImageID",
      "Exif.Image.CFARepeatPatternDim", "Exif.Image.BatteryLevel",
      "Exif.Image.Copyright", "Exif.Photo.ExposureTime", "Exif.Photo.FNumber",
      "Exif.Image.IPTCNAA", "Exif.Image.InterColorProfile",
      "Exif.Photo.ExposureProgram", "Exif.Photo.SpectralSensitivity",
      "Exif.Photo.ISOSpeedRatings", "Exif.Photo.OECF", "Exif.Photo.Interlace",
      "Exif.Photo.TimeZoneOffset", "Exif.Photo.SelfTimerMode",
      "Exif.Photo.ExifVersion", "Exif.Photo.DateTimeOriginal",
      "Exif.Photo.DateTimeDigitized", "Exif.Photo.ComponentsConfiguration",
      "Exif.Photo.CompressedBitsPerPixel", "Exif.Photo.ShutterSpeedValue",
      "Exif.Photo.ApertureValue", "Exif.Photo.BrightnessValue",
      "Exif.Photo.ExposureBiasValue", "Exif.Photo.MaxApertureValue",
      "Exif.Photo.SubjectDistance", "Exif.Photo.MeteringMode",
      "Exif.Photo.LightSource", "Exif.Image.Flash", "Exif.Photo.FocalLength",
      "Exif.Image.FlashEnergy", "Exif.Image.SpatialFrequencyResponse",
      "Exif.Image.Noise", "Exif.Image.ImageNumber",
      "Exif.Image.SecurityClassification", "Exif.Image.ImageHistory",
      "Exif.Photo.SubjectArea", "Exif.Photo.ExposureIndex",
      "Exif.Image.TIFFEPStandardID",

      "Exif.Photo.SubSecTime", "Exif.Photo.SubSecTimeOriginal",
      "Exif.Photo.SubSecTimeDigitized", "Exif.Photo.FlashpixVersion",
      "Exif.Photo.ColorSpace", "Exif.Photo.PixelXDimension",
      "Exif.Photo.PixelYDimension",
//  "exif:ExifImageLength" ,
      "Exif.Photo.RelatedSoundFile", "Exif.Image.SpatialFrequencyResponse",
      "Exif.Image.FocalPlaneXResolution", "Exif.Image.FocalPlaneYResolution",
      "Exif.Image.FocalPlaneResolutionUnit", "Exif.Image.SubjectLocation",
      "Exif.Image.ExposureIndex", "Exif.Image.TIFFEPStandardID",
      "Exif.Image.SensingMethod", "Exif.Photo.FileSource",
      "Exif.Photo.SceneType", "Exif.Photo.CFAPattern",
      "Exif.Photo.CustomRendered", "Exif.Photo.ExposureMode",
      "Exif.Photo.WhiteBalance", "Exif.Photo.DigitalZoomRatio",
      "Exif.Photo.FocalLengthIn35mmFilm", "Exif.Photo.SceneCaptureType",
      "Exif.Photo.GainControl", "Exif.Photo.Contrast", "Exif.Photo.Saturation",
      "Exif.Photo.Sharpness", "Exif.Photo.DeviceSettingDescription",
      "Exif.Photo.SubjectDistanceRange", "Exif.Photo.ImageUniqueID",
      "Exif.Image.PrintImageMatching", "Exif.GPSInfo.GPSVersionID",
      "Exif.GPSInfo.GPSLatitudeRef", "Exif.GPSInfo.GPSLatitude",
      "Exif.GPSInfo.GPSLongitudeRef", "Exif.GPSInfo.GPSLongitude",
      "Exif.GPSInfo.GPSAltitudeRef", "Exif.GPSInfo.GPSAltitude",
      "Exif.GPSInfo.GPSTimeStamp", "Exif.GPSInfo.GPSSatellites",
      "Exif.GPSInfo.GPSStatus", "Exif.GPSInfo.GPSMeasureMode",
      "Exif.GPSInfo.GPSDOP", "Exif.GPSInfo.GPSSpeedRef",
      "Exif.GPSInfo.GPSSpeed", "Exif.GPSInfo.GPSTrackRef",
      "Exif.GPSInfo.GPSTrack", "Exif.GPSInfo.GPSImgDirectionRef",
      "Exif.GPSInfo.GPSImgDirection", "Exif.GPSInfo.GPSMapDatum",
      "Exif.GPSInfo.GPSDestLatitudeRef", "Exif.GPSInfo.GPSDestLatitude",
      "Exif.GPSInfo.GPSDestLongitudeRef", "Exif.GPSInfo.GPSDestLongitude",
      "Exif.GPSInfo.GPSDestBearingRef", "Exif.GPSInfo.GPSDestBearing",
      "Exif.GPSInfo.GPSDestDistanceRef", "Exif.GPSInfo.GPSDestDistance",
      "Exif.GPSInfo.GPSProcessingMethod", "Exif.GPSInfo.GPSAreaInformation",
      "Exif.GPSInfo.GPSDateStamp", "Exif.GPSInfo.GPSDifferential" };
  Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(
      (const Exiv2::byte*) data.c_str(), data.length());
  if (image.get() == 0)
    return false;
  image->readMetadata();
  Exiv2::ExifData &exifData = image->exifData();
  if (exifData.empty()) {
    return true;
  }  
  Exiv2::ExifData::const_iterator end = exifData.end();
  for (Exiv2::ExifData::const_iterator it = exifData.begin(); it != end; ++it) {

    for (size_t i = 0; i < sizeof(arrExifKey) / sizeof(string); i++) {
      const string& key = arrExifKey[i];
      if (key == it->key()) {
        size_t pos = key.find_last_of('.');
        const std::string& value = it->value().toString();
        if (key == "Exif.Photo.PixelXDimension" && !value.empty()) {
          mapExif["ExifImageWidth"] = value;
        } else if (key == "Exif.Photo.PixelYDimension" && !value.empty()) {
          mapExif["ExifImageLength"] = value;
        } else {
          if (!value.empty())
            mapExif[key.substr(pos + 1)] = value;
        }
        break;
      }
    }
  }
  return true;
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

bool CImageHelper::GetSize(const string& data, int& nWidth, int& nHeight, string& magick)
{
	Image pingImage;
	Blob blob((void*)data.c_str(), data.length());
	pingImage.ping(blob);
	nWidth = pingImage.columns();
	nHeight = pingImage.rows();
	magick = pingImage.magick();
	if((nWidth==0) || (nHeight==0)){//通过ping没有读出来，尝试全部读取
		Image readImage;
		readImage.read(blob);
		nWidth = readImage.columns();
		nHeight = readImage.columns();
		magick = readImage.magick();
	}
	return true;
}

bool CImageHelper::ReadImage(const string& data, Image& image, int nWidth, int nHeight)
{
	Blob blob((void*)data.c_str(), data.length());
        if(nWidth == 0 && nHeight == 0)
        {   
                image.read(blob);
                return true;
        }   
	Image pingImage;
	pingImage.ping(blob);
	int columns = pingImage.columns();
	int rows = pingImage.rows();

	if((columns==0) || (rows==0))//ping读不出来
	{
		image.read(blob);
		return true;
	}

	int width = 0; // the width of destination image
	int height = 0;// the height of destination image
	if(nWidth == 0){ 
                height = nHeight;
                width = columns * height / rows;
        }else if(nHeight == 0){ 
                width = nWidth;
                height = rows * width / columns;
        }else{
                if((double)rows/columns > (double)nHeight/nWidth){
			height = nHeight;
                        width = columns * height / rows;
                }else{
			width = nWidth;
                        height = rows * width / columns;
                }   
        }   

	if(columns/2 < width){// no need to resize image
		image.read(blob);
	}else{//use size algorithm
		char size[20];
		snprintf(size,sizeof(size), "%dx%d!", width, height);
		image.size(size);
		image.read(blob);
	}
	return true;
}

void CImageHelper::RemoveAppSegment(std::string& img_data) {

  bool bAlreadyMeetExif = false;
  int img_data_len = img_data.length();
  int current_offset = 2;
  bool bIsAppMarker = true;

  if (!((img_data.length() > 2) && (img_data[0] == '\xFF')
      && (img_data[1] == '\xD8')))
    return;
  while (bIsAppMarker) {
    if (current_offset + 2 >= img_data_len)
      break;
    // read two bytes for app marker
    char current_marker[2];
    current_marker[0] = img_data[current_offset++];
    current_marker[1] = img_data[current_offset++];

    //read two bytes for segment length
    if (current_offset + 2 >= img_data_len)
      break;

    char current_segment_length[4] = { 0 };
    current_segment_length[1] = img_data[current_offset++];
    current_segment_length[0] = img_data[current_offset++];
    int *pnSegmentLength = (int *) current_segment_length;
    if (IsAPP0Marker(current_marker)) {
      current_offset = *pnSegmentLength + current_offset - 2;
    }
    //else if(IsExifMarker(current_marker))
    //{
    //  if(bAlreadyMeetExif)
    //  {
    //    //remove redundent exif
    //    bAlreadyMeetExif = true;
    //    img_data.erase(current_offset-4,*pnSegmentLength+2);
    //    img_data_len= img_data_len- (*pnSegmentLength+2);
    //    current_offset = current_offset - 4;
    //  }
    //  else
    //  {
    //    //go to segment end
    //    current_offset = *pnSegmentLength + current_offset -2;
    //    bAlreadyMeetExif = true;
    //  }
    //}
   
    else if (IsAppNMarker(current_marker)) {
      //remove segment
      img_data.erase(current_offset - 4, *pnSegmentLength + 2);
      img_data_len = img_data_len - (*pnSegmentLength + 2);
      current_offset = current_offset - 4;
    } else {
      bIsAppMarker = false;
    }
  }
}


bool CImageHelper::IsAPP0Marker(const char* str) {
  if (str && str[0] == '\xFF' && str[1] == '\xE0')
    return true;
  return false;
}

bool CImageHelper::IsAppNMarker(const char* str) {
  if (!str || str[0] != '\xFF') {
    return false;
  }
  char s[15] = { 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA,
      0xEB, 0xEC, 0xED, 0xEE, 0xEF };
  for (int i = 0; i < 15; i++) {
    if (str[1] == s[i]) {
      return true;
    }
  }
  return false;
}


