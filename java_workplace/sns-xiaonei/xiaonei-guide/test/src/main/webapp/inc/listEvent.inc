<%@ page pageEncoding="UTF-8" %>
<%@ taglib uri="http://jakarta.apache.org/taglibs/string-1.1" prefix="str" %>
<table class="info">
	<caption><a href="${applicationScope.urlEvent}/GetEvent.do?id=${item.eventId}">${item.eventName}</a></caption>
	<tr><th>创 建 人:</th><td><a href="${applicationScope.urlMain}/profile.do?id=${item.ownerId}">${item.ownerName}</a> (${item.ownerUnivName})</td></tr>
	<c:if test="${!empty item.eventHost}">
		<tr><th>主 办 方:</th><td>${item.eventHost}</td></tr>
	</c:if>
	<tr><th>活动地点:</th><td>${item.location}</td></tr>
	<tr><th>开始时间:</th><td class="time"><fmt:formatDate value="${item.beginTime}" type="date" pattern="yyyy-MM-dd HH:mm" /></td></tr>
	<c:if test="${!empty item.endTime}">
		<tr><th>结束时间:</th><td class="time"><fmt:formatDate value="${item.endTime}" type="date" pattern="yyyy-MM-dd HH:mm" /></td></tr>
	</c:if>
</table>
