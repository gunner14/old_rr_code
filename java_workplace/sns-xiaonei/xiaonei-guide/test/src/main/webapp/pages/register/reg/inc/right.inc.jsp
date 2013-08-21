<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<c:set property="_rinc_id" value="${rinc_id}"></c:set> 
<c:choose>
<c:when test="${_rinc_id = 'dog'}">
<%@ include file="/pages/register/reg/inc/right/dog.inc.jsp" %>
</c:when>
</c:choose>