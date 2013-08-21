package com.xiaonei.reg.guide.flows.xfive.logics;

import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;

import net.sf.json.JSONArray;
import net.sf.json.JSONObject;

import org.springframework.beans.factory.annotation.Autowired;

import com.ibm.icu.text.DecimalFormat;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.WUserCache;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.usertrace.dao.GuideRecommendJadeDAO;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.vip.commons.port.ToFriend;

/**
 * 
 * @author Kobin
 * @todo   推荐好友排序逻辑
 */
public class GuideXFiveUserSortLogic {
	@Autowired
	private GuideRecommendJadeDAO guideRecommendJadeDAO;
	
	/**
     * 根据用户价值评分排序：（用户价值评分 = 0.5 * 头像评分 + 0.5 * 登录情况得分）
     * @param  jsonStr
     *         推荐好友的json串：
     *         {"candidate":[{"id":420144700,"name":"米诺诺","net":" [北京服装学院]","head":"http://hdn.xnimg.cn/photos/hdn121/20111010/0735/tiny_UWil_1389a019117.jpg"}]}
     * @return 排序后的json串：增加用户价值评分（mark）属性
     */
    public String sortJsonArrayByUserMark(String jsonStr){
    	if(jsonStr == null){
    		return null;
    	}
    	
    	JSONObject jb = JSONObject.fromObject(jsonStr);
        Object jsonOb = jb.get("candidate");
        if(jsonOb == null){
        	return null;
        }
        
    	/*
    	 * 计算用户价值评分
    	 */
    	JSONArray jsonArray = JSONArray.fromObject(jsonOb);
    	for (int i = 0; i < jsonArray.size(); i++) {
    		JSONObject obj = JSONObject.fromObject(jsonArray.get(i));
    		
    		int userId = (Integer)obj.get("id");
    		
    		double mark = getUserMark(userId);
    		
    		obj.put("mark", mark);
    		
    		jsonArray.set(i, obj);
    	}
        	
    	/*
    	 * 按用户价值评分排序 
    	 */
    	List<JSONObject> jsonList = sortJsonArray(jsonArray);

    	/*
    	 * 返回排序后的json数据
    	 */
    	JSONObject jsonObj = new JSONObject();
    	jsonObj.put("candidate", JSONArray.fromObject(jsonList));
    	return jsonObj.toString();
    }
    
    
    /**
     * 添加好友时推荐好友
     * @param  selfId   登录者id【简称：A】
     * @param  ownerId  被添加者id【简称：B】
     * @return 按推荐度得分排序后的json串：
     *         {"candidate":[{"id":420144700,"name":"米诺诺","head":"http://head.xiaonei.com/photos/0/0/men_tiny.gif","mark":1.243}]}
     */
    public String getRecommendFriends(int selfId, int ownerId){
    	String resultJson = "";
    	//查询B的所有好友id列表
    	List<Integer> friendList = ToFriend.getFriendsIds(ownerId);
    	if(friendList != null && !friendList.isEmpty()){
    		JSONArray jsonArray = new JSONArray();
    		
    		Iterator<Integer> it = friendList.iterator();
    		while(it.hasNext()){
    			//B的好友id【简称：F】
    			int friendId = it.next();
    			
    			/*
    			 * 计算推荐度得分: 推荐度 = (F和A的关系 + F和B的关系 + F本身质量) / 3
    			 */
    			
    			// A与F的共同好友数
    			Set<Integer> shareIds_AF = SnsAdapterFactory.getBuddyByIdCacheAdapter().getSharedFriends(selfId, friendId); 
    			int counts_AF = 0;
    			if(shareIds_AF != null){
    				counts_AF = shareIds_AF.size();
    			}
    			// B与F的共同好友数
				Set<Integer> shareIds_BF = SnsAdapterFactory.getBuddyByIdCacheAdapter().getSharedFriends(ownerId, friendId);
				int counts_BF = 0;
    			if(shareIds_BF != null){
    				counts_BF = shareIds_BF.size();
    			}
    			// F的用户质量得分
    			double userMark = getUserMark(friendId);
    			double mark = (counts_AF + counts_BF + userMark) / 3;
    			// 格式化小数点位数
    			DecimalFormat df = new DecimalFormat("0.000");
    			mark = Double.parseDouble(df.format(mark));
    					
    			//组织json串数据
    			User user = SnsAdapterFactory.getUserAdapter().get(friendId);
    			if(user != null){
    				JSONObject obj = new JSONObject();
    				obj.put("id", friendId);
    				obj.put("name", user.getName());
//        			jsonObj.put("net", user);
    				obj.put("head", user.getTinyUrl());
    				obj.put("mark", mark);
        			jsonArray.add(obj);
    			}
    		}
    		
    		/*
        	 * 按用户价值评分排序 
        	 */
        	List<JSONObject> jsonList = sortJsonArray(jsonArray);

        	/*
        	 * 返回排序后的json数据
        	 */
        	JSONObject jsonObj = new JSONObject();
        	jsonObj.put("candidate", JSONArray.fromObject(jsonList));
        	resultJson = jsonObj.toString();
    	}
    	
    	return resultJson;
    }
    
