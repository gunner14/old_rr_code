#include "faceclusteringimpl.h"
#include <algorithm>
#include "../thirdparty/cluster/cluster.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace std;

#ifdef LOG
	#include <fstream>
	using namespace  std;
	ofstream ofs("log");
	#define LOGOUT ofs
#else
#include <iostream>
using namespace std;  
	#define LOGOUT cout
#endif

extern string g_sPathName;

IFaceClustering* IFaceClustering::createInstance(){
	
	IFaceClustering* fc = new FaceClustering();
	return fc;
}

FaceClustering::FaceClustering(void)
{
	m_pFaceDetector = IFaceDetector::createInstance();
	m_pFaceDescriptor = IFaceFeature::CreateInstance("data\\facelandmark");
	m_pVecFaces = 0;
	m_dDistT = 0.25;
}

FaceClustering::~FaceClustering(void)
{
}

/****
multithread????
***/
void FaceClustering::scanImage(const recognize::Image& img){

	vector<FRECT> faces;
	cv::Mat bgra(img.height, img.width, CV_8UC4, (unsigned char*)img.data);
	cv::Mat gray;
	cv::cvtColor(bgra, gray, CV_BGRA2GRAY);

	m_pFaceDetector->detectFace(gray.data, img.width, img.height, faces);
	for(unsigned i=0; i<faces.size(); i++){

		int x0 = faces[i].x;
		int y0 = faces[i].y;
		int x1 = x0 + faces[i].w-1; 
		int y1 = y0 + faces[i].h-1;
		
		RECT rc;
		rc.bottom = y1;
		rc.left = x0;
		rc.right = x1;
		rc.top = y0;

		recognize::FacePtr face = new recognize::Face;
		memset(face, 0, sizeof(recognize::Face));
		face->sort_id = -1;
		face->img = img;
		face->rc = rc;
		face->feature = new float[m_pFaceDescriptor->GetFeatureLength()];
		m_pFaceDescriptor->ExtractFeature(img.width, img.height, (byte*)gray.data, x0,y0, x1, y1, face->feature);
		m_pVecFaces->push_back(face);

	}
}

void FaceClustering::setFacePtr(vector<recognize::FacePtr> *pFaces){
	m_pVecFaces = pFaces;
}

void FaceClustering::endScan(){

	_cluster(m_dDistT);
}

void FaceClustering::release(){
 delete this;

}

void FaceClustering::_cluster(double dist){

	double distanceT = dist;

// to do: use recognition results
	vector<recognize::FacePtr> notaggedfaces;
	for (unsigned i=0; i<m_pVecFaces->size();i++)
	{
		recognize::FacePtr& face = (*m_pVecFaces)[i];
		if(0==face->tags[0])
			notaggedfaces.push_back(face);
	}

	int n = notaggedfaces.size();

	int i,j;

	if (n < 2) return;

	/* Set up the ragged array */
	double** distmatrix;
	distmatrix = (double**)malloc(n*sizeof(double*));
	if(distmatrix==NULL) return; /* Not enough memory available */
	distmatrix[0] = NULL;
	/* The zeroth row has zero columns. We allocate it anyway for convenience.*/
	for (i = 1; i < n; i++)
	{ distmatrix[i] = (double*)malloc(i*sizeof(double));
	if (distmatrix[i]==NULL) break; /* Not enough memory available */
	}
	if (i < n) /* break condition encountered */
	{ j = i;
	for (i = 1; i < j; i++) free(distmatrix[i]);
	return;
	}

	for(int i=1; i<n;i++)
		for (int j = 0; j < i; j++)
		{
			recognize::FacePtr face1 = notaggedfaces[i];
			recognize::FacePtr face2 = notaggedfaces[j];
			float simi = m_pFaceDescriptor->GetSimilarity(face1->feature, face2->feature, 0, 0 ); //to do: test other metric
			distmatrix[i][j] = 1.0f - simi;
			LOGOUT<<i<<" "<<j<<": "<<simi<<endl;
		}
//*
     Node* tree = treecluster(n, 0, 0, 0, 0, 0, 0,'m', distmatrix );
	 if(NULL == tree) return;

	 int* clusterid = new int[n];
	 cuttreed(n, tree, distanceT, clusterid);


	 for(int i=0; i<n;i++)
		 LOGOUT<<clusterid[i]<<" ";
	LOGOUT<<endl;
		
	 int ncluster = *(max_element(clusterid, clusterid+n));
	 LOGOUT<<"cluster number: "<<ncluster<<endl;
	 vector< vector<int> > clusters(ncluster+1);
	 for(int i=0; i<n; i++){
		clusters[clusterid[i]].push_back(i);
	 }

	 for(int i=0; i<n; i++){
		 if(clusters[clusterid[i]].size() >1)
			notaggedfaces[i]->sort_id = clusterid[i];
		// else
		//	notaggedfaces[i]->sort_id = -1;
		 LOGOUT<<notaggedfaces[i]->sort_id<<",";
		 LOGOUT<<endl;
	 }

	 for (i = 1; i < n; i++) free(distmatrix[i]);
	 delete[] clusterid;//*/
}

int FaceClustering::getFeatureLength(){

	return m_pFaceDescriptor->GetFeatureLength();
}


bool FaceClustering::cluster(vector<float*> featureBuffer, vector<int>& clusterNum, double dist){

	double distanceT = dist;

	int n = featureBuffer.size();//直接利用feature的信息
	int i,j;

	if (n < 2) return false;

	/* Set up the ragged array */
	double** distmatrix;
	distmatrix = (double**)malloc(n*sizeof(double*));
	if(distmatrix==NULL) return false; /* Not enough memory available */
	distmatrix[0] = NULL;
	/* The zeroth row has zero columns. We allocate it anyway for convenience.*/
	for (i = 1; i < n; i++)
	{ distmatrix[i] = (double*)malloc(i*sizeof(double));
	if (distmatrix[i]==NULL) break; /* Not enough memory available */
	}
	if (i < n) /* break condition encountered */
	{ j = i;
	for (i = 1; i < j; i++) free(distmatrix[i]);
	return false;
	}

	for(int i=1; i<n;i++)
		for (int j = 0; j < i; j++)
		{
			float simi = m_pFaceDescriptor->GetSimilarity(featureBuffer[i], featureBuffer[j], 0, 0 ); //to do: test other metric
			distmatrix[i][j] = 1.0f - simi;
		}
		//*
		Node* tree = treecluster(n, 0, 0, 0, 0, 0, 0,'m', distmatrix );
		if(NULL == tree) return false;

		int* clusterid = new int[n];
		cuttreed(n, tree, distanceT, clusterid);

		int ncluster = *(max_element(clusterid, clusterid+n));
		vector< vector<int> > clusters(ncluster+1);
		for(int i=0; i<n; i++){
			clusters[clusterid[i]].push_back(i);
		}

		vector<int> clusterLabel;
		for (int i = 0; i < clusters.size(); i++){
			clusterLabel.push_back(-1);
		}

		int currentNum = 1;
		for(int i=0; i<n; i++){
			if(clusters[clusterid[i]].size() >1){
				if (clusterLabel[clusterid[i]] == -1){
					clusterLabel[clusterid[i]] = currentNum;
					currentNum++;
				}
				clusterNum[i] = clusterLabel[clusterid[i]];
			}else
				clusterNum[i] = 0;
		}

		for (i = 1; i < n; i++) free(distmatrix[i]);
		delete[] clusterid;//*/

		return true;
}

