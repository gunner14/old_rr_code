#ifndef __RRALBUM_FACERECOGNIZE_H__
#define __RRALBUM_FACERECOGNIZE_H__
#include "faceutils.h"

namespace rralbum
{
	//FaceRecognize的流程:
	//1.初始化时，把数据导入pFaces和pClassifiers，然后调用SetDataPointers(...)
	//2.多次调用OneScan(...)，可检测人脸并提取特征
	//3.调用EndScan()，结束批次扫描(每次控制合适的处理文件数量，保证效率和及时的结果通知)，数据pFaces更新
	//4.调用PredictFaceTag(...)可预测某张人脸的tag或者某个聚类分组的tag
	//5.用户若进行过命名或打勾确认操作，调用方把相应的tagging设置为true
	//6.调用UpdateClassifiers()，更新分类器数据，并把tagging由true改为false
	//7.调用GetLastError()获取最近错误代码

//#define SHARED_LIBRARY
//#define EXPORT_LIBRARY

#ifdef SHARED_LIBRARY
#ifdef EXPORT_LIBRARY
	class __declspec(dllexport) FaceRecognize
#else
	class __declspec(dllimport) FaceRecognize
#endif
#else
	class FaceRecognize
#endif
	{
	public:
		FaceRecognize(const char *pathname);
		~FaceRecognize();

		// create an object and return the pointer, user needs to fill its valus
		recognize::FacePtr       PushBackFace();
        recognize::ClassifierPtr PushBackClassifier();
                
		// operate on face objects
        recognize::FacePtr       GetFace(int index);
        recognize::FacePtr       FindFace(int faceid);	
        int           SizeFace();
		void          EraseFace(int faceid);

		// operate on classifier objects
        recognize::ClassifierPtr GetClassifier(int index);
        int           SizeClassifier();
                

	public:
		int  GetFeatureLength();
		int  GetClassifierLength();

		//扫描某个图片
		bool OneScan(const recognize::Image &image);

		//扫描结束，获取结果
		bool EndScan();

		//学习分类器
		bool UpdateClassifiers();

		//预测某张人脸的tag，或者某个聚类分组的tag，结果在tags[1~5]中
		bool PredictFaceTag(bool bGroup, int id);

		//获取错误码
		int  GetLastError() const;

	protected:
		std::vector<recognize::FacePtr>       *m_pFaces;	 
		std::vector<recognize::ClassifierPtr> *m_pClassifiers;
		int m_iErrorCode; //错误代码
		
		void*  m_pClustering;
	};
}

#endif //__RRALBUM_FACERECOGNIZE_H__
