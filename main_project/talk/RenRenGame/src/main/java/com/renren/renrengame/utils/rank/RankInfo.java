package com.renren.renrengame.utils.rank;

import java.util.ArrayList;
import java.util.List;

import com.xiaonei.platform.component.application.jade.home.AppTopListHome;
import com.xiaonei.platform.component.application.model.AppTopList;
import com.xiaonei.platform.component.application.model.NewProductListView;

public class RankInfo {

	private RankItemInfo getInfo(NewProductListView view) {
		RankItemInfo item = new RankItemInfo(); 
		item.appLogo = view.getAppBase().getLogo();
		item.appIcon = view.getAppIcon();
		item.appLink = view.getSideNavUrl();
		item.appName = view.getAppName();
		item.rating = Math.round(view.getRating() * 10) / 10.0;
		item.cate = view.getCate();
		AppTopList appTop = AppTopListHome.getInstance().get(view.getAppId());
        if (appTop == null) {
             view.setMostAvtiveUpDown(9999);
        } else {
             view.setMostAvtiveUpDown(appTop.getRankFloat());
        }
        item.upOrDown = view.getMostAvtiveUpDown();
        item.comment_account = (String) view.getItemInfo().get("comment_account");
		return item;
	}
	
	public List<RankItemInfo> getRankList(List<NewProductListView> list) {
		List<RankItemInfo> rankList = new ArrayList<RankItemInfo>();
		for (NewProductListView view : list) {
			RankItemInfo item = getInfo(view);
			rankList.add(item);
		}
		return rankList;
	}
}
