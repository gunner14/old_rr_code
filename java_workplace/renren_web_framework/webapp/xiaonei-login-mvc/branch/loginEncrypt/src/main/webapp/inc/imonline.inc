<%@ page pageEncoding="UTF-8" %>
<c:choose>
	<c:when test="${!empty requestScope.hostid}">
		<c:set var="loginerId" value="${requestScope.hostid}"/>
	</c:when>
	<c:otherwise>
		<c:set var="loginerId" value="0"/>
	</c:otherwise>
</c:choose>    
<c:if test="${(item.id == requestScope.hostid) && (item.status ==1)}">
	在线:)
</c:if>
<c:if test="${(item.id != requestScope.hostid) && (item.status ==1)}">
	<a href="#" class="imstatus" onclick="javascript:talkto(event, ${visiter.id}, ${item.id}, '${requestScope.visiter.email}', '${item.name}');return false">在线</a>:)
</c:if>