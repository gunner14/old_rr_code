package com.renren.sns.guide.service;

import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Random;

import mop.hi.oce.domain.network.UniversityInfo;

import org.springframework.beans.factory.annotation.Autowired;

import com.dodoyo.datum.cache.DatumInfoMgr;
import com.renren.forum.service.ForumMemberService;
import com.renren.sns.guide.utils.ThreadPool;
import com.renren.sns.minisite.xoa.client.TagService;
import com.renren.sns.minisite.xoa.model.SiteInfo;
import com.renren.xoa.lite.ServiceFactories;
import com.renren.xoa.lite.ServiceFactory;
import com.renren.xoa.lite.ServiceFuture;
import com.renren.xoa.lite.ServiceFutureHelper;
import com.xiaonei.highschool.domain.Page;
import com.xiaonei.highschool.service.FansService;
import com.xiaonei.highschool.service.PageService;
import com.xiaonei.login.logic.MemCacheFactory;
import com.xiaonei.page.xoa.adapter.PageClientAdapter;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.common.RunnableTaskExecutor;
import com.xiaonei.reg.guide.pojo.GuideZ3Pojo;
import com.xiaonei.reg.guide.pojo.PagePojo;
import com.xiaonei.reg.guide.pojo.RecommendContent;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.usertrace.dao.GuideRecommendDAO;
import com.xiaonei.reg.usertrace.dao.GuideRecommendJadeDAO;
import com.xiaonei.xce.feed.FeedLogicAdapter;

public class GuideRecommendLogic {
	
	@Autowired
	private ForumMemberService forumMemberService;


	@Autowired
	private GuideRecommendJadeDAO guideRecommendJadeDAO;
	@Autowired
	private com.renren.sns.tag.service.ZhanService zhanService;

	public com.renren.sns.tag.service.ZhanService getZhanService() {
		return zhanService;
	}

	public void setZhanService(
			com.renren.sns.tag.service.ZhanService zhanService) {
		this.zhanService = zhanService;
	}

	public GuideRecommendJadeDAO getGuideRecommendJadeDAO() {
		return guideRecommendJadeDAO;
	}

	public void setGuideRecommendJadeDAO(
			GuideRecommendJadeDAO guideRecommendJadeDAO) {
		this.guideRecommendJadeDAO = guideRecommendJadeDAO;
	}

	// private static GuideRecommendLogic _instance = new GuideRecommendLogic();
	private List<RecommendContent> guideRecommendContentList; // 所有资源
	private Map<String, List<RecommendContent>> sortGuideRecommendMap = new HashMap<String, List<RecommendContent>>();

	/*
	 * public static GuideRecommendLogic getInstance() { return _instance; }
	 */

	public Map<String, List<RecommendContent>> getSortGuideRecommendMap() {
		return sortGuideRecommendMap;
	}

	public void setSortGuideRecommendMap(
			Map<String, List<RecommendContent>> sortGuideRecommendMap) {
		this.sortGuideRecommendMap = sortGuideRecommendMap;
	}

	public List<RecommendContent> getGuideRecommendContentList() {
		return guideRecommendContentList;
	}

	public void setGuideRecommendContentList(
			List<RecommendContent> guideRecommendContentList) {
		this.guideRecommendContentList = guideRecommendContentList;
	}

	// 单个获取小站信息
	public SiteInfo getXiaoZhanPojoById(long siteId) {
		final ServiceFactory fact = ServiceFactories.getFactory();// 获取ServiceFactory实例
		final com.renren.sns.minisite.xoa.client.SiteService service = fact
				.getService(com.renren.sns.minisite.xoa.client.SiteService.class);// 通过ServiceFactory来获取具体了Service实例
		// 参数小站id
		ServiceFuture<SiteInfo> future = service.getSite(new Integer(siteId
				+ ""));
		SiteInfo ret = ServiceFutureHelper.execute(future, 5000);
		return ret;

	}

