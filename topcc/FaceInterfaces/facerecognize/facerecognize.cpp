#include <assert.h>
#pragma warning(disable: 4819)

#include "../faceclustering/faceclustering.h"
#include "../include/facerecognize.h"
#include "faceclassifier.h"
#include <iostream>
#include "intkey.h"
using namespace rralbum;
using namespace recognize;


//-------------------------------------------------------------------------------------------------
string g_sPathName_1;


//-------------------------------------------------------------------------------------------------
FaceRecognize::FaceRecognize(const char *pathname)
{
	m_pFaces = new vector<FacePtr>;
	m_pClassifiers = new vector<ClassifierPtr>;
	
	//strcpy(g_sPathName, pathname);
	g_sPathName_1.append(pathname);
	m_pClustering = IFaceClustering::createInstance();
	((IFaceClustering*)m_pClustering)->setFacePtr(m_pFaces);
}


//-------------------------------------------------------------------------------------------------
FaceRecognize::~FaceRecognize()
{
	((IFaceClustering*)m_pClustering)->release();

	vector<FacePtr>::iterator fp  = m_pFaces->begin();
	vector<FacePtr>::iterator fp1 = m_pFaces->end();
	for(; fp!=fp1; ++fp)
	{
		FacePtr p = *fp;
		delete []p->feature;
		delete p;
	}
	m_pFaces->clear();
	delete m_pFaces;
	
	vector<ClassifierPtr>::iterator cp  = m_pClassifiers->begin();
	vector<ClassifierPtr>::iterator cp1 = m_pClassifiers->end();
	for(; cp!=cp1; ++cp)
	{
		ClassifierPtr p = *cp;
		delete []p->classifier;
		delete p;
	}
	m_pClassifiers->clear();
	delete m_pFaces;
}


//-------------------------------------------------------------------------------------------------
// create an object and return the pointer, user needs to fill its values
FacePtr FaceRecognize::PushBackFace()
{
	FacePtr p = new Face;
	memset(p, 0, sizeof(Face));
	p->feature = new float[GetFeatureLength()];
	m_pFaces->push_back(p);
	return p;
}


//-------------------------------------------------------------------------------------------------
// create an object and return the pointer, user needs to fill its values
ClassifierPtr FaceRecognize::PushBackClassifier()
{
	ClassifierPtr p = new Classifier;
	memset(p, 0, sizeof(Classifier));
	p->classifier = new float[GetClassifierLength()];
	m_pClassifiers->push_back(p);
	return p;
}


//-------------------------------------------------------------------------------------------------
FacePtr FaceRecognize::GetFace(int index)
{
	return (*m_pFaces)[index];
}


//-------------------------------------------------------------------------------------------------
FacePtr FaceRecognize::FindFace(int faceid)
{
	int size = m_pFaces->size();
	for(int i=0; i<size; i++)
	{
		FacePtr p = (*m_pFaces)[i];
		if(p->id==faceid) return p;
	}
	return 0;
}


//-------------------------------------------------------------------------------------------------
int FaceRecognize::SizeFace()
{
	return m_pFaces->size();
}


//-------------------------------------------------------------------------------------------------
void FaceRecognize::EraseFace(int faceid)
{
	vector<FacePtr>::iterator it = m_pFaces->begin();
	vector<FacePtr>::iterator it1= m_pFaces->end();

	for(; it!=it1; ++it)
	{
		FacePtr p = *it;
		if(p->id==faceid)
		{
			m_pFaces->erase(it);
			return;
		}
	}
}


//-------------------------------------------------------------------------------------------------
ClassifierPtr FaceRecognize::GetClassifier(int index)
{
	return (*m_pClassifiers)[index];
}


//-------------------------------------------------------------------------------------------------
int FaceRecognize::SizeClassifier()
{
	return m_pClassifiers->size();
}


