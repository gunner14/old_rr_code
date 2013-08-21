<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<c:set property="_finc_id" value="${finc_id}"></c:set> 
<c:choose>
	<c:when test="${_finc_id = "53"}">
		<%@ include file="/pages/register/reg/inc/form/53.inc.jsp" %>
	</c:when>
	<c:when test="${_finc_id = "61"}">
		<%@ include file="/pages/register/reg/inc/form/61.inc.jsp" %>
	</c:when>
	<c:when test="${_finc_id = "62"}">
		<%@ include file="/pages/register/reg/inc/form/62.inc.jsp" %>
	</c:when>
</c:choose>