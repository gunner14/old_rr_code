<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@page import="org.json.JSONObject"%>
<%@page import="com.renren.sns.guide.model.FriendWithinTime"%>
<%@page import="com.renren.sns.guide.utils.GuideUtils"%>
<%@page import="com.xiaonei.platform.core.model.User"%>
<%
JSONObject jsonObject = new JSONObject();
jsonObject.put("userId", "");
jsonObject.put("userName", "");
jsonObject.put("userTinyUrl", "");
jsonObject.put("userBlogCount", "");
jsonObject.put("userDoingCount", "");
jsonObject.put("userFriendCount", "");
jsonObject.put("userAlbumCount", "");
jsonObject.put("userDoingId", "");
jsonObject.put("userDoingTime", "");
jsonObject.put("userNewestDoing", "");
jsonObject.put("isFriend", "");
jsonObject.put("isShowProfile", "");
jsonObject.put("isShowGossipBoard", "");
String json=jsonObject.toString();
try{
	User user=(User)request.getAttribute("user");
	int friendCount=0;
	if(request.getAttribute("friendCount")!=null){
		friendCount=(Integer)request.getAttribute("friendCount");
	}
	
	int albumCount=0;
	if(request.getAttribute("guideAlbumCountbiaoshi")!=null){
		albumCount=(Integer)request.getAttribute("guideAlbumCountbiaoshi");
	}
	
	int blogCount=0;
	if(request.getAttribute("blogCountkk")!=null){
		blogCount=(Integer)request.getAttribute("blogCountkk");
	}

	int doingCount=0;
	if(request.getAttribute("doingCountkku")!=null){
		doingCount=(Integer)request.getAttribute("doingCountkku");
	}

	long doingId=0;
	if(request.getAttribute("doingId")!=null){
		doingId=(Long)request.getAttribute("doingId");
	}

	String doingContent=(String)request.getAttribute("doingContent");
	
	String updateTitle=(String)request.getAttribute("updateTitle");
	
	boolean isFriend=(Boolean)request.getAttribute("isFriend");
	boolean showProfile=(Boolean)request.getAttribute("showProfilekku");
	boolean showGossipBoard=(Boolean)request.getAttribute("showGossipBoard");
	FriendWithinTime fwt=new FriendWithinTime();
	fwt.setUserId(user.getId());    
	fwt.setUserName(user.getName());
	fwt.setUserTinyUrl(user.getTinyFullUrl());
	fwt.setFriendCount(friendCount);
	//相册数，包括头像相册，大头贴相册和快速上传？UGC那边也不太确定，反正至少包含俩
	fwt.setAlbumCount(albumCount);
	fwt.setBlogCount(blogCount);
	fwt.setDoingCount(doingCount);
	fwt.setFriend(isFriend);
	fwt.setDoingId(doingId);
	fwt.setNewestDoing(doingContent);
	fwt.setDoingTime(updateTitle);
	fwt.setShowProfile(showProfile);
	fwt.setShowGossipBoard(showGossipBoard);
	json = GuideUtils.getInstance().getJSONForGetFriendWithinTime(fwt);
	
}
catch(Exception e){	
	e.printStackTrace();
}
out.println(json);
%>