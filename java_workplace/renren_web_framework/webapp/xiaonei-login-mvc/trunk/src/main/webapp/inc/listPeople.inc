<%@ page pageEncoding="UTF-8" %>
<%@ taglib uri="http://jakarta.apache.org/taglibs/string-1.1" prefix="str" %>
<p class="image"><a href="${applicationScope.urlMain}/profile.do?id=${item.id}"><img src="${item.head_Url}" alt="${item.head_Url} (${item.name})" /></a></p>
<table class="info">
	<caption><a href="${applicationScope.urlMain}/profile.do?id=${item.id}">${item.name}</a><c:if test="${item.selected==1}"><span class="rank"><c:forEach begin="1" end="${item.star}"><a href="${applicationScope.urlProfile}/info/Help.do#help1_15"><img src="${applicationScope.urlSmallStatic}/img/star.gif" alt="星级" /></a></c:forEach></span></c:if><%@ include file="/inc/imonline.inc" %></caption>		
	
	<!--先设定一个networkid_的变量为网络ID -->
	<c:set var="networkid_" value="${item.univId}" />
	<tr><th><%@ include file="/inc/network.inc" %>:</th><td>${item.univName}</td></tr>
	<c:choose>
	<c:when test="${item.stage==20}">
	<tr><th>入学:</th><td><c:if test="${item.univYear > 0}"><a href="${applicationScope.urlBrowse}/Srch.do?action=uyear&amp;year=${item.univYear}&amp;univ=<str:encodeUrl>${item.univName}</str:encodeUrl>">${item.univYear}</a></c:if></td></tr>
	</c:when>
	<c:when test="${item.stage==10}">
	<tr><th>入学:</th><td><c:if test="${item.univYear > 0}"><a href="${applicationScope.urlBrowse}/ns.do?highschool_name=<str:encodeUrl>${item.univName}</str:encodeUrl>&yr=${item.univYear}&action=highschool">${item.univYear}</a></c:if></td></tr>
	</c:when>
	</c:choose>
	<c:if test="${people_rel == 'friendship'}"><tr><th>浏览:</th><td><a href="${applicationScope.urlFriend}/GetFriendList.do?id=${item.id}">好友</a><span class="pipe">|</span><a href="${applicationScope.urlBlog}/GetBlog.do?id=${item.id}">日志</a><span class="pipe">|</span><a href="${applicationScope.urlPhoto}/getalbumlist.do?id=${item.id}">相册</a><span class="pipe">|</span><a href="${applicationScope.urlMain}/share/ShareList.do?id=${item.id}">分享</a></td></tr></c:if>
</table>
