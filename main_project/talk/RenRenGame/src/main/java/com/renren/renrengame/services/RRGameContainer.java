package com.renren.renrengame.services;

import java.util.ArrayList;
import java.util.Date;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Set;
import java.util.Vector;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReadWriteLock;
import java.util.concurrent.locks.ReentrantLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;

import javax.servlet.http.Cookie;



import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.renrengame.utils.RRGameConstants;
import com.renren.renrengame.utils.Security;
import com.renren.renrengame.utils.applist.GameItemInfo;
import com.renren.renrengame.utils.applist.GameListInfo;
import com.renren.renrengame.utils.applist.GameListReader;
import com.renren.renrengame.utils.applist.GameXMLGenerator;
import com.renren.renrengame.utils.applist.LocalRecommendedGames;
import com.renren.renrengame.utils.applist.RecentGames;
import com.renren.renrengame.utils.buddy.BuddyHandler;
import com.renren.renrengame.utils.buddy.PlatformSocialGameBuddyReader;
import com.renren.renrengame.utils.buddy.PlayerListInfo;
import com.renren.renrengame.utils.buddy.PlayerListReader;
import com.renren.renrengame.utils.buddy.PlayerXMLGenerator;
import com.renren.renrengame.utils.config.ConfigFileReader;
import com.renren.renrengame.utils.config.ConfigInfo;
import com.renren.renrengame.utils.config.ConfigXMLGenerator;
import com.renren.renrengame.utils.hot.PlatformGameItemInfo;
import com.renren.renrengame.utils.hot.PlatformRecommendedGames;
import com.renren.renrengame.utils.rank.RankInfo;
import com.renren.renrengame.utils.rank.RankItemInfo;
import com.renren.renrengame.utils.spread.SpreadInfo;
import com.renren.renrengame.utils.spread.SpreadItemInfo;
import com.renren.renrengame.utils.window.WindowXMLReader;
import com.xiaonei.icode.verify.CodeUtil;
import com.xiaonei.platform.core.menu.UserMenu;
import com.xiaonei.platform.core.menu.UserMenuManager;
import com.xiaonei.platform.component.application.cache.AppCacheMgr;
import com.xiaonei.platform.component.application.cache.AppConnectCacheMgr;
import com.xiaonei.platform.component.application.jade.home.AppTopListHome;
import com.xiaonei.platform.component.application.jade.home.ProductListHome;
import com.xiaonei.platform.component.application.model.AppTopList;
import com.xiaonei.platform.component.application.model.NewProductListView;

public class RRGameContainer {
	private final Logger log = Logger.getLogger(RRGameContainer.class.getName());

	protected ScheduledExecutorService executor;

	public GameListReader getGlReader() {
		return glReader;
	}

	public void setGlReader(GameListReader glReader) {
		this.glReader = glReader;
	}

	public GameXMLGenerator getGameXMLGen() {
		return gameXMLGen;
	}

	public void setGameXMLGen(GameXMLGenerator gameXMLGen) {
		this.gameXMLGen = gameXMLGen;
	}
	public PlayerXMLGenerator getPlayerXMLGen() {
		return playerXMLGen;
	}

	public void setPlayerXMLGen(PlayerXMLGenerator playerXMLGen) {
		this.playerXMLGen = playerXMLGen;
	}

	public BuddyHandler getBuddyHandler() {
		return buddyHandler;
	}

	public void setBuddyHandler(BuddyHandler buddyHandler) {
		this.buddyHandler = buddyHandler;
	}

	public Security getSec() {
		return sec;
	}

	public void setSec(Security sec) {
		this.sec = sec;
	}

	public ConfigFileReader getCfReader() {
		return cfReader;
	}

	public void setCfReader(ConfigFileReader cfReader) {
		this.cfReader = cfReader;
	}

	public ConfigXMLGenerator getConfigXMLGen() {
		return configXMLGen;
	}

	public void setConfigXMLGen(ConfigXMLGenerator configXMLGen) {
		this.configXMLGen = configXMLGen;
	}

	public LocalRecommendedGames getRecommendedGames() {
		return recommendedGames;
	}

	public void setRecommendedGames(LocalRecommendedGames recommendedGames) {
		this.recommendedGames = recommendedGames;
	}

	public PlatformRecommendedGames getPlatformRecommendedGames() {
		return platformRecommendedGames;
	}

