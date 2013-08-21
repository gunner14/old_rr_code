<%@ page pageEncoding="UTF-8" %>
<c:choose>
	<c:when test="${applicationScope.domain == 'kaixin.com'}">
		<%--<c:if test="${channel=='首页' || channel=='日志' || channel=='日志终端'}">
			<div id="ad1000000053"></div>
		</c:if>--%>
	</c:when>
	<c:when test="${applicationScope.domain == 'renren.com'}">
		<c:if test="${channel=='首页' || channel=='日志' || channel=='日志终端'}">
			<div id="ad1000000053"></div>
		</c:if>
	</c:when>
</c:choose>
