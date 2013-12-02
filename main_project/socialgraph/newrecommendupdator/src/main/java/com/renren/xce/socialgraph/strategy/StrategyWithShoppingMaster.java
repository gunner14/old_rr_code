package com.renren.xce.socialgraph.strategy;

import java.util.ArrayList;
import java.util.List;

import xce.socialgraph.util.Clogging;

import com.renren.xce.socialgraph.common.Category;
import com.renren.xce.socialgraph.common.Strategy;
import com.renren.xce.socialgraph.updator.CreateRecommendDataThread;

/**
 * Get Stategy without Application recommendation
 * @author yangxiaoyun
 * @email xiaoyun.yang@renren-inc.com
 */
public class StrategyWithShoppingMaster implements Strategy {
	public List<Category> getStrategy(int userId) {
		List<Category> categoryList = new ArrayList<Category>();
		
		Clogging.Debug("UserApplyStrategy use StrategyWithShoppingMaster strategy userId:" + userId);
		
	
		CommonCategory shopMasters = new CommonCategory(CreateRecommendDataThread.RSHOPMASTER, 1, 10);
                categoryList.add(shopMasters);		

		CommonCategory commonFreinds = new CommonCategory(CreateRecommendDataThread.COMMON_FRIENDS, 45, 10);
		categoryList.add(commonFreinds);
	
		CommonCategory page = new CommonCategory(CreateRecommendDataThread.PAGE, 3, 10);
		categoryList.add(page);
	
                CommonCategory pageCollege = new CommonCategory(CreateRecommendDataThread.PAGECOLLEGE, 1, 10);
                categoryList.add(pageCollege);

	
	
	
		CommonCategory app = new CommonCategory(CreateRecommendDataThread.RAPP, 2, 10);
		categoryList.add(app);
	
		CommonCategory forum = new CommonCategory(CreateRecommendDataThread.RFORUM, 2, 10);
		categoryList.add(forum);
	
		CommonCategory desk = new CommonCategory(CreateRecommendDataThread.RDESK, 1, 10);
		categoryList.add(desk);
		
		return categoryList;
	}
}