	public void setPlatformRecommendedGames(
			PlatformRecommendedGames platformRecommendedGames) {
		this.platformRecommendedGames = platformRecommendedGames;
	}
	public PlatformSocialGameBuddyReader getPlatformPlayerListReader() {
		return platformPlayerListReader;
	}

	public void setPlatformPlayerListReader(
			PlatformSocialGameBuddyReader platformPlayerListReader) {
		this.platformPlayerListReader = platformPlayerListReader;
	}
	public PlayerListReader getLocalPlayerListReader() {
		return localPlayerListReader;
	}

	public void setLocalPlayerListReader(PlayerListReader localPlayerListReader) {
		this.localPlayerListReader = localPlayerListReader;
	}

	public RecentGames getRecentGames() {
		return recentGames;
	}
	public void setRecentGames(RecentGames recentGames) {
		this.recentGames = recentGames;
	}
	
	public WindowXMLReader getWindowXmlReader() {
		return windowXmlReader;
	}
	public void setWindowXmlReader(WindowXMLReader windowXmlReader) {
		this.windowXmlReader = windowXmlReader;
	}
	
	public RankInfo getRankInfo() {
		return rankInfo;
	}

	public void setRankInfo(RankInfo rankInfo) {
		this.rankInfo = rankInfo;
	}

	public SpreadInfo getSpreadInfo() {
		return spreadInfo;
	}

	public void setSpreadInfo(SpreadInfo spreadInfo) {
		this.spreadInfo = spreadInfo;
	}
	
	@Autowired
	protected GameListReader glReader;
	@Autowired
	protected LocalRecommendedGames recommendedGames;
	
	@Autowired
	protected WindowXMLReader windowXmlReader;
	
	@Autowired
	protected GameXMLGenerator gameXMLGen;
	@Autowired
	protected PlayerListReader localPlayerListReader;
	@Autowired
	protected PlatformSocialGameBuddyReader platformPlayerListReader;
	@Autowired
	protected PlayerXMLGenerator playerXMLGen;	
	@Autowired
	protected ConfigFileReader cfReader;
	@Autowired
	protected ConfigXMLGenerator configXMLGen;
	@Autowired
	protected BuddyHandler buddyHandler;
	@Autowired
	protected PlatformRecommendedGames platformRecommendedGames;
	@Autowired
	protected Security sec;
	@Autowired
	protected RecentGames recentGames;
	@Autowired
	protected RankInfo rankInfo;
	@Autowired
	protected SpreadInfo spreadInfo;
	@Autowired
	protected ProductListHome productListHome;

	private String windowXML = "";
	private String gameXML = "";
	private PlayerListInfo localPlayerList = null;
	private PlayerListInfo platformPlayerList = null;
	private GameListInfo webGameList = null;
	private GameListInfo socialGameList = null;
	private String configXML = "";
	private ArrayList<PlatformGameItemInfo> platformRecommendedGamesList = null;
	private ArrayList<GameItemInfo> maybelikeGamesList = null;
	private List<RankItemInfo> gameRankList = null;
	private List<SpreadItemInfo> gameSpreadList = null;
	private Lock applyDateLock = new ReentrantLock();
	private Lock applicantLock = new ReentrantLock();
	private Lock accepterLock = new ReentrantLock();
//	private ReadWriteLock applyDateLock = new ReentrantReadWriteLock();
//	private ReadWriteLock applicantLock = new ReentrantReadWriteLock();
	private Map<String,Set<Integer>> accepterCount = new ConcurrentHashMap();	//限制用户每天接受一次申请
	private Map<Integer, Integer> applyCount = new ConcurrentHashMap();	//限制用户每天申请加好友的次数
	private Map<Integer, Date> applyDate = new ConcurrentHashMap();		//限制用户申请加好友的时间间隔
	private int count = 12 * 24;
	private int socialPlayerUpdateCount = 3;

	public RRGameContainer() {
		log.debug("RRGameContainer.RRGameContainer  ---->start to run..");
		executor = Executors.newSingleThreadScheduledExecutor();
		executor.scheduleAtFixedRate(
				new Runnable() {
					@Override
					public void run() {

						log.debug("RRGameContainer.RRGameContainer  ---->start timer..");

						if (count == 12 * 24) {
							UpdateConfigInfo();
							UpdateGameInfo();
							UpdateLocalPlayerInfo();
							ClearApplicantAndAccepter();
                            updateWindowInfo();
                            updateHotGame();
							updateSpreadGame();
							PlayerAccessRecord.getInstance().clear();
							count = 0;
						} 
						if (socialPlayerUpdateCount == 3) {
							UpdatePlatformPlayerInfo();
							socialPlayerUpdateCount = 0;
						}
						///test
//						else if (count == 2) {
//							accepterCount.clear();
//							count = 0;
//						}
						//test
						
						ClearApplyDateMap();
						count++;
						socialPlayerUpdateCount++;
						log.debug("RRGameContainer.RRGameContainer  ---->count."+count);		
					}
				}, RRGameConstants.READFILETIMER_DELAY,
				RRGameConstants.READFILETIMER_FERQ, TimeUnit.SECONDS);	
	}

