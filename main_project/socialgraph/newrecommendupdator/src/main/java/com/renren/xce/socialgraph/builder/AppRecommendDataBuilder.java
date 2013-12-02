package com.renren.xce.socialgraph.builder;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import xce.socialgraph.recommend.app.ApplicationData;
import xce.socialgraph.recommend.app.ApplicationRecommendUtil;

import com.renren.xce.socialgraph.common.DataBuilder;
import com.renren.xce.socialgraph.common.DataItem;
import com.renren.xce.socialgraph.updator.CreateRecommendDataThread;
import com.xiaonei.platform.component.application.cache.AppBaseCacheMgr;
import com.xiaonei.platform.component.application.home.UserAppHome;
import com.xiaonei.platform.component.application.model.AppBase;
import com.xiaonei.platform.core.model.AppMenuItem;
import com.xiaonei.platform.core.opt.base.AppMenuItemCacheMgr;
import com.xiaonei.xce.socialgraph.proto.GeneratedItems.Item;

/**
 * building data of applications recommend
 * @author zhangnan
 * @email zhangnan@renren-inc.com
 */
public class AppRecommendDataBuilder implements DataBuilder {
	private static int CACHE_SHARES_SIZE = 2;		//show 2 common friends
	
	public List<DataItem> getData(int hostId) {
		List<DataItem> resultList = new ArrayList<DataItem>();
		List<ApplicationData> appRecommendList = ApplicationRecommendUtil.getInstance().getAppRecommendList(hostId);
		Collections.shuffle(appRecommendList);		//random result
		
		for (ApplicationData app : appRecommendList) {
			if (UserAppHome.getInstance().isAppUser(app.getAppId(),hostId)) {		//filter user's app list
				continue;
			}
			
			AppMenuItem appObj = AppMenuItemCacheMgr.getInstance().get(app.getAppId());
			AppBase appBase = AppBaseCacheMgr.getInstance().get(app.getAppId());
			
			Item.Builder item = Item.newBuilder();
			item.setId(app.getAppId());
			item.setField(app.getSharesCount());
			item.setMessage(CreateRecommendDataThread.RAPP);
			item.addMessages(appObj.getAppName());			//stored with app name in position 0
			item.addMessages(appObj.getSideNavUrl());		//stored with app url in position 1
			item.addMessages(appBase.getLogo48());		//stored with app header in position 2
			
			for (Integer shareId : app.getShareList()) {
				if (item.getFieldsCount() >= CACHE_SHARES_SIZE) {
					break;
				}
				item.addFields(shareId);
			}
		
			DefaultDataItem appItem = new DefaultDataItem((long)app.getAppId(), item);
			resultList.add(appItem);
		}
		
		return resultList;
	}

	public String getBuilderType() {
		return CreateRecommendDataThread.RAPP;
	}

}