	// 批量获取小站信息
	public List<SiteInfo> getBatchXiaoZhanPojoByBatchId(List<Long> ids) {
		final ServiceFactory fact = ServiceFactories.getFactory();// 获取ServiceFactory实例
		final com.renren.sns.minisite.xoa.client.SiteService service = fact
				.getService(com.renren.sns.minisite.xoa.client.SiteService.class);// 通过ServiceFactory来获取具体了Service实例
		int size = ids.size();
		List<Integer> integerIds = new ArrayList<Integer>();
		for (int i = 0; i < size; i++) {
			integerIds.add(new Integer(ids.get(i) + ""));
		}
		ServiceFuture<SiteInfo[]> future = service.getSites(integerIds);
		SiteInfo[] ret = ServiceFutureHelper.execute(future, 5000);

		if (ret == null) {
			GuideLogger.printLog("getBatchXiaoZhanPojoByBatchId is null: "
					+ ids);
			return new ArrayList<SiteInfo>();
		}

		List<SiteInfo> xiaoZhaoPojoList = Arrays.asList(ret);

		return xiaoZhaoPojoList;

	}

	public boolean attentionXiaoZhao(int siteId, int userId) {
		final ServiceFactory fact = ServiceFactories.getFactory();// 获取ServiceFactory实例
		final com.renren.sns.minisite.xoa.client.SiteService service = fact
				.getService(com.renren.sns.minisite.xoa.client.SiteService.class);// 通过ServiceFactory来获取具体了Service实例
		// int siteId = 671008790;
		// int userId = 309372060;
		ServiceFuture<Boolean> future = service.follow(siteId, userId);
		Boolean ret = ServiceFutureHelper.execute(future, 5000); // true
																	// 成功，false失败
		return ret;

	}

	//加载初始化数据
	//@PostConstruct
	public void loadData() {
		try {
			GuideLogger
					.printLog("load data************************ guideRecommendJadeDAO:"
							+ guideRecommendJadeDAO);
			guideRecommendContentList = guideRecommendJadeDAO
					.selectGuideRecommendContent(1);
			if (guideRecommendContentList != null) {
				int size = guideRecommendContentList.size();
				RecommendContent recommendContent = null;
				List<RecommendContent> recommentList = null;
				String interest = "";
				for (int i = 0; i < size; i++) {
					recommendContent = guideRecommendContentList.get(i);
					interest = recommendContent.getInterest();
					recommentList = sortGuideRecommendMap.get(interest);
					if (recommentList == null) {
						recommentList = new ArrayList<RecommendContent>();
						recommentList.add(recommendContent);
						sortGuideRecommendMap.put(interest, recommentList);
					} else {
						recommentList.add(recommendContent);
					}
				}
			}

		} catch (SQLException e) {
			e.printStackTrace();
		}
	}
	
	//在推荐位上关注一个小站时，再推一上同类型的小站根据数据库中的interest来判断
	public List<RecommendContent> getSameRecommendContent(int userid, int num,
			int contentType, String from,String tag) {
		if (guideRecommendContentList == null) {
			loadData();
		}
		if (guideRecommendContentList == null) {
			return new ArrayList<RecommendContent>();
		}
		List<RecommendContent> resultList = new ArrayList<RecommendContent>();
		List<RecommendContent> recommentList=sortGuideRecommendMap.get(tag);
		if(recommentList==null || recommentList.size()<=0){
			return getBatchMinitSiteData(resultList, userid, from);
		}
		

		// 过滤掉已经关注的小站
		List<RecommendContent> removeAfterList = removeAlreadyFllowMinisite(
				recommentList, userid);
		if(removeAfterList.size()>0){
			resultList.add(removeAfterList.get(0));
		}
		else{
			removeAfterList = removeAlreadyFllowMinisite(
					guideRecommendContentList, userid);
			if(removeAfterList.size()>0){
				resultList.add(removeAfterList.get(0));
			}
			
		}
		return getBatchMinitSiteData(resultList, userid, from);
	}

	

