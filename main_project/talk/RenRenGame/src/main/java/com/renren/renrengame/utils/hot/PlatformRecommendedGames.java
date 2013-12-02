package com.renren.renrengame.utils.hot;

import java.util.ArrayList;
import java.util.List;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.platform.component.application.model.AppBase;
import com.xiaonei.platform.component.application.model.NewProductListView;
import com.xiaonei.platform.core.model.AppMenuItem;
import com.xiaonei.platform.core.opt.base.AppMenuItemCacheMgr;

public class PlatformRecommendedGames {
	private final Logger log = Logger.getLogger(PlatformRecommendedGames.class
			.getName());
	@Autowired
	com.xiaonei.platform.component.application.jade.dao.ProductlistDAO productlistDAO;

	private ArrayList<PlatformGameItemInfo> getRecommendedGamesFromPlatform() {
		ArrayList<PlatformGameItemInfo> recommendedGamesList = new ArrayList<PlatformGameItemInfo>();
		final int type = 4;
		final int selected = 6;
		final int category = 10;
		final int offset = 0;
		final int count = 8;
		List<NewProductListView> plvList = productlistDAO.getHotGamesList(type,
				selected, category, offset, count);
		for (int i = 0; i < plvList.size(); ++i) {
			// recommendedGamesList.add(String.valueOf(plvList.get(i).getAppId())
			// );
			PlatformGameItemInfo gii = new PlatformGameItemInfo();
			gii.ID = plvList.get(i).getAppId();
			gii.name = plvList.get(i).getAppName();
			gii.type = plvList.get(i).getCate();
			log.debug("getRecommendedGamesFromPlatform   id:" + gii.ID
					+ "   name:" + gii.name);
			AppMenuItem appMenuItem = AppMenuItemCacheMgr.getInstance().get(
					gii.ID);
			if (appMenuItem == null)
				continue;
			gii.url = appMenuItem.getSideNavUrl();
			gii.url += gii.url.indexOf('?') == -1 ? "?" : "&";
			gii.url += "rrlobby_mid" + i;
			gii.url += "&" + (50226+i);

			try {
				AppBase appbase = com.xiaonei.platform.component.application.cache.AppBaseCacheMgr
						.getInstance().get(appMenuItem.getAppId());
				gii.logo = appbase.getLogo();
			} catch (Exception e) {
				log.warn("GameListReader.getGameItemsLogo ----> Exception: Fail to get logo75.");
				e.printStackTrace();
				continue;
			}
			recommendedGamesList.add(gii);
		}

		return recommendedGamesList;
	}

	//平台提供的 可能喜欢的游戏
	public ArrayList<PlatformGameItemInfo> getRecommendedGames() {
		log.debug("RecommendedGames.getRecommendedGamesFromFile ----> start to run");
		return getRecommendedGamesFromPlatform();
	}
}
