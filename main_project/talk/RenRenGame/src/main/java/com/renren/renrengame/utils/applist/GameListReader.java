package com.renren.renrengame.utils.applist;

import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.apache.commons.httpclient.HttpClient;
import org.apache.commons.httpclient.HttpMethod;
import org.apache.commons.httpclient.HttpStatus;
import org.apache.commons.httpclient.URIException;
import org.apache.commons.httpclient.methods.GetMethod;
import org.apache.commons.httpclient.util.URIUtil;
import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;
import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.DocumentHelper;
import org.dom4j.Element;

import com.renren.renrengame.utils.RRGameConstants;
import com.xiaonei.platform.component.application.model.AppBase;
import com.xiaonei.platform.component.application.model.AppBasic;
import com.xiaonei.platform.component.application.model.NewProductListView;
import com.xiaonei.platform.component.application.utils.AppIdCacheManager;
import com.xiaonei.platform.core.model.AppMenuItem;
import com.xiaonei.platform.core.opt.base.AppMenuItemCacheMgr;

public class GameListReader {

	private final Logger log = Logger.getLogger(GameListReader.class.getName());

	private GameListInfo getGameItemsLogo(GameListInfo gli) {
		for (int i = 0; i < gli.gameList.size(); i++) {
			AppMenuItem appMenuItem = AppMenuItemCacheMgr.getInstance().get(
					Integer.parseInt(gli.gameList.get(i).ID));
			if (appMenuItem == null)
				continue;

			gli.gameList.get(i).name = appMenuItem.getAppName();
			gli.gameList.get(i).logo = appMenuItem.getAppIcon();
			gli.gameList.get(i).url = appMenuItem.getSideNavUrl();
			gli.gameList.get(i).url  += (gli.gameList.get(i).url .indexOf('?') == -1 ? "?" : "&");
			if (gli.gameList.get(i).type.equals(RRGameConstants.SOCIALGAME_TITLE))
				gli.gameList.get(i).url  += "50239";
			else if (gli.gameList.get(i).type.equals(RRGameConstants.WEBGAME_TITLE))
				gli.gameList.get(i).url  += "50240";

			try {
				AppBase appbase = com.xiaonei.platform.component.application.cache.AppBaseCacheMgr
						.getInstance().get(appMenuItem.getAppId());
				if (appbase == null)
					continue;
				gli.gameList.get(i).logo75 = appbase.getLogo();

				AppBasic appbasic = com.xiaonei.platform.component.application.cache.AppBasicCacheMgr
						.getInstance().get(appMenuItem.getAppId());
				if (appbasic == null)
					continue;
				gli.gameList.get(i).cavaspageurl = appbasic.getCanvasPageUrl();
			} catch (Exception e) {
				log.warn("GameListReader.getGameItemsLogo ----> Exception: Fail to get logo75.");
				e.printStackTrace();
				continue;
			}

			log.debug("GameListReader.getGameItemsLogo: id:"
					+ gli.gameList.get(i).ID + "    type:"
					+ gli.gameList.get(i).type + "  subtype:"
					+ gli.gameList.get(i).subtype + "   name:"
					+ gli.gameList.get(i).name + "  logo:"
					+ gli.gameList.get(i).logo + "    logo75:"
					+ gli.gameList.get(i).logo75 + "  url:"
					+ gli.gameList.get(i).url + "   cavaspageurl:"
					+ gli.gameList.get(i).cavaspageurl);
		}

		return gli;
	}

	private GameListInfo getGameItemsTypeFromList(
			ArrayList<String> gameInfoList, String gameType) {
		GameListInfo gii = null;
		if (gameType.equals(RRGameConstants.SOCIALGAME_TITLE))
			// gii = getSocialGameItemsTypeFromList(gameInfoList);
			gii = getSocialGameItemsTypeFromList(gameInfoList);
		else if (gameType.equals(RRGameConstants.WEBGAME_TITLE))
			gii = getWebGameItemsTypeFromList(gameInfoList);

		return gii;
	}