	public List<RecommendContent> getRecommendContentList(int userid, int num,
			int contentType, String from) {
		if (guideRecommendContentList == null) {
			loadData();
		}
		if (guideRecommendContentList == null) {
			return new ArrayList<RecommendContent>();
		}

		// 过滤掉已经关注的小站
		List<RecommendContent> removeAfterList = removeAlreadyFllowMinisite(
				guideRecommendContentList, userid);

		/*List<Integer> userOperateRecommendContentIdList = null;// 用户操作过的内容
		try {
			userOperateRecommendContentIdList = guideRecommendJadeDAO
					.selectRecommendContentViewLog(userid);
		} catch (SQLException e1) {
			e1.printStackTrace();
		}*/

		List<RecommendContent> recommendContentList =removeAfterList;// 用户未操作过的内容
		/*int size = removeAfterList.size();
		if (userOperateRecommendContentIdList == null
				|| userOperateRecommendContentIdList.size() <= 0) {
			if (size >= num) {
				return getBatchMinitSiteData(removeAfterList.subList(0, num),
						userid, from);
			} else {
				GuideLogger.printLog("removeAfterList is less 4 size:" + size);
				return getBatchMinitSiteData(removeAfterList.subList(0, size),
						userid, from);

			}
		} else {
			RecommendContent recommendContent = null;
			for (int i = 0; i < size; i++) {
				recommendContent = removeAfterList.get(i);
				if (userOperateRecommendContentIdList.contains(recommendContent
						.getId()) == false) {
					recommendContentList.add(recommendContent);
				}

			}
			if (recommendContentList.size() < num) {
				recommendContentList.clear();
				try {
					userOperateRecommendContentIdList = guideRecommendJadeDAO
							.selectRecommendContentViewLogCon(userid);
				} catch (SQLException e) {
					e.printStackTrace();
				}
				for (int i = 0; i < size; i++) {
					recommendContent = (RecommendContent) removeAfterList
							.get(i);
					if (userOperateRecommendContentIdList
							.contains(recommendContent.getId()) == false) {
						recommendContentList.add(recommendContent);
					}
				}
			}

		}*/

		List<RecommendContent> resultList = new ArrayList<RecommendContent>();
		Random random = new Random();
		int newSize = recommendContentList.size();
		if (num == 1) {
			if (newSize > 0) {
				int randomNum = random.nextInt(newSize);
				resultList.add(recommendContentList.get(randomNum));
			}
		} else if (num == 4) {
			if (newSize > 10) {
				int randomNum = random.nextInt(newSize / 10);
				resultList.add(recommendContentList.get(randomNum));
				resultList.add(recommendContentList.get(randomNum + newSize
						/ 10));
				resultList.add(recommendContentList
						.get(randomNum + newSize / 5));
				int randomNum2 = random.nextInt(newSize - newSize / 10 * 3);
				resultList.add(recommendContentList.get(randomNum2 + newSize
						/ 10 * 3));
			} else {
				if (newSize > 4) {
					for (int i = 0; i < 4; i++) {
						resultList.add(recommendContentList.get(i));
					}
				} else {
					for (int i = 0; i < newSize; i++) {
						resultList.add(recommendContentList.get(i));
					}
				}

			}
		} else {
			GuideLogger.printLog("num is not correct num:" + num);
		}

		return getBatchMinitSiteData(resultList, userid, from);
	}

