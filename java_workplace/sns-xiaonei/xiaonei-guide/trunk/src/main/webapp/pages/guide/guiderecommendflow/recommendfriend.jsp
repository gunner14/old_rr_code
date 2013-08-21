<%@ page contentType="text/html;charset=UTF-8" session="false"%><%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %><%@page import="com.xiaonei.platform.core.model.WUserCache"%><%@page import="java.util.Collection"%><%! Collection<WUserCache> recommendFriend=null;
%>
<%recommendFriend=(Collection<WUserCache>)request.getAttribute("guidefriendcmap");
%>
<c:forEach var="friend" items="<%=recommendFriend%>"><li id="commonFriend_${friend.id}" uid="${friend.id}" class="clearfix" data-index="0">
<span class="headpichold">
<a logtype="imgToPrf" uid="${friend.id}" href="http://www.renren.com/profile.do?id=${friend.id}">
<img logtype="imgToPrf" uid="${friend.id}" width="50" height="50" onload="clipImage(this)" src="${friend.tiny_Url}">
</a>
</span>
<span>
<a logtype="nameToPrf" uid="${friend.id}" href="http://www.renren.com/profile.do?id=${friend.id}" class="name">${friend.name}</a>
<p>
<a logtype="addFriend" uid="${friend.id}" onclick="showRequestFriendDialog('${friend.id}','${friend.name}','${friend.tiny_Url}', '', '', XN.app.pymk.removeFromAdd);return false;" href="javascript:;" class="addfriend_action"><img src="http://a.xnimg.cn/imgpro/icons/plus-green.png"> 加为好友</a>
</p>
<p>
<c:if test="${friend.gender!=null && friend.gender!=''}"><a logtype="info" uid="${friend.id}" onclick="XN.app.pymk.showProfile('${friend.id}');" href="javascript:;">查看资料</a>
</c:if></p></span>
<div id="more-mayknow-${friend.id}" class="comefrom" style="display: none;">
<div id="more-person-data-${friend.id}">
<p>
<span class="profile-label">性别：</span>${friend.gender}
</p>
<p>
<c:if test="${friend.univId!=0}"><c:set var="networkid_" value="${friend.univId}" />						
所在<%@ include file="/inc/network.inc"%>:${friend.univName}							
</c:if>					
</p>
</div>
</div>
	<input type="hidden" value="FFW" id="type_value_${friend.id}">
	<input type="hidden" id="guest_${friend.id}" value="${friend.id}"/>
	<input type="hidden" id="log_imgToPrf_${friend.id}" value="RecGuide_imgToPrf"/>
	<input type="hidden" id="log_nameToPrf_${friend.id}" value="RecGuide_nameToPrf"/>
	<input type="hidden" id="log_addFriend_${friend.id}" value="RecGuide_addFriend"/>
	<input type="hidden" id="log_info_${friend.id}" value="RecGuide_info"/>
	<input type="hidden" id="log_show_${friend.id}" value="RecGuide_show"/>
	<input type="hidden" id="log_addFriendEnd_${friend.id}" value="RecGuide_addFriendEnd"/>
</li>
</c:forEach>