#ifndef PREDICTOR_H
#define PREDICTOR_H

#include "../Feature.h"
#include "../Model.h"
#include "../Cache.h"
#include "OceSlice/slice/RecommendationManager.h"
#include "OceCxxAdapter/src/SocialGraphUserPhotoStatsTripodAdapter.h"
class Predictor{
	public:
		Predictor(Logistic* logistic);
		~Predictor();
		static bool rank(int i_hid, RecommendItemSeq& item_list);
		void run();
	public:
		Logistic* _logistic;
};
#endif