	private void updateHotGame() {
		log.debug("RRGameContainer.updateHotGame  ---->start to update:");
		List<NewProductListView> hostGameList = ProductListHome.getInstance().getMostPopProductlist4GameExclude(10);	
		gameRankList = rankInfo.getRankList(hostGameList);
	}
	
	private void updateSpreadGame() {
		log.debug("RRGameContainer.updateSpreadGame  ---->start to update:");
		gameSpreadList = spreadInfo.getSpreadList();
	}

	public List<SpreadItemInfo> getSpreadGameList() {
		return gameSpreadList;
	}
	

	public List<RankItemInfo> getHotGameList() {
		return gameRankList;
	}

	private void ClearApplicantAndAccepter() {
		//可以改为每天new出新的存储区
		
		log.info("ClearApplicantAndAccepter----- accepterCount.size:"+accepterCount.size());
		accepterLock.lock();
 		accepterCount.clear();
		accepterLock.unlock();
	
		log.info("ClearApplicantAndAccepter----- applyCount.size:"+applyCount.size());
		applicantLock.lock();
		applyCount.clear(); // 每天清空一次 请求列表	
		applicantLock.unlock();
	}
	
	private void ClearApplyDateMap() {
		Date   curDate   =   new   Date(System.currentTimeMillis());//获取当前时间
		applyDateLock.lock();
		Iterator iter = applyDate.entrySet().iterator();
		while (iter.hasNext()) {
		    Map.Entry entry = (Map.Entry) iter.next();
		    Integer key = (Integer) entry.getKey();
		    Date val = (Date) entry.getValue();
		    
		    Date temp = new Date(val.getTime());
		    temp.setMinutes(temp.getMinutes()+5);
		    if (curDate.compareTo(temp) > 0) {
		    	iter.remove();
//		    	log.debug("ClearApplyDateMap     remove:"+key.intValue()+"    value:"+val);
		    }
		} 
		applyDateLock.unlock();
	}
	
	
	private void UpdateGameInfo() {
		//webgame 列表(左侧)
		webGameList = glReader.getGameItems(RRGameConstants.WEBGAME_TITLE);
		log.debug("RRGameContainer.UpdateGameInfo  ----> webGamelist:"
				+ webGameList.gameList.size());
		
		//socialgame 列表(左侧)
		socialGameList = glReader.getGameItems(RRGameConstants.SOCIALGAME_TITLE);
		log.debug("RRGameContainer.UpdateGameInfo  ----> socialGameList:"
				+ socialGameList.gameList.size());
		
		//本地提供的 可能喜欢的游戏 (左侧)
		ArrayList<GameItemInfo> localRecommendedGamesList = recommendedGames
				.getRecommendedGames();

		gameXML = gameXMLGen.gameXML(localRecommendedGamesList,
				socialGameList.gameList, socialGameList.gameTypeList,
				webGameList.gameList, webGameList.gameTypeList);
		
		//本地提供的 可能喜欢的网页游戏 (右侧)
		maybelikeGamesList = recommendedGames.getMaybeLikeGames();

		//平台提供的 可能喜欢的社区游戏(右侧)
		platformRecommendedGamesList = platformRecommendedGames.getRecommendedGames();
		
		log.debug("RRGameContainer.UpdateGameInfo  ----> webGamelist:"
				+ webGameList.gameList.size() + "  socialgameList:"
				+ socialGameList.gameList.size() + "   recommendedGanelist:"
				+ localRecommendedGamesList.size()
				+ "   PlatformRecommendedGamesList:"
				+ platformRecommendedGamesList.size()
				+ "  maybelikeGamesList:"
				+ maybelikeGamesList.size());
		log.debug("RRGameContainer.UpdateGameInfo  ---->gameXML:"
				+ gameXML.length());
	}
	
	protected void updateWindowInfo() {
		log.debug("RRGameContainer.updateWindowInfo  ---->start to update:");
		windowXML = windowXmlReader.getWindowXml();
		
	}
	