	private GameListInfo getWebGameItemsTypeFromList(
			ArrayList<String> gameInfoList) {
		log.debug("GameListReader.getWebGameItemsTypeFromList ----> start");

		GameListInfo gii = new GameListInfo();
		gii.gameList = new ArrayList<GameItemInfo>();
		gii.gameTypeList = new ArrayList<String>();

		log.debug("GameListReader.getWebGameItemsTypeFromList ----> gameInfoList:"
				+ gameInfoList.size());
		String subtype = "";
		for (int i = 0; i < gameInfoList.size(); i++) {
			String item = gameInfoList.get(i);
			if (item.substring(0, 2).equals("##")) {
				subtype = item.substring(2).trim();
				gii.gameTypeList.add(subtype);
			} else {
				if (subtype.equals(""))
					continue;

				GameItemInfo giInfo = new GameItemInfo();

				int tempPos = item.indexOf(' ', 0);
				int spacePos = tempPos > 0 ? tempPos : item.indexOf('\t', 0);
				if (spacePos == -1)
					continue;

				giInfo.ID = item.substring(spacePos + 1).trim();
				giInfo.subtype = subtype;
				giInfo.type = RRGameConstants.WEBGAME_TITLE;

				gii.gameList.add(giInfo);
			}

		}
		log.debug("GameListReader.getWebGameItemsTypeFromList ----> gii.gameList.size():"
				+ gii.gameList.size());
		return gii;
	}

	private GameListInfo getSocialGameItemsTypeFromList(
			ArrayList<String> gameInfoList) {
		log.debug("--->getSocialGameItemsTypeFromList  string.size:"
				+ gameInfoList.size());

		GameListInfo gii = new GameListInfo();
		gii.gameList = new ArrayList<GameItemInfo>();
		gii.gameTypeList = new ArrayList<String>();

		for (int i = 0; i < gameInfoList.size(); i++) {
			String item = gameInfoList.get(i);
			int begin = item.indexOf('|', 0);
			if (begin == -1)
				continue;
			int middle = item.indexOf('|', begin + 1);
			if (middle == -1)
				continue;
			int end = item.indexOf('|', middle + 1);
			if (end == -1)
				continue;

			GameItemInfo giInfo = new GameItemInfo();
			giInfo.ID = item.substring(begin + 1, middle).trim();
			giInfo.type = RRGameConstants.SOCIALGAME_TITLE;
			giInfo.subtype = item.substring(middle + 1, end).trim();

			gii.gameList.add(giInfo);

			int j;
			for (j = 0; j < gii.gameTypeList.size(); j++) {
				if (gii.gameTypeList.get(j).equals(giInfo.subtype))
					break;
			}
			if (j == gii.gameTypeList.size())
				gii.gameTypeList.add(giInfo.subtype);

		}
		log.debug("--->getSocialGameItemsTypeFromList  gameList.size:"
				+ gii.gameList.size());
		log.debug("--->getSocialGameItemsTypeFromList  gameTypeList.size:"
				+ gii.gameTypeList.size());

		return gii;
	}

	private ArrayList<String> getGameInfoFromFile(String gameType) {
		log.debug("GameListReader.getGameInfoFromFile ----> start to run");
		ArrayList<String> v = new ArrayList<String>();

		String gameListName = "";
		if (gameType.equals(RRGameConstants.SOCIALGAME_TITLE))
			gameListName = RRGameConstants.SOCIALGAME_FILENAME;
		else if (gameType.equals(RRGameConstants.WEBGAME_TITLE))
			gameListName = RRGameConstants.WEBGAME_FILENAME;

		if (gameListName.equals(""))
			return v;

		URL resourceURL = this.getClass().getClassLoader()
				.getResource(gameListName);
		if (resourceURL == null) {
			log.error("GameListReader.getGameInfoFromFile ---->Cannot find gamelist.");
			return v;
		}
		String gameListPath = resourceURL.getPath();
		log.debug("GameListReader.getGameInfoFromFile ----> gameListPath"
				+ gameListPath);

		try {
			InputStream inStream = new FileInputStream(gameListPath); // 读入原文件
			int byteread = 0;
			int headpos = 0;
			int rearpos = 0;
			byte[] buffer = new byte[RRGameConstants.BUFFER_SIZE];
			int offset = 0;
			while ((byteread = inStream.read(buffer, offset,
					RRGameConstants.BUFFER_SIZE - offset)) != -1) {
				headpos = 0;
				rearpos = offset;
				while (rearpos < byteread + offset) {
					if (buffer[rearpos] == '\n') {
						v.add(new String(buffer, headpos, rearpos - headpos));
						headpos = rearpos + 1;
					}

					rearpos++;
				}
				if (rearpos == byteread + offset) {

					int tempPos = 0;
					while (headpos < byteread + offset)
						buffer[tempPos++] = buffer[headpos++];
					offset = tempPos;
				}
			}
			inStream.close();
		} catch (Exception e) {
			log.warn("GameListReader.getGameInfoFromFile---> Get data from file..");
			e.printStackTrace();
		}

		return v;
	}

