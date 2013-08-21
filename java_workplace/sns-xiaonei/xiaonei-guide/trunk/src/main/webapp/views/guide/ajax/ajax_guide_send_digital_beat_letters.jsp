<%@ page pageEncoding="utf-8" %>
<%@page import="com.xiaonei.platform.component.feed.commerce.CommercePublisher"%>
<%@page import="com.xiaonei.platform.component.feed.commerce.model.CommerceModel"%>
<%@page import="com.xiaonei.platform.core.opt.base.BaseThreadUtil"%>
<%@page import="com.xiaonei.platform.core.model.User"%>
<%@page import="com.xiaonei.platform.core.cache.MemCacheKeys"%>
<%@page import="com.xiaonei.platform.core.cache.MemCacheFactory"%>
<%@page import="com.xiaonei.platform.core.cache.MemCacheManager"%>
<%
MemCacheManager mcm = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_guide);
User user=BaseThreadUtil.currentHost(request); 
if (mcm.get("guideSendDigitalBeatLetters" + user.getId()) == null ){
	mcm.set("guideSendDigitalBeatLetters" + user.getId(), "sended", (int) MemCacheKeys.day * 1);
	CommerceModel model = new CommerceModel();		
	model.setContent("<a href='http://huodong.renren.com/3q_index.html' target='_blank'>支持QQ or 360？ "+user.getName()+"做出了艰难的决定！</a>");	
	model.setFeedType(5044); 
	model.setFromID(user.getId());
	model.setFromName(user.getName());
	model.setPrefix("");
	model.setTitle("");
	model.setUrl("");
	CommercePublisher.publish(model);
}	
%>