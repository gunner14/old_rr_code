package com.renren.xce.socialgraph.strategy;

/*
 * IncreaseInnerGroup
 * IncreaseInterGroup
 * ProbabilityInnerGroup: uniform, long tail
 * ProbabilityInterGroup: uniform, long tail 
 */

import java.util.ArrayList;
import java.util.List;

import xce.socialgraph.util.Clogging;

import com.renren.xce.socialgraph.common.Category;
import com.renren.xce.socialgraph.common.Strategy;
import com.renren.xce.socialgraph.updator.CreateRecommendDataThread;
import com.renren.xce.socialgraph.updator.OfflineUpdator;

public class RcdFoFStrategy implements Strategy {

	public List<Category> getStrategy(int userId) {
		List<Category> categoryList = new ArrayList<Category>();		
		CommonCategory fof = new CommonCategory(CreateRecommendDataThread.RFOF, OfflineUpdator.kFoFMaxRcdNumber, 10);
		categoryList.add(fof);
		return categoryList;
	}
}
