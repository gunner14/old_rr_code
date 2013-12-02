package com.renren.renrengame.utils.spread;

import java.util.ArrayList;
import java.util.List;

import org.apache.log4j.Logger;

import com.renren.renrengame.services.RRGameContainer;
import com.xiaonei.platform.component.application.jade.home.ProductListHome;
import com.xiaonei.platform.component.application.model.NewProductListView;

public class SpreadInfo {

	private SpreadItemInfo getInfo(NewProductListView view) {
		SpreadItemInfo item = new SpreadItemInfo(); 
		item.appLogo = view.getAppBase().getLogo();
		item.appIcon = view.getAppIcon();
		item.appLink = view.getSideNavUrl();
		item.appName = view.getAppName();
		item.cate = view.getCate();
		item.userNum = getStringFromNum((String) view.getItemInfo().get("total_users"));
		return item;
	}
	
	private String getStringFromNum(String str) {
		try{
		    int num = Integer.parseInt(str);
		    if (num >= 100000000) {
		    	return num / 100000000 + "亿";
		    }
		    if (num >= 10000) {
		    	return num / 10000 + "万";
		    }
		    return str;
		} catch (Exception e) {
			return str;
		}
	}
	
	public List<SpreadItemInfo> getSpreadList() {
		SpreadFileAnalysis sfa = new SpreadFileAnalysis();
		List<Integer> arr = sfa.get9From20();
		List<NewProductListView> list = ProductListHome.getInstance().gets(arr);
		List<SpreadItemInfo> rankList = new ArrayList<SpreadItemInfo>();
		for (NewProductListView view : list) {
			SpreadItemInfo item = getInfo(view);
			rankList.add(item);
		}
		return rankList;
	}
}