	// 去除用户关注过的小站
	private List<RecommendContent> removeAlreadyFllowMinisite(
			List<RecommendContent> minitsiteList, int userid) {

		final ServiceFactory fact = ServiceFactories.getFactory();// 获取ServiceFactory实例
		final com.renren.sns.minisite.xoa.client.SiteService service = fact
				.getService(com.renren.sns.minisite.xoa.client.SiteService.class);// 通过ServiceFactory来获取具体了Service实例
		// 参数用户id
		ServiceFuture<Integer[]> future = service.getFollowerSiteIds(userid);
		Integer[] ret = ServiceFutureHelper.execute(future, 500);

		if (ret == null) {
			GuideLogger.printLog("select already fllow list is null userid:"
					+ userid);
			return minitsiteList;
		}
		List<Integer> minisiteIdList = Arrays.asList(ret);

		List<RecommendContent> removeAfterList = new ArrayList<RecommendContent>();
		int size = minitsiteList.size();
		for (int i = 0; i < size; i++) {
			if (!minisiteIdList.contains(new Integer(minitsiteList.get(i)
					.getContent_business_id() + ""))) {
				removeAfterList.add(minitsiteList.get(i));
			}
		}
		return removeAfterList;

	}

	/*
	 * //单个获到小站基本信息 public List<RecommendContent>
	 * getMinitSiteData(List<RecommendContent> recommendContentList,int
	 * userid,GuideRecommendJadeDAO guideRecommendJadeDAO ,String from){ int
	 * returnListsize = recommendContentList.size(); RecommendContent
	 * recommendContent=null; if(StringUtils.isBlank(from)){ from="guide_home";
	 * } for (int i = 0; i < returnListsize; i++) { try { recommendContent=
	 * recommendContentList.get(i);
	 * guideRecommendJadeDAO.insertGuideRecommendCountViewLog(userid,
	 * 1,(recommendContent.getId()),from);
	 * guideRecommendJadeDAO.updateGuideRecommendView(recommendContent.getId());
	 * recommendContent.setSiteInfo(GuideRecommendLogic.
	 * getInstance().getXiaoZhanPojoById
	 * (recommendContent.getContent_business_id()));
	 * GuideLogger.printLog("recommendContentID:"
	 * +recommendContent.getSiteInfo().getId()); } catch (SQLException e) {
	 * e.printStackTrace(); } } return recommendContentList; }
	 */

	// 批量获取小站基本信息
	public List<RecommendContent> getBatchMinitSiteData(
			List<RecommendContent> recommendContentList, int userid, String from) {
		int returnListsize = recommendContentList.size();
		RecommendContent recommendContent = null;
		List<Long> ids = new ArrayList<Long>();
		for (int i = 0; i < returnListsize; i++) {
			recommendContent = recommendContentList.get(i);
			int id = recommendContent.getId();// 流水id
			//insertGuideRecommendCountViewLog(userid, 1, id, from, "");// 展示一次插入一条记录
			updateGuideRecommendView(id);// 展示一次一更新一次记录数
			ids.add(recommendContent.getContent_business_id());
		}

		List<SiteInfo> xiaoZhaoInfoList = getBatchXiaoZhanPojoByBatchId(ids);
		if (returnListsize == xiaoZhaoInfoList.size()) {
			for (int i = 0; i < returnListsize; i++) {
				long contentBusenessId = xiaoZhaoInfoList.get(i).getId();
				for (int j = 0; j < returnListsize; j++) {
					if (contentBusenessId == recommendContentList.get(j)
							.getContent_business_id()) {
						recommendContentList.get(j).setSiteInfo(
								xiaoZhaoInfoList.get(i));
						GuideLogger.printLog("recommendContentID:"
								+ recommendContentList.get(j).getSiteInfo()
										.getId());
						break;
					}
				}
			}
			return recommendContentList;
		} else {
			GuideLogger.printLog("batch get size is not same ids:" + ids);
			return new ArrayList<RecommendContent>();
		}
	}

