package com.renren.xce.socialgraph.builder;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import xce.socialgraph.recommend.forum.ForumData;
import xce.socialgraph.recommend.forum.ForumRecommendUtil;

import com.renren.forum.biz.MemberForumBiz;
import com.renren.forum.model.Forum;
import com.renren.forum.model.MemberForum;
import com.renren.forum.service.ForumService;
import com.renren.xce.socialgraph.common.DataBuilder;
import com.renren.xce.socialgraph.common.DataItem;
import com.renren.xce.socialgraph.updator.CreateRecommendDataThread;
import com.renren.xce.socialgraph.updator.NewRecommendUpdator;
import com.xiaonei.xce.socialgraph.proto.GeneratedItems.Item;

/**
 * building data of forum recommend
 * @author zhangnan
 * @email zhangnan@renren-inc.com
 */
public class ForumRecommendDataBuilder implements DataBuilder {
	private static int CACHE_SHARES_SIZE = 2;		//show 2 common friends
	
	public static ForumService forumService = null;
	public static MemberForumBiz memberForumBiz = null;
	
	public List<DataItem> getData(int hostId) {
		Initialize();			//initialize foruService and memberForumBiz
		
		final String forumUrl = "http://xiaozu.renren.com/xiaozu/";
		List<DataItem> resultList = new ArrayList<DataItem>();
		List<ForumData> forumRecommendList = ForumRecommendUtil.getInstance().getForumRecommendList(hostId);		//get recommendation list
		List<MemberForum> existForumList = memberForumBiz.getForumByMember(hostId);				//get forum member of host
		Set<Integer> fliterForumId = new HashSet<Integer>();
		
		for (MemberForum it : existForumList) {
			fliterForumId.add((int)it.getForumId());
		}
		for (ForumData data : forumRecommendList) {
			if (fliterForumId.contains(data.getAppId())) {		//fliter forum exist in host forum list
				continue;
			}
			Item.Builder item = Item.newBuilder();
			item.setId(data.getAppId());			//forum id
			item.setField(data.getSharesCount());			//shares count
			item.setMessage(CreateRecommendDataThread.RFORUM);			//type of recommendation
			
			Forum detail = forumService.getForumForIndexCommend((long)data.getAppId());
			item.addMessages(detail.getName());				//name of forum in position 0
			item.addMessages(forumUrl + data.getAppId());				//url of forum in position 1
			item.addMessages(detail.getTinyUrl());			//tinyUrl of forum in position 2
			
			for (Integer shareId : data.getShareList()) {					//set shares
				if (item.getFieldsCount() >= CACHE_SHARES_SIZE) {
					break;
				}
				item.addFields(shareId);
			}
			
			DefaultDataItem forumItem = new DefaultDataItem((long)data.getAppId(), item);
			resultList.add(forumItem);
		}
		
		return resultList;
	}

	public String getBuilderType() {
		// TODO Auto-generated method stub
		return CreateRecommendDataThread.RFORUM;
	}
	
	/**
	 * Initialize members of class, forumService and memberForumBiz
	 */
	private void Initialize() {
		if (null == forumService) {
			forumService = NewRecommendUpdator.context.getBean(ForumService.class);
		}
		if (null == memberForumBiz) {
			memberForumBiz = NewRecommendUpdator.context.getBean(MemberForumBiz.class);
		}
	}
}
