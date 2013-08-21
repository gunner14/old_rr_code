<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ page import="com.renren.sns.guide.model.GuideUserSomeInfo"%>
<%@ page import="com.renren.sns.guide.utils.GuideUtils"%>
<%@ page import="com.xiaonei.platform.core.model.User"%>
<%@ page import="org.json.JSONObject"%>
<%
JSONObject jsonObject = new JSONObject();
jsonObject.put("userId", "");
jsonObject.put("friendCount", "");
jsonObject.put("albumCount", "");
jsonObject.put("blogCount", "");
jsonObject.put("doingContent", "");
jsonObject.put("htmlContentForPubLisher", "");
jsonObject.put("middleSchoolName", "");
jsonObject.put("middleSchoolYear", "");
String json=jsonObject.toString();
try{
	User user=(User)request.getAttribute("user");
	int friendCount=(Integer)request.getAttribute("friendCount");
	int albumCount=(Integer)request.getAttribute("albumCount");
	int blogCount=(Integer)request.getAttribute("blogCount");
	int doingCount=(Integer)request.getAttribute("doingCount");
	String doingContent=(String)request.getAttribute("doingContent");
	String htmlContentForPubLisher=(String)request.getAttribute("htmlContentForPubLisher");
	String middleSchoolName = (String)request.getAttribute("middleSchoolName");
	int middleSchoolYear = (Integer)request.getAttribute("middleSchoolYear");
	GuideUserSomeInfo gusi=new GuideUserSomeInfo();
	gusi.setUserId(user.getId());    
	gusi.setFriendCount(friendCount);
	//相册数，包括头像相册，大头贴相册和快速上传？UGC那边也不太确定，反正至少包含俩
	gusi.setAlbumCount(albumCount);
	gusi.setBlogCount(blogCount);
	gusi.setDoingContent(doingContent);
	gusi.setMiddleSchoolName(middleSchoolName);	
	gusi.setMiddleSchoolYear(middleSchoolYear);
	gusi.setHtmlContentForPubLisher(htmlContentForPubLisher);
	json = GuideUtils.getInstance().getJSONForGuideUserSomeInfo(gusi);
}
catch(Exception e){	
	System.err.print("ajax_guide_get_some_user_info.jsp:"+e.toString());
}
out.println(json);
%>