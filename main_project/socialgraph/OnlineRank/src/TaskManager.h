#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H
#include "model.h"
#include "Feature.h"
#include "Access.h"

class Tasks{
	public:
				Tasks(int nthread=1);
		virtual		~Tasks();
		void		genfeat(Access* acc, FeatureMap* featMap, const char* save_prefix);
		void		eval(Access* acc, FeatureMap* featMap, Logistic* pLogisitc);
		void		fofeval(Access* acc);
	private:
		void		_assign(int x);
		int		_nthread;
		vector<FeatureThread*>	_vecThreads;
};

#endif