//-------------------------------------------------------------------------------------------------
bool FaceRecognize::UpdateClassifiers()
{
	int nfaces = m_pFaces->size();
	if(nfaces==0) return false;

	// find all tags
	CIntKey intkey;
	vector<FacePtr>::iterator iter1 = m_pFaces->begin();
	vector<FacePtr>::iterator iter2 = m_pFaces->end();
	for(; iter1!=iter2; ++iter1)
	{
		int key = (*iter1)->tags[0];
		if(key>0) intkey.AddKey(key); // a tagged face
	}

	// check number of classes
	int m = intkey.Size();
	if(m<2) return true;

	// collect the training set
	vector<float*> xs;
	vector<int>    ys;
	vector<bool>   tag(m);
	vector<float*> wbs;
	
	for(int i=0; i<m; i++) tag[i] = false;
	iter1 = m_pFaces->begin();
	iter2 = m_pFaces->end();
	for(; iter1!=iter2; ++iter1)
	{
		FacePtr pface = *iter1;
		int key = pface->tags[0];
		if(key>0) // a tagged face
		{
			// class label
			int label = 0;
			intkey.KeyToIndex(key, label);

			xs.push_back(pface->feature);
			ys.push_back(label);

			if(pface->tagging)
			{
				pface->tagging = false;
				tag[label] = true;
			}
		}
	}

	// check the current classifiers
	int nclassifiers = m*(m-1)/2;
	int nclassifiers0 = m_pClassifiers->size();
	if(nclassifiers0<nclassifiers)
	{
		CIntKey subset;
		if(nclassifiers0>0)
		{
			vector<ClassifierPtr>::iterator iter1 = m_pClassifiers->begin();
			vector<ClassifierPtr>::iterator iter2 = m_pClassifiers->end();
			for(; iter1!=iter2; ++iter1)
			{
				subset.AddKey((*iter1)->tag1);
				subset.AddKey((*iter1)->tag2);
			}
		}

		// increased new tags
		const int len = GetClassifierLength();	
		for(int i=0, k=0; i<m-1; i++)
		{
			int tag1 = 0;
			intkey.IndexToKey(i, tag1);
			for(int j=i+1; j<m; j++)
			{
				int tag2 = 0;
				intkey.IndexToKey(j, tag2);

				if(nclassifiers0==0)
				{
					ClassifierPtr pclassifier = new Classifier();
					pclassifier->tag1 = tag1;
					pclassifier->tag2 = tag2;
					pclassifier->classifier = new float[len];					
					m_pClassifiers->push_back(pclassifier);
				}
				else
				{
					if(!subset.ContainKey(tag1) || !subset.ContainKey(tag2)) 
					{
						// if the classifier (tag1,tag2) is not contained, insert it at position k
						ClassifierPtr pclassifier = new Classifier();
						pclassifier->tag1 = tag1;
						pclassifier->tag2 = tag2;
						pclassifier->classifier = new float[len];
						if((unsigned int)k<m_pClassifiers->size()) 
							m_pClassifiers->insert(m_pClassifiers->begin()+k, pclassifier);
						else
							m_pClassifiers->push_back(pclassifier);
					}
				}

				k++;
			}
		}
	}

	// collect all wbs
	for(int i=0, k=0; i<m-1; i++)
	{
		for(int j=i+1; j<m; j++)
		{
			float *wb = ((*m_pClassifiers)[k])->classifier;
			wbs.push_back(wb);
			k++;
		}
	}

	// train face classifiers
	const int dim = GetFeatureLength();
	IFaceClassifier *pFaceClassifier = IFaceClassifier::CreateInstance();
	pFaceClassifier->TrainLSVMs(dim, m, tag, xs, ys, 10, wbs);	
	pFaceClassifier->Release();
	
	return true;
}


//-------------------------------------------------------------------------------------------------
bool FaceRecognize::PredictFaceTag(bool bGroup, int id)
{
	if(m_pClassifiers->size()==0) return false;

	CIntKey subset;
	vector<float*> wbs;

	vector<ClassifierPtr>::iterator iter1 = m_pClassifiers->begin();
	vector<ClassifierPtr>::iterator iter2 = m_pClassifiers->end();
	for(; iter1!=iter2; ++iter1)
	{
		subset.AddKey((*iter1)->tag1);
		subset.AddKey((*iter1)->tag2);
		wbs.push_back((*iter1)->classifier);
	}
	int m = subset.Size();
	if(wbs.size()!=m*(m-1)/2) return false;

	const int dim = GetFeatureLength();
	if(bGroup)
	{
		// predict a group of faces
		vector<float*> xs;
		vector<int> l;

		// collect face features
		vector<FacePtr>::iterator iter1 = m_pFaces->begin();
		vector<FacePtr>::iterator iter2 = m_pFaces->end();
		for(; iter1!=iter2; ++iter1)
		{
			FacePtr pface = *iter1;
			if(pface->sort_id==id) xs.push_back(pface->feature);
		}
		if(xs.size()==0) return false;

		IFaceClassifier *pFaceClassifier = IFaceClassifier::CreateInstance();
		pFaceClassifier->TestLSVMs(dim, xs, m, wbs, l);
		pFaceClassifier->Release();

		// output the tags for the group
		bool update = false;
		const int ntag = min(m,5);
		for(iter1=m_pFaces->begin(); iter1!=iter2; ++iter1)
		{
			// find the face id
			FacePtr pface = *iter1;
			if(pface->sort_id==id)
			{
				int oldtag = pface->tags[1];
				for(int i=0; i<ntag; i++)
				{
					int tag = 0;
					subset.IndexToKey(l[i], tag);
					pface->tags[1+i] = tag;
				}				
				if(!update && oldtag!=pface->tags[1]) update = true;
			}
		}

		return update;
	}
	else
	{
		// predict one face
		vector<FacePtr>::iterator iter1 = m_pFaces->begin();
		vector<FacePtr>::iterator iter2 = m_pFaces->end();
		for(; iter1!=iter2; ++iter1)
		{
			// find the face id
			FacePtr pface = *iter1;
			if(pface->id==id)
			{	
				if(pface->tags[0]>0) break; // already tagged

				float *x =  pface->feature;
				vector<int> l;
				
				IFaceClassifier *pFaceClassifier = IFaceClassifier::CreateInstance();
				pFaceClassifier->TestLSVMs(dim, x, m, wbs, l);
				pFaceClassifier->Release();

				int oldtag = pface->tags[1];

				// output the tags
				const int ntag = min(m,5);
				for(int i=0; i<ntag; i++)
				{
					int tag = 0;
					subset.IndexToKey(l[i], tag);
					pface->tags[1+i] = tag;
				}

				return (oldtag!=pface->tags[1]); // complete prediction
			}
		}
	}

	return false;
}


//扫描某个图片
bool FaceRecognize::OneScan(const recognize::Image &image){
	
	((IFaceClustering*)m_pClustering)->scanImage(image);
	return true;

}

//扫描结束，获取结果
bool FaceRecognize::EndScan(){
	((IFaceClustering*)m_pClustering)->endScan();
	return true;
}


//-------------------------------------------------------------------------------------------------
int FaceRecognize::GetClassifierLength()
{
	return ((IFaceClustering*)m_pClustering)->getFeatureLength()+1;
}


//-------------------------------------------------------------------------------------------------
int FaceRecognize::GetFeatureLength()
{
	return ((IFaceClustering*)m_pClustering)->getFeatureLength();
}