    /**
     * 按用户价值评分排序
     * @param jsonArray
     * @return
     */
    private List<JSONObject> sortJsonArray(JSONArray jsonArray){
    	@SuppressWarnings({ "unchecked", "deprecation" })
		List<JSONObject> jsonList = JSONArray.toList(jsonArray, JSONObject.class);

    	Comparator<Object> comp = new Comparator<Object>(){
			@Override
			public int compare(Object o1, Object o2) {
				JSONObject obj1 = (JSONObject)o1;
				JSONObject obj2 = (JSONObject)o2;
				if(obj1.getDouble("mark") < obj2.getDouble("mark")){
					return 1;
				} else{
					return 0;
				}
			}
        };
    	
    	Collections.sort(jsonList, comp);
    	
    	return jsonList;
    }
    
    /**
     * 计算用户价值评分:（用户价值评分 = 0.5 * 头像评分 + 0.5 * 登录情况得分）
     * @param userId
     * @return
     */
    private double getUserMark(int userId){
    	User user = SnsAdapterFactory.getUserAdapter().get(userId);
    	
    	if(user == null){
    		return 0;
    	}
    	
		//是否有头像
    	String head = user.getTinyUrl();
		boolean hasHead = true;
		String menTiny = "men_tiny.gif";
		String womenTiny = "women_tiny.gif";
		if(head == null){
			hasHead = false;
		} else{
			if(head.contains(menTiny) || head.contains(womenTiny)){
				hasHead = false;
			}
		}
		//是否星级用户
		boolean isStar = user.isStarUser();
		
		/*
		 * 计算用户头像得分
		 */
		double userHeadMark = 0;
		if(isStar && hasHead){//星级头像用户
			userHeadMark = 1;
		} else if(hasHead && !isStar){//有头像非星级用户
			userHeadMark = 0.5;
		} else if(!hasHead){//无头像用户
			userHeadMark = 0;
		}
		
		//用户是否在线（默认异常情况下用户是离线状态）
		boolean isOnline = false;
		try {
			List<Integer> userList = new ArrayList<Integer>();
			userList.add(userId);
			final Map<Integer, WUserCache> friendUserCaches = SnsAdapterFactory.getUserCacheAdapter().getUserCacheMap(userList);
			WUserCache userCache = friendUserCaches.get(userId);
			isOnline = userCache.isOnline();
		} catch (Exception e1) {
			e1.printStackTrace();
		}
		
		/*
		 * 计算用户登录情况评分
		 */
		double userLoginMark = 0;
		if(isOnline){//当前在线
			userLoginMark = 1;
		} else{
			String record = "";
			try {
				record = guideRecommendJadeDAO.getUserLoginRecord(userId);
				if(record == null || record.length() != 28){
					GuideLogger.printLog("user_id:" + userId + " | userLogin record:" + record, GuideLogger.ERROR);
				}
			} catch (SQLException e) {
				e.printStackTrace();
			}
			userLoginMark = getLoginMark(record);
		}
		//用户在未来7天内的登录情况，最后得分为做归⼀化 2012-07-12 kobin
		if(userLoginMark > 1){
			userLoginMark = 1;
		}
		
		double mark = 0.5 * userHeadMark + 0.5 * userLoginMark;
		//格式化小数点位数
		DecimalFormat df = new DecimalFormat("0.000");
		mark = Double.parseDouble(df.format(mark));
		
		return mark;
    }
    
    
    /**
	 * 计算用户登录分数
	 * @param record（用户28天的登录记录）
	 * @return
	 */
	private double getLoginMark(String record){
		double mark = 0;
		if(record != null && record.length() == 28){
			mark = 0.499 * getLoginDays(record.substring(0, 7)) + 
				   0.129 * getLoginDays(record.substring(8, 14)) + 
				   0.199 * getLoginDays(record.substring(15, 21)) +
				   0.086 * getLoginDays(record.substring(22, 28));
		}
		return mark;
	}
	
	/**
	 * 计算用户一周登录天数
	 * @param record
	 * @return
	 */
	private int getLoginDays(String record){
		int s = Integer.parseInt(record, 2);
		int re = 0;
		while(s!=0){	
			s &= s-1;
			re++;
		}
		return re;
	}
}
