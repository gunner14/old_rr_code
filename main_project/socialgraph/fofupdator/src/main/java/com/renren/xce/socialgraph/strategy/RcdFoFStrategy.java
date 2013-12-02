package com.renren.xce.socialgraph.strategy;


import java.util.ArrayList;
import java.util.List;

import com.renren.xce.socialgraph.common.Category;
import com.renren.xce.socialgraph.common.Strategy;
import com.renren.xce.socialgraph.updator.CreateRecommendDataThread;
import com.renren.xce.socialgraph.updator.FoFUpdator;

public class RcdFoFStrategy implements Strategy {

	public List<Category> getStrategy(int userId) {
		List<Category> categoryList = new ArrayList<Category>();		
		CommonCategory fof = new CommonCategory(CreateRecommendDataThread.RFOF, FoFUpdator.kFoFMaxRcdNumber, 10);
		categoryList.add(fof);
		return categoryList;
	}
}
