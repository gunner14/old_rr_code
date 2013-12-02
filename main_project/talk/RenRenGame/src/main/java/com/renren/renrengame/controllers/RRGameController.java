package com.renren.renrengame.controllers;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import javax.servlet.http.Cookie;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.renrengame.services.PlayerAccessRecord;
import com.renren.renrengame.services.RRGameContainer;
import com.renren.renrengame.utils.MD5Generator;
import com.renren.renrengame.utils.RRGameConstants;
import com.renren.renrengame.utils.applist.GameItemInfo;
import com.renren.renrengame.utils.hot.PlatformGameItemInfo;
import com.renren.renrengame.utils.rank.RankItemInfo;
import com.renren.renrengame.utils.spread.SpreadItemInfo;
import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.platform.component.application.model.NewProductListView;
import com.xiaonei.platform.core.model.User;

@Path("")
public class RRGameController {	
	protected RRGameContainer rrGameContainer;
	@Autowired
	protected HostHolder holder;
	private final Logger log = Logger.getLogger(RRGameController.class.getName());

	public RRGameContainer getRRGameService() {
		return rrGameContainer;
	}

	@Autowired
	public void setRRGameService(RRGameContainer rrGameContainer) {
		this.rrGameContainer = rrGameContainer;
	}

	
	
	@Get("window")
	@Post("window")
	public String getWindow(@Param("time") String source, @Param("date") String md5){
		/*if (!MD5Generator.checkMD5(source, RRGameConstants.KEY, md5)){
			return  "@Error,Message:Md5 is not correct";
		} */
		User user = holder.getUser();
		if (user != null) {
			int id = user.getId();
			PlayerAccessRecord par = PlayerAccessRecord.getInstance();
			if (par.viseted(id)) {
				log.info(id + "------------------------------------------->visited");
				return "@";
			}
		}
		log.info("------------------------------------------->unvisited");
		return "@" + rrGameContainer.getWindowXML();		
	}
	
	@Get("gamelist")
	@Post("gamelist")
	public String getGameList(@Param("time") String source,
			@Param("date") String md5) {
		if (!MD5Generator.checkMD5(source, RRGameConstants.KEY, md5))
			return "@Error,Message:Md5 is not correct";

		return "@" + rrGameContainer.getGameXML();
	}

	@Get("players")
	@Post("players")
	public String players(@Param("time") String source,
			@Param("date") String md5, @Param("game") String game,
			@Param("server") String server, @Param("number") String number) {
		if (!MD5Generator.checkMD5(source, RRGameConstants.KEY, md5))
			return "@Error,Message:Md5 is not correct";

		if (holder.getUser() == null)
			return "@Error,Message:Authentication failed";

		return "@" + rrGameContainer.getPlayerXML(game, server, number);
	}

	@Get("apply")
	@Post("apply")
	public String apply(@Param("time") String source,
			@Param("date") String md5, @Param("applicant") int applicant,
			@Param("accepter") int[] accepters, @Param("game") String game) {
		if (!MD5Generator.checkMD5(source, RRGameConstants.KEY, md5))
			return "@Error,Message:Md5 is not correct";

		if (holder.getUser() == null)
			return "@Error,Message:Authentication failed";

		return "@" + rrGameContainer.buddyApply(applicant, accepters, game);
	}

	@Get("applytestdodo")
	@Post("applytestdodo")
	public String apply(@Param("applicant") int applicant,
			@Param("accepter") int[] accepters, @Param("game") String game) {

		if (holder.getUser() == null)
			return "@Error,Message:Authentication failed";

		return "@" + rrGameContainer.buddyApplyTest(applicant, accepters, game);
	}
	
	@Get("check")
	@Post("check")
	public String check(@Param("time") String source,
			@Param("date") String md5, @Param("code") String code,
			@Param("type") String type, @Param("uid") String uid) {
		if (!MD5Generator.checkMD5(source, RRGameConstants.KEY, md5))
			return "@Error,Message:Md5 is not correct";

		if (holder.getUser() == null)
			return "@Error,Message:Authentication failed";

		return "@" + rrGameContainer.validCode(code, uid);
	}

	@Get("config")
	@Post("config")
	public String check(@Param("time") String source, @Param("date") String md5) {
		if (!MD5Generator.checkMD5(source, RRGameConstants.KEY, md5))
			return "@Error,Message:Md5 is not correct";

		if (holder.getUser() == null)
			return "@Error,Message:Authentication failed";

		return "@" + rrGameContainer.getConfig();
	}
	
	@Get("geticon")
	@Post("geticon")
	public String geticon(@Param("time") String source, @Param("date") String md5,
			@Param("appid") int appid) {
		if (!MD5Generator.checkMD5(source, RRGameConstants.KEY, md5))
			return "@Error,Message:Md5 is not correct";

		if (holder.getUser() == null)
			return "@Error,Message:Authentication failed";

		return "@" + rrGameContainer.getIcon(appid);
	}	
	
	/*@Get("hot")
	@Post("hot")
	public String hot(Invocation inv) {
		try {
			ArrayList<PlatformGameItemInfo> platformRecommendedGamesList = rrGameContainer.getPlatformRecommendedGamesList();
			ArrayList<GameItemInfo> maybeLikeGamesList = rrGameContainer.getMaybelikeGamesList();

//			log.debug("0---> maybeLikeGamesList.size:"+maybeLikeGamesList.size());
//			log.debug("0---> platformRecommendedGamesList:"+platformRecommendedGamesList.size());
			if (platformRecommendedGamesList != null)
				inv.addModel("gamelist", platformRecommendedGamesList);
			if (maybeLikeGamesList != null) {
				Collections.shuffle(maybeLikeGamesList);
				inv.addModel("maybelike0", maybeLikeGamesList.get(0));
				inv.addModel("maybelike1", maybeLikeGamesList.get(1));		
			}
		} catch (Exception e) {
			e.printStackTrace();
		}

		return "game.jsp";
	}*/
	
	@Get("hot")
	@Post("hot")
	public String hot(Invocation inv) {
		List<RankItemInfo> gameRankList = rrGameContainer.getHotGameList();
		List<SpreadItemInfo> gameSpreadList = rrGameContainer.getSpreadGameList();
		inv.addModel("gameRankList", gameRankList);
		inv.addModel("gameSpreadList", gameSpreadList);
		return "game.jsp";
	}
	
	/*public String hot(){
		return "game.htm";
	}*/
	
	
	@Get("recentapp")
	@Post("recentapp")
	public String recentApp(Invocation inv, @Param("time") String source, @Param("date") String md5) {
//		if (!MD5Generator.checkMD5(source, RRGameConstants.KEY, md5))
//			return "@Error,Message:Md5 is not correct";

		if (holder.getUser() == null)
			return "@Error,Message:Authentication failed";		
		Cookie[] cookies = inv.getRequest().getCookies();
		int uid = getUIDFromCookie(cookies);
		if (uid == 0)
			return "@Error,Message:Lack of uid in cookie";
		return "@" + rrGameContainer.getRecentApp(uid);
	}
	
	private int getUIDFromCookie(Cookie []cookies) {
		int uid = 0;
		if (cookies != null) {
			for (int i=0; i<cookies.length; ++i) {
				if (cookies[i].getName().equalsIgnoreCase("id")) {
					uid = Integer.parseInt(cookies[i].getValue());
				}
			}
		}
		return uid;
	}
}
