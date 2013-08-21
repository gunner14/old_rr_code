#ifndef __RRALBUM_FACEUTILS_H__
#define __RRALBUM_FACEUTILS_H__

#include <iostream>
#include <stdio.h>
#include <vector>

typedef struct tagRECT
{
      int left, top;
          int right, bottom;
}RECT;

namespace recognize
{
	typedef struct Image {
		int   id;         //图片编号
		int   width;      //图片宽度
		int   height;     //图片高度
		const unsigned char*data; //图片数据（源数据为该结构 struct RGBA : Moveable<RGBA> {byte b, g, r, a;};）
	}*ImagePtr;

	typedef struct Face {
		int    id;             //人脸id（由主程序生成）
		int    sort_id;        //自动分组id（由人脸识别模块生成）
		Image  img;            //图片数据
		RECT   rc;             //人脸在原图所在的区域
		float *feature;        //特征数据，固定长度 = GetFeatureLength()
                int    tags[6];        //预测tag列表（如可排序则排序）（tags[0]为用户指定tag、下标大于0时是模型预测的）
		bool   tagging;        //用户是否进行过命名或打勾确认操作
	}*FacePtr;

	typedef struct Classifier {
		int tag1;          //人物1的标签
		int tag2;          //人物2的标签
		float *classifier; //分类器数据，固定长度 = GetClassifierLength()
	}*ClassifierPtr;
}

#endif //__RRALBUM_FACEUTILS_H__