	private String GetWebGameInfo(String url, String queryString) {
		String response = null;
		HttpClient client = new HttpClient();
		HttpMethod method = new GetMethod(url);
		try {
			if (StringUtils.isNotBlank(queryString))
				method.setQueryString(URIUtil.encodeQuery(queryString));
			client.executeMethod(method);
			if (method.getStatusCode() == HttpStatus.SC_OK) {
				response = method.getResponseBodyAsString();
			}
		} catch (URIException e) {
			log.warn("GetWebGameInfo:    执行HTTP Get请求时，编码查询字符串“" + queryString
					+ "发生异常！" + e);
		} catch (IOException e) {
			log.warn("GetWebGameInfo:   执行HTTP Get请求" + url + "时，发生异常！" + e);
		} finally {
			method.releaseConnection();
		}
		return response;
	}

	private GameListInfo getWebGameFromXML(String webGameXML) {
		log.debug("GameListReader.getWebGameFromXML ---start ");
		GameListInfo gli = new GameListInfo();
		gli.gameTypeList = new ArrayList<String>();
		gli.gameList = new ArrayList<GameItemInfo>();

		Document doc = null;
		try {
			doc = DocumentHelper.parseText(webGameXML);
		} catch (DocumentException e) {
			e.printStackTrace();
			return gli;
		}
		Element root = doc.getRootElement();
		for (Iterator i = root.elementIterator("item"); i.hasNext();) {
			Element item = (Element) i.next();
			String type = item.attributeValue("type").trim();
			log.debug("GameListReader.getWebGameFromXML ---   type:" + type);
			if (type.length() > 0)
				gli.gameTypeList.add(type);
			for (Iterator j = item.elementIterator("id"); j.hasNext();) {
				Element id = (Element) j.next();
				String idstr = id.getTextTrim();
				if (Integer.parseInt(idstr) <= 0)
					continue;
				GameItemInfo giInfo = new GameItemInfo();
				giInfo.ID = idstr;
				giInfo.subtype = type;
				giInfo.type = RRGameConstants.WEBGAME_TITLE;
				log.debug("GameListReader.getWebGameFromXML ---   type:" + type
						+ "   id:" + idstr);
				gli.gameList.add(giInfo);
			}
		}

		log.debug("GameListReader.getWebGameFromXML  gli.gameTypeList.size:"
				+ gli.gameTypeList.size() + "   gli.gameList.size:"
				+ gli.gameList.size());
		return gli;
	}

	private GameListInfo getWebGameInfoFromWG() {
		String webGameInfo = GetWebGameInfo(RRGameConstants.WEBGAME_URL, "");
		log.info("GameListReader.getWebGameInfoFromWG   webGameXML:"
				+ webGameInfo);
		return getWebGameFromXML(webGameInfo);
	}

	private ArrayList<String> getSocialGamesFromPlatform() {
		ArrayList<String> v = new ArrayList<String>();
		List<NewProductListView> socialGameInfoList = AppIdCacheManager
				.getInstance().getGameAppList();
		log.debug("getSocialGameInfoFromPlatform.size:"
				+ socialGameInfoList.size());
		for (int i = 0; i < socialGameInfoList.size(); ++i) {
			String gameInfo = "| " + socialGameInfoList.get(i).getAppId()
					+ " | " + socialGameInfoList.get(i).getCate() + "|";
			v.add(gameInfo);
			log.debug("getSocialGameInfoFromPlatform.data:" + gameInfo);
		}
		if (v.size() == 0) {
			log.warn("getSocialGameInfoFromPlatform.data:     平台没有数据，从文件读取!");
			return getGameInfoFromFile(RRGameConstants.SOCIALGAME_TITLE);
		}
		return v;
	}

	////
	public GameListInfo getGameItems(String gameType) {
		log.warn("GameListReader.getGameItems---> start to run");
		GameListInfo gli = null;
		if (gameType.equals(RRGameConstants.SOCIALGAME_TITLE))
			// gameInfoList = getSocialGameInfoFromPlatform();
			gli = getGameItemsTypeFromList(getSocialGamesFromPlatform(),
					RRGameConstants.SOCIALGAME_TITLE);
		else if (gameType.equals(RRGameConstants.WEBGAME_TITLE))
			// gameInfoList = getGameInfoFromFile(gameType);
			gli = getWebGameInfoFromWG();

		return getGameItemsLogo(gli);
	}
	////
}