	// 为发提高性能改用线程池
	public void insertGuideRecommendCountViewLog(final int userid,
			final int type, final int recommend_content_id, final String from,
			final String typeName) {
		Thread letter = new Thread() {
			@Override
			public void run() {
				try {
					guideRecommendJadeDAO.insertGuideRecommendCountViewLog(
							userid, type, recommend_content_id, from, typeName);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		};
		RunnableTaskExecutor.getInstance().runTask(letter);
	}

	public void insertGuideRecommendCountViewLog(final int userid,
			final int type, final List<Integer> recommendContentIds,
			final String from, final String typeName) {
		if (recommendContentIds == null || recommendContentIds.size() <= 0) {
			return;
		}
		Thread letter = new Thread() {
			@Override
			public void run() {
				try {
					int size = recommendContentIds.size();
					for (int i = 0; i < size; i++) {
						guideRecommendJadeDAO.insertGuideRecommendCountViewLog(
								userid, type, recommendContentIds.get(i), from,
								typeName);
					}

				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		};
		RunnableTaskExecutor.getInstance().runTask(letter);
	}

	public void updateGuideRecommendView(final int recommend_content_id) {
		Thread letter = new Thread() {
			@Override
			public void run() {
				try {
					guideRecommendJadeDAO
							.updateGuideRecommendView(recommend_content_id);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		};
		RunnableTaskExecutor.getInstance().runTask(letter);
	}

	public void updateGuideRecommendhits(final int recommend_content_id) {
		Thread letter = new Thread() {
			@Override
			public void run() {
				try {
					guideRecommendJadeDAO
							.updateGuideRecommendhits(recommend_content_id);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		};
		RunnableTaskExecutor.getInstance().runTask(letter);
	}

	// 发新鲜事
	public void sendFeed(final int[] ids, final int userid) {
		GuideLogger.printLog("userid:" + userid);
		if (ids == null) {
			return;
		}
		Thread letter = new Thread() {
			@Override
			public void run() {
				try {
					int size = ids.length;
					for (int i = 0; i < size; i++) {
						// zhanService.sendSiteFeedToUser(ids[i],userid,5);
						// GuideLogger.printLog("################id:"+ids[i]);
						FeedLogicAdapter.getInstance().addMiniSiteFeed(ids[i],
								userid);
					}

				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		};
		RunnableTaskExecutor.getInstance().runTask(letter);
	}

	public void sendFeed1(final List<RecommendContent> ids, final int userid) {
		GuideLogger.printLog("userid:" + userid);
		if (ids == null) {
			return;
		}
		Thread letter = new Thread() {
			@Override
			public void run() {
				try {
					int size = ids.size();
					for (int i = 0; i < size; i++) {
						// zhanService.sendSiteFeedToUser(ids[i],userid,5);
						// GuideLogger.printLog("################id:"+ids[i]);
						FeedLogicAdapter.getInstance().addMiniSiteFeed(
								new Integer(ids.get(i).getContent_business_id()
										+ ""), userid);
					}

				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		};
		RunnableTaskExecutor.getInstance().runTask(letter);
	}

	public void sendFeed(final List<Integer> ids, final int userid) {
		GuideLogger.printLog("userid:" + userid);
		if (ids == null) {
			return;
		}
		Thread letter = new Thread() {
			@Override
			public void run() {
				try {
					int size = ids.size();
					for (int i = 0; i < size; i++) {
						// GuideLogger.printLog("################id:"+ids.get(i));
						FeedLogicAdapter.getInstance().addMiniSiteFeed(
								ids.get(i), userid);
						// zhanService.sendSiteFeedToUser(ids.get(i),userid,5);

					}

				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		};
		RunnableTaskExecutor.getInstance().runTask(letter);
	}

	public void sendFeed(final int id, final int userid) {
		GuideLogger.printLog("userid:" + userid);
		Thread letter = new Thread() {
			@Override
			public void run() {
				try {
					// GuideLogger.printLog("################id:"+id);
					FeedLogicAdapter.getInstance().addMiniSiteFeed(id, userid);
					// zhanService.sendSiteFeedToUser(id,userid,5);

				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		};
		RunnableTaskExecutor.getInstance().runTask(letter);
	}

	public void insertNewflowLogRecord(final int userid, final int stage,
			final String fromSource, final int step, final String pageid) {
		Thread letter = new Thread() {
			@Override
			public void run() {
				try {
					GuideRecommendDAO.getInstance().insert_newflow_log_record(
							userid, stage, fromSource, step, pageid);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		};
		RunnableTaskExecutor.getInstance().runTask(letter);
	}

	public void insertGuideLogRecord(final int userid, final int stage,
			final String fromSource, final int step) {
		Thread letter = new Thread() {
			@Override
			public void run() {
				try {
					GuideRecommendDAO.getInstance().insert_guide_log_record(
							userid, stage, fromSource, step);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		};
		RunnableTaskExecutor.getInstance().runTask(letter);
	}

	/*
	 * -------------------------------Guide Z3 logic---------------------------------------
	 */
	private static List<GuideZ3Pojo> guideZ3List;

	public List<GuideZ3Pojo> getGuideZ3List(int sexFlag) {
		if (guideZ3List == null) {
			try {
				guideZ3List = guideRecommendJadeDAO.selectGuideZ3List(sexFlag);
			} catch (SQLException e) {
				GuideLogger.printLog(e.getMessage());
				e.printStackTrace();
			}
		}
		if (guideZ3List == null) {
			guideZ3List = new ArrayList<GuideZ3Pojo>();
		}
		return guideZ3List;
	}

	public void batchFoucsTag(final String tagValues[], final int userId) {
		final ServiceFactory fact = ServiceFactories.getFactory();
		final TagService service = fact.getService(TagService.class);
		ServiceFuture<Object> future = service.batchFollow(tagValues, userId);
		ServiceFutureHelper.execute(future, 5000);
		GuideLogger.printLog(">>>>>>>>>>>>>>>>>>>>>>>>>batchFoucsTag userId:" + userId);
	}

	public void addFoucsCount(final String tagValues[], final int sexFlag,final int uid) {
		Thread letter = new Thread() {
			@Override
			public void run() {
				try {
					if (tagValues != null) {
						StringBuilder sbf=new StringBuilder();
						for (int i = 0; i < tagValues.length; i++) {
							String tagValue = tagValues[i];
							guideRecommendJadeDAO.updateGuideZ3AddFoucsCount(tagValue, sexFlag);
							insertGuideRecommendCountViewLog(
									uid, 3, -1,"guidez3flow",tagValue);
							sbf.append(tagValue);
							sbf.append(":");
						}
						String tags=sbf.toString();
						if(tags==null || "".equals(tags.trim())){
							tags="notag";
						}
						guideRecommendJadeDAO.insertGuideUserFeedCount(uid, -10, tags);
					}
				} catch (Exception e) {
					GuideLogger.printLog(e.getMessage());
					e.printStackTrace();
				}
			}
		};
		RunnableTaskExecutor.getInstance().runTask(letter);
	}

	public void sendTagFeedToUser(final int userId, final String tagValues[], final int count) {
		final ServiceFactory fact = ServiceFactories.getFactory();
		final TagService service = fact.getService(TagService.class);
		String tagValue = "";
		try {
			if (tagValues != null) {
				for (int i = 0; i < tagValues.length; i++) {
					tagValue = tagValues[i];
					if(tagValue != null && !tagValue.equals("")){
						ServiceFuture<Object> future = service.sendTagFeedToUser(userId, tagValue, count);
						ServiceFutureHelper.execute(future, 5000);
						GuideLogger.printLog(">>>>>sendTagFeedToUser userId:" + userId+" tagValue:"+tagValue+" count:"+count);
					}
				}
			}
		} catch (Exception e) {
			GuideLogger.printLog("GuideZ3Flow sendTagFeedToUser |userId:" + userId + "|tagValue:" + tagValue + "|count:" + count);
			e.printStackTrace();
		}
	}
	
	//插入新鲜事条数方法
	public void insertGuideUserFeedCount(final int userId, final int feedCount,final String tags) {
		Thread letter = new Thread() {
			@Override
			public void run() {
				try {
					
					List<Integer> feedCountList=guideRecommendJadeDAO.selectGuideUserFeedCount(userId);
					//GuideLogger.printLog("********************  "+feedCountList);
					if(feedCountList !=null && feedCountList.size()>0 && feedCountList.get(0)==-10){
						GuideLogger.printLog("********************  feedCountListsize:"+feedCountList.size()+" feedcount:"+feedCount +" userid:"+userId);
						guideRecommendJadeDAO.updateGuideUserFeedCount(feedCount, userId);
					}
					//guideRecommendJadeDAO.insertGuideUserFeedCount(userId, feedCount, tags);
					
				} catch (Exception e) {
					GuideLogger.printLog(e.getMessage());
					e.printStackTrace();
				}
			}
		};
		RunnableTaskExecutor.getInstance().runTask(letter);
	}
	
	
	//给page增加默认关注
	//public Map<String, PagePojo> pageInfoMap = new HashMap<String, PagePojo>();
	private MemCacheManager mem = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_guide);
	public static final String MEM_CACHE_FROM_PAGE = "page_info_universityid_";
	public void followUniversityPage(int userId){
		//获取大学id
		List<UniversityInfo> uniList=null;
		int universityId=-1;
		try {
			uniList=DatumInfoMgr.getInstance().getUniversityInfos(userId);
			
		} catch (Exception e) {
			e.printStackTrace();
		}
		if(uniList==null || uniList.size()<=0){
			GuideLogger.printLog("*******uniList is null");
			return;
		}
		universityId=uniList.get(0).getUniversityId();
		
		//先从mem cache中取
		Object obj = mem.get(MEM_CACHE_FROM_PAGE+universityId);
		PagePojo pagePojo=null;
		// 如果取不到，把从库中取，并把结果放到mem cache中
		if (obj == null) {
			GuideLogger.printLog("*******obj is null");
			try {
				pagePojo = guideRecommendJadeDAO.selectGuideRecommendPage(universityId);
			} catch (Exception e) {
				e.printStackTrace();
			}
			if(pagePojo==null){
				GuideLogger.printLog("*******pagePojo is null");
				return;
			}
			mem.set(MEM_CACHE_FROM_PAGE+universityId, pagePojo, (int) MemCacheKeys.day * 5);
		}
		else{
			try {
				pagePojo = (PagePojo)obj;
			} catch (NullPointerException e) {
				e.printStackTrace();
			}
		}
		GuideLogger.printLog("*******suceess call method pageid:"+pagePojo.getPageId()+" userid:"+userId+" universityId:"+universityId);
		PageClientAdapter.makeFans(pagePojo.getPageId(), userId, true);//关注大学page
		forumMemberService.joinSchoolForum(userId, universityId);//关注小组主页，具体逻辑在小那边实现
		
		
	}
	
	//校园频道需求 新注册用户，如果满足以下两个条件
    //1、大学入学年为2011  2、填写了大学资料
    //就自动关注资料中填写的大学所对应的校园频道。
	public void followUniversityChannel(User host){
		class univThread implements Runnable{

            User host;
            public univThread(final User host){
                this.host = host;
            }

            @Override
            public void run() {
                List<UniversityInfo> uniList = null;
                try {
                    uniList = DatumInfoMgr.getInstance().getUniversityInfos(this.host.getId());
                } catch (final Exception e) {
                    e.printStackTrace();
                }
                if((uniList!=null) && (uniList.size() >0)){
                    for(final UniversityInfo uni:uniList){
                        if(uni.getEnrollYear() == 2011){
                            final Page schoolPage = PageService.getInstance().getPageBySchoolId(uni.getUniversityId());
                            if(!FansService.getInstance().isFans(schoolPage,this.host)){
                            	GuideLogger.printLog("schoolpage:"+schoolPage);
                            	if(schoolPage!=null){
                            		FansService.getInstance().reqBeFans(schoolPage, this.host);
                            	}
                            }
                            break;
                        }
                    }
                }

            }
        }
        ThreadPool.defaultInstance().execute(new univThread(host));
	}

}
