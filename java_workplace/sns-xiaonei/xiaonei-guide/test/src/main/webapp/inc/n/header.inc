<%@ page pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>	
<%@ taglib uri="http://www.renren.com/logo" prefix="logo" %>
<c:choose>
	<c:when test="${is_v6_user && (empty vipStyle || vipStyleId==2)}">
		<%@ include file="/inc/n/header2.inc" %>
	</c:when>
	<c:otherwise>
		<%@ include file="/inc/n/header1.inc" %>
	</c:otherwise>
</c:choose>