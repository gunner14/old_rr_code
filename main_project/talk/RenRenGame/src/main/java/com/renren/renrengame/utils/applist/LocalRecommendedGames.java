package com.renren.renrengame.utils.applist;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.InputStream;
import java.net.URL;
import java.util.ArrayList;
import org.apache.log4j.Logger;

import com.renren.renrengame.utils.RRGameConstants;
import com.xiaonei.platform.component.application.model.AppBase;
import com.xiaonei.platform.core.model.AppMenuItem;
import com.xiaonei.platform.core.opt.base.AppMenuItemCacheMgr;

public class LocalRecommendedGames {
	private final Logger log = Logger.getLogger(LocalRecommendedGames.class
			.getName());

	private ArrayList<GameItemInfo> getRecommendedGamesFromList(
			ArrayList<String> gameIDList) {
		ArrayList<GameItemInfo> gamesList = new ArrayList<GameItemInfo>();

		for (int i = 0; i < gameIDList.size(); i++) {
			GameItemInfo gli = new GameItemInfo();
			AppMenuItem appMenuItem = AppMenuItemCacheMgr.getInstance().get(
					Integer.parseInt(gameIDList.get(i).trim()));
			if (appMenuItem == null)
				continue;

			gli.ID = gameIDList.get(i);
			gli.name = appMenuItem.getAppName();
			gli.logo = appMenuItem.getAppIcon();
			gli.url = appMenuItem.getSideNavUrl();
			gli.url += (gli.url.indexOf('?') == -1 ? "?" : "&")+(50221+i);
			gli.subtype = gli.type = RRGameConstants.RECOMMENDEDGAMES_TITLE;

			try {
				AppBase appbase = com.xiaonei.platform.component.application.cache.AppBaseCacheMgr
						.getInstance().get(appMenuItem.getAppId());

				gli.logo75 = appbase.getLogo();
			} catch (Exception e) {
				log.warn("RecommendedGames.getRecommendedGamesFromList ----> Exception: Fail to get logo75.");
				e.printStackTrace();
				continue;
			}

			gamesList.add(gli);
			log.debug("RecommendedGames.getRecommendedGamesFromList: id:"
					+ gli.ID + "    type:" + gli.type + "  subtype:"
					+ gli.subtype + "   name:" + gli.name + "  logo:"
					+ gli.logo + "    logo75:" + gli.logo75 + "  url:"
					+ gli.url);
		}

		return gamesList;
	}

	
	private ArrayList<GameItemInfo> getMaybeLikeGamesFromList(
			ArrayList<String> gameInfoList) {
		log.debug("RecommendedGames.getMaybeLikeGamesFromList.gameInfoList.size:"+gameInfoList.size());
		ArrayList<GameItemInfo> gamesList = new ArrayList<GameItemInfo>();
		for (int i = 0; i < gameInfoList.size();) {
			GameItemInfo gli = new GameItemInfo();
			for (int j = 0; j < 5; ++j) {
				String data = gameInfoList.get(i+j);
				int pos = data.indexOf(':');
				if (pos == -1)
					break;
				String key = data.substring(0, pos).trim();				
				String value = data.substring(pos+1).trim();
				
				log.debug("RecommendedGames.getMaybeLikeGamesFromList key:"+key+"   value:"+value);
				
				if (key.equalsIgnoreCase("id"))
					gli.ID = value;
				else if (key.equalsIgnoreCase("title"))
					gli.name = value;
				else if (key.equalsIgnoreCase("url")) {
					gli.url = value;
					gli.url += gli.url.indexOf('?')==-1 ? "?" : "&";
					gli.url += "rrlobby_end"+i/5;
					gli.url += "&"+(50234+i);
					
					log.debug("RecommendedGames.getMaybeLikeGamesFromList.gamesList.url :"+gli.url);
				}
				else if (key.equalsIgnoreCase("img")) {
					gli.logo = "http://gadget.talk.renren.com/images/" + value;
				}
				else if (key.equalsIgnoreCase("description"))
					gli.description = value;				
			}
			i += 5;
			if (gli.ID.length() > 0)
				gamesList.add(gli);
			if (gli.ID.length() > 0)
				log.debug("RecommendedGames.getMaybeLikeGamesFromList.gamesList ++:");
			else
				log.debug("RecommendedGames.getMaybeLikeGamesFromList.gamesList :ID_0");
				
		}
		return gamesList;
	}

	private ArrayList<String> getGamesInfoFromFile(String filePath) {
		long t1 = System.currentTimeMillis();
		log.debug("RecommendedGames.getRecommendedGamesFromFile ----> start to run");
		ArrayList<String> v = new ArrayList<String>();

		URL resourceURL = this.getClass().getClassLoader()
				.getResource(filePath);
		if (resourceURL == null) {
			log.error("RecommendedGames.getRecommendedGamesFromFile ---->Cannot find gamelist.");
			return v;
		}
		String recommendedGamesPath = resourceURL.getPath();
		log.debug("RecommendedGames.getRecommendedGamesFromFile ----> gameListPath:"
				+ recommendedGamesPath);

		try {
			InputStream inStream = new FileInputStream(recommendedGamesPath); // 读入原文件
			int byteread = 0;
			int headpos = 0;
			int rearpos = 0;
			byte[] buffer = new byte[RRGameConstants.BUFFER_SIZE];
			int offset = 0;
			while ((byteread = inStream.read(buffer, offset,
					RRGameConstants.BUFFER_SIZE - offset)) != -1) {
				// log.debug("ConfigFileReader.getPlayerInfoFromFile ----> byteread"+byteread);

				headpos = 0;
				rearpos = offset;
				while (rearpos < byteread + offset) {
					if (buffer[rearpos] == '\n') {
						// log.debug("PlayerListReader.getPlayerInfoFromFile ----> push_back"+
						// new String(buffer,headpos, rearpos-headpos));
						v.add(new String(buffer, headpos, rearpos - headpos));
						headpos = rearpos + 1;
					}

					rearpos++;
				}
				if (rearpos == byteread + offset) {
					// log.debug("PlayerListReader.getPlayerInfoFromFile ----> can't found!! p:"+buffer[rearpos-1]);

					int tempPos = 0;
					while (headpos < byteread + offset)
						buffer[tempPos++] = buffer[headpos++];
					offset = tempPos;
				}
			}

			inStream.close();

			log.debug("RecommendedGames.getRecommendedGamesFromFile ----> v.size"
					+ v.size());

		} catch (Exception e) {
			log.warn("RecommendedGames.getRecommendedGamesFromFile---> Get data from file..");
			e.printStackTrace();
		}

		long t2 = System.currentTimeMillis();
		log.info("RecommendedGames.getRecommendedGamesFromFile---> time cost: "
				+ (t2 - t1));

		return v;
	}
	

	public ArrayList<GameItemInfo> getRecommendedGames() {
		log.debug("RecommendedGames.getRecommendedGamesFromFile ----> start to run");
		return getRecommendedGamesFromList(getGamesInfoFromFile(RRGameConstants.RECOMMENDEDGAMES_FILENAME));
	}
	
	public ArrayList<GameItemInfo> getMaybeLikeGames() {
		ArrayList<GameItemInfo> templist = getMaybeLikeGamesFromList(getGamesInfoFromFile(RRGameConstants.MAYBELIKE_FILENAME));
		log.debug("RecommendedGames.getMaybeLikeGames.szie :"+templist.size());
		return templist;	
	}


}
