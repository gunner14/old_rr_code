package com.renren.renrengame.utils.buddy;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.log4j.Logger;

import com.renren.renrengame.utils.RRGameConstants;
import com.xiaonei.platform.component.application.model.AppBasic;
import com.xiaonei.platform.component.application.model.NewProductListView;
import com.xiaonei.platform.component.application.utils.AppIdCacheManager;

public class PlatformSocialGameBuddyReader {
	private final Logger log = Logger
			.getLogger(PlatformSocialGameBuddyReader.class.getName());
	private Map<Integer, Integer> playerDBCursor = new HashMap();

	private ArrayList<Integer> getSocialGameInfoFromPlatform() {
		ArrayList<Integer> v = new ArrayList<Integer>();
		List<NewProductListView> socialGameInfoList = AppIdCacheManager
				.getInstance().getGameAppList();
		log.debug("getSocialGameInfoFromPlatform.size:"
				+ socialGameInfoList.size());
		for (int i = 0; i < socialGameInfoList.size(); ++i) {
			v.add(socialGameInfoList.get(i).getAppId());
		}
		return v;
	}

	private PlayerListInfo getPlayerInfoFromList(
			ArrayList<Integer> socialGameIDList) {
		long t1 = System.currentTimeMillis();

		PlayerListInfo pli = new PlayerListInfo();
		pli.gameTitleList = new ArrayList<String>();
		pli.playerList = new ArrayList<ArrayList<String>>();

		for (int i = 0; i < socialGameIDList.size(); i++) {
			AppBasic appbasic = com.xiaonei.platform.component.application.cache.AppBasicCacheMgr
					.getInstance().get(socialGameIDList.get(i).intValue());
			pli.gameTitleList.add(appbasic.getCanvasPageUrl());
			
			Object startDBCursor = playerDBCursor.get(socialGameIDList.get(i)); 
			int startPos = startDBCursor==null ? 0 : ((Integer)startDBCursor).intValue();
			
			List<Integer> UserIdList = com.xiaonei.platform.component.application.jade.home.AppActiveUserLogAdapter
					.getInstance().getAppActiveUserLog(
							socialGameIDList.get(i).intValue(), startPos, RRGameConstants.PLAYER_UPDATE_COUNT);
			boolean reload = false;
			if (UserIdList.size()<RRGameConstants.PLAYER_UPDATE_COUNT && startPos>0)
				reload = true;
			if (UserIdList.size() == RRGameConstants.PLAYER_UPDATE_COUNT)
				startPos += RRGameConstants.PLAYER_UPDATE_COUNT;
			else 
				startPos = 0;
			ArrayList<String> userId = new ArrayList<String>();
			for (int j = 0; j < UserIdList.size(); j++) {
				userId.add(String.valueOf(UserIdList.get(j).intValue()));
			}
			if (reload) {
				List<Integer> UserIdListTemp = com.xiaonei.platform.component.application.jade.home.AppActiveUserLogAdapter
				.getInstance().getAppActiveUserLog(
						socialGameIDList.get(i).intValue(), startPos, RRGameConstants.PLAYER_UPDATE_COUNT-UserIdList.size());
				for (int j = 0; j < UserIdListTemp.size(); j++) {
					userId.add(String.valueOf(UserIdListTemp.get(j).intValue()));
				}
				startPos += UserIdListTemp.size();
			}
			
			log.info("getPlayerInfoFromList -->  IdSize:"+UserIdList.size()+"  title:"+pli.gameTitleList.get(i)+
					"   socialGameId:"+socialGameIDList.get(i).intValue()+"   startPos:"+startPos);
			
			playerDBCursor.put(socialGameIDList.get(i), startPos);

			pli.playerList.add(userId);						
		}
		log.debug("------->>PlayerListInfo getPlayerInfoFromList:  pli.playerList:"+pli.playerList.size()
				+"   pli.gameTitleList:"+pli.gameTitleList.size());
		return pli;
	}

	public PlayerListInfo getPlayerIDList() {
		return getPlayerInfoFromList(getSocialGameInfoFromPlatform());
	}
}