	private void UpdatePlatformPlayerInfo() {
		log.debug("RRGameContainer.UpdatePlatformPlayerInfo  ---->start to update:");

		platformPlayerList = platformPlayerListReader.getPlayerIDList();
	}

	private void UpdateLocalPlayerInfo() {
		log.debug("RRGameContainer.UpdatelocalPlayerInfo  ---->start to update:");

		localPlayerList = localPlayerListReader.getPlayerIDList();		
	}

	private void UpdateConfigInfo() {
		ArrayList<ConfigInfo> ciList = cfReader.getConfigInfo();
		configXML = configXMLGen.configXML(ciList);
	}

	public ArrayList<PlatformGameItemInfo> getPlatformRecommendedGamesList() {
		return platformRecommendedGamesList;
	}

	public ArrayList<GameItemInfo> getMaybelikeGamesList() {
		return maybelikeGamesList;
	}

	public String getGameXML() {
		return gameXML;
	}

	public String getConfig() {
		return configXML;
	}

	public String getWindowXML() {
		return windowXML;
	}
	
	public String getPlayerXML(String gameTitle, String server, String number) {
		ArrayList<String> playerIDList = null;
		playerIDList = playerXMLGen.getPlayerXML(platformPlayerList, gameTitle, server, number);
		if (playerIDList.size() == 0)
			playerIDList = playerXMLGen.getPlayerXML(localPlayerList, gameTitle, server, number);
		else
			log.debug("getPlayerXML from platform   playerIDList.size:"+playerIDList.size());
		// log.info("RRGameContainer.getPlayerXML: gameTitle:"+gameTitle+"   server:"+server+"    number:"+number
		// );
		return playerXMLGen.getItemXML(playerXMLGen.getPlayerInfoList(playerIDList),gameTitle, server);
	}

	public boolean varifyTicket(String ticket, String userid) {
		return sec.varifyTicket(ticket, userid);
	}

	public String validCode(String code, String uid) {
		boolean ret = CodeUtil.getInstance().validCode(code,
				RRGameConstants.ICODE, uid);
		log.info("RRGameContainer.validCode: code:" + code + "   type:"
				+ RRGameConstants.ICODE + "    uid:" + uid + "    ret:" + ret);
		if (!ret)
			return "Error,Message:The verification Code you provided is not correct";

		return "Success";
	}

	private ArrayList<Integer> validAccepter(int[] accepters, String game) {
		ArrayList<Integer> validAccepters = new ArrayList<Integer>();
		accepterLock.lock();
		Set<Integer> gameAccepters = accepterCount.get(game);
		if (gameAccepters == null) {
			Set<Integer> tempAccepters = new HashSet<Integer>();
			for (int i=0; i<accepters.length; ++i) {
				tempAccepters.add(accepters[i]);
				validAccepters.add(accepters[i]);
			}
			accepterCount.put(game, tempAccepters);		
//			log.debug("validAccepter  addNew game:"+game+"   validAccepter:"+validAccepters);
		} else {
			for (int i=0; i<accepters.length; ++i) {
				if (!gameAccepters.contains(accepters[i])) {
					validAccepters.add(accepters[i]);
					gameAccepters.add(accepters[i]);
//					log.debug("validAccepter  addNew game:"+game+"   validAccepter:"+validAccepters);
				}							
			}
		}	
		accepterLock.unlock();
		return validAccepters;
	}
	
	private String getGameTitle(String game) {
		String ret = "";
		if (webGameList==null || socialGameList==null || 
				webGameList.gameList==null || socialGameList.gameList==null) {
			log.debug("getGameTitle ,,   webGameList:"+webGameList+"    socialGameList:"+socialGameList);
			return ret;
		}
//		log.debug("getGameTitle  game:"+game+"   webGamelist.gameList:"+webGameList.gameList.size()+
//				"   socialGameList.gameList.size:"+socialGameList.gameList.size()	);
		for (int i=0; i<webGameList.gameList.size(); ++i) {
			if (webGameList.gameList.get(i).cavaspageurl!=null && 
					webGameList.gameList.get(i).cavaspageurl.equalsIgnoreCase(game)) {
				ret = webGameList.gameList.get(i).getName();
				break;
			}
		}
		if (ret.equalsIgnoreCase("")) {
			for (int j=0; j<socialGameList.gameList.size(); ++j) {
				if (socialGameList.gameList.get(j).cavaspageurl.equalsIgnoreCase(game)) {
					ret = socialGameList.gameList.get(j).getName();
					break;
				}
			}
		}
		if (ret.length() == 0)
			log.error("getGameTitle game:"+game+"  not fount.");

		return ret;
	}
	
