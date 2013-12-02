package com.renren.xce.socialgraph.strategy;


import java.util.ArrayList;
import java.util.List;

import com.renren.xce.socialgraph.common.Category;
import com.renren.xce.socialgraph.common.Strategy;
import com.renren.xce.socialgraph.updator.CreateFFUCRDataThread;
import com.renren.xce.socialgraph.updator.CreateRecommendDataThread;
import com.renren.xce.socialgraph.updator.FFUCRUpdator;

public class RcdFFUCRStrategy implements Strategy {

	public List<Category> getStrategy(int userId) {
		List<Category> categoryList = new ArrayList<Category>();		
		CommonCategory ffucr = new CommonCategory(CreateFFUCRDataThread.FREINDS_CLUSTER, FFUCRUpdator.kFoFMaxRcdNumber, 10);
		categoryList.add(ffucr);
		return categoryList;
	}
}
