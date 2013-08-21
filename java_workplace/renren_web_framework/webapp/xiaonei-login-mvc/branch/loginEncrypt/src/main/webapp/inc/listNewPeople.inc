<%@ page pageEncoding="UTF-8" %>
<%@ taglib uri="http://jakarta.apache.org/taglibs/string-1.1" prefix="str" %>
<p class="image"><a href="${applicationScope.urlMain}/profile.do?portal=search&from=s&id=${item.id}"><img src="${item.head_Url}" alt="${item.head_Url} (${item.name})" /></a></p>
<table class="info">
	<caption><a href="${applicationScope.urlMain}/profile.do?portal=search&from=s&id=${item.id}">${item.name}</a><c:if test="${item.selected==1}"><span class="rank"><c:forEach begin="1" end="${item.star}"><a href="${applicationScope.urlProfile}/info/Help.do#help1_15"><img src="${applicationScope.urlSmallStatic}/img/star.gif" alt="星级" /></a></c:forEach></span></c:if><%@ include file="/inc/imonline.inc" %></caption>		
	<tr><th>入学:</th><td><c:if test="${item.univYear > 0}"><a href="${applicationScope.urlBrowse}/Search.do?action=year&amp;year=${item.univYear}&amp;univ=${item.univId}">${item.univYear}</a></c:if></td></tr>
	<tr><th>宿舍:</th><td><a href="${applicationScope.urlBrowse}/Search.do?action=dorm&amp;dorm=<str:encodeUrl>${item.dorm}</str:encodeUrl>&amp;univ=${item.univId}">${item.dorm}</a></td></tr>
	<tr><th>院系:</th><td><a href="${applicationScope.urlBrowse}/Search.do?action=university&amp;univ=${item.univId}">${item.univName}</a><c:if test="${!empty item.department}"><span class="pipe">-</span><a href="${applicationScope.urlBrowse}/Search.do?action=department&amp;department=<str:encodeUrl>${item.department}</str:encodeUrl>&amp;univ=${item.univId}">${item.department}</a></c:if></td></tr>
	<c:if test="${people_rel == 'friendship'}"><tr><th>浏览:</th><td><a href="${applicationScope.urlFriend}/GetFriendList.do?id=${item.id}">好友</a><span class="pipe">|</span><a href="${applicationScope.urlBlog}/GetBlog.do?id=${item.id}">日志</a><span class="pipe">|</span><a href="${applicationScope.urlPhoto}/getalbumlist.do?id=${item.id}">相册</a><span class="pipe">|</span><a href="${applicationScope.urlMain}/share/ShareList.do?id=${item.id}">分享</a></td></tr></c:if>
</table>