	public String buddyApply(int applicant, int[] accepters, String game) {
		if (accepters.length > RRGameConstants.ACCEPTER_MAX) {
			log.debug("applicant:"+applicant+"    buddyApply set the number of accepters more than "+RRGameConstants.ACCEPTER_MAX);
			return "false,Can't set the number of accepters more than "
					+ RRGameConstants.ACCEPTER_MAX;
		}
		String retMessage = "false";
		if (valid5MinApply(applicant)) {
			ArrayList<Integer> validAccepters = validAccepter(accepters, game);
//			log.debug("buddyApply  accepters:"+accepters.length+"    validAccepters:"+validAccepters.size());
			int count = valid1DayApply(applicant, validAccepters.size());
			for (int i = 0; i <count&&i<validAccepters.size(); ++i) {
				// buddyHandler.apply(applicant, accepters[i], game);
				String gameTitle = getGameTitle(game);
				int ret = buddyHandler.applyBuddy(applicant, validAccepters.get(i).intValue(), gameTitle);
				// if (ret == -1)
				// return "false,"+buddyHandler.getMessage();
//				if (ret > -1)
//					retMessage = "false";
			}
			if (count > 0)
				retMessage  = "true";			
			if (retMessage.equals("false"))
				log.debug("applicant:"+applicant+"    buddyApply   fail to valid1DayApply");
		} else 
			log.debug("applicant:"+applicant+"    buddyApply   fail to valid5MinApply");
		return retMessage;
	}

	public String getIcon(int appid) {		
		return AppConnectCacheMgr.getInstance().get(appid).getConnect_iphone_logo();
	}
	
	public String buddyApplyTest(int applicant, int[] accepters, String game) {
		log.debug("buddyApplyTest   applicant:"+applicant+"    accepters:"+accepters+"  game:"+game
				+"  accepters.length:"+accepters.length);
		for (int i=0; i<accepters.length; i++)
			log.debug("buddyApplyTest   accepters:"+accepters[i]);
		if (accepters==null || game==null)
			return "empty values";

		return buddyApply(applicant, accepters, game);
	}
	
	public String getRecentApp(int uid) {
		if (webGameList==null || webGameList.gameList==null) 
			return "";		
		ArrayList<GameItemInfo> gameItemInfoList = recentGames.getRecentGame(uid, webGameList.gameList);
		log.debug("getRecentApp ---> uid:"+uid+"   gameItemInfoList.size:"+gameItemInfoList.size());

		return gameXMLGen.recentGameXML(uid, gameItemInfoList);
	}
		
	//可以去掉锁，实际使用中造成的影响比较小
	private int valid1DayApply(int applicant, int count) {
		int ret = 0;
		int putSize =0 ;
		
		applicantLock.lock();
		Object o = applyCount.get(applicant);
		if (o == null) {
			if (RRGameConstants.APPLYCOUNT < count)
				count = RRGameConstants.APPLYCOUNT;
			ret = putSize = count;
		} else {
			int temp = (Integer) o;
			if (temp + count <= RRGameConstants.APPLYCOUNT) {
				putSize = count + temp;
				ret = count;
			} else {
				ret = RRGameConstants.APPLYCOUNT - temp;
				putSize = ret +temp;
			}
		}
		applyCount.put(applicant, putSize);
		applicantLock.unlock();
		return ret;
	}
	
	private boolean valid5MinApply(int applicant) {
		Date   curDate   =   new   Date(System.currentTimeMillis());//获取当前时间
		boolean isFound = false;
		boolean ret = false;
		applyDateLock.lock();
		isFound = applyDate.containsKey(applicant);
		if (!isFound) {			
			applyDate.put(applicant, curDate);
			log.debug("valid5MinApply  new add:  applicant_"+applicant+"   curDate:"+curDate);
			ret = true;
		} else {
			Date val = new Date(applyDate.get(applicant).getTime());
			val.setMinutes(val.getMinutes()+5);
			if (curDate.compareTo(val) > 0) {
				ret = true;
				applyDate.put(applicant, curDate);
				log.debug("valid5MinApply  replace add:  applicant_"+applicant+"   curDate:"+curDate);
			} else {
				ret = false;
				log.debug("valid5MinApply  fail to add ");
			}				
		}
		applyDateLock.unlock();
		return ret;
	}
}
