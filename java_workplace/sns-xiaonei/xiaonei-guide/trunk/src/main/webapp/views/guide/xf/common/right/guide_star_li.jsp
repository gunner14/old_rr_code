<%@ page pageEncoding="utf-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>
<c:forEach var="item" items="${requestScope.guideKaixinHotStarList}" begin="0" end="2" varStatus="status">
	<li>
		<a class="photo" href="${applicationScope.urlWww}/profile.do?id=${item.id}">
			<img width="50" height="50" src="${item.tinyFullurl}"/>
		</a>
		<p>
			<strong>
				<a href="${applicationScope.urlWww}/profile.do?id=${item.id}">${item.name}</a>
			</strong>
			<span>
				<c:choose>
					<c:when test="${item.gender=='男生'}">男</c:when>
					<c:otherwise>女</c:otherwise>
				</c:choose>
			${item.age}岁</span>
			<c:if test="${! empty item.homeCity}">
				<span>家乡：${item.homeCity}</span>
			</c:if>
		</p>
	</li>	
</c:forEach>	
