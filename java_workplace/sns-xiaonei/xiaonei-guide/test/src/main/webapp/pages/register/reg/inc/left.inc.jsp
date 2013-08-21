<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<c:set property="_linc_id" value="${linc_id}"></c:set> 
<c:choose>
<c:when test="${_linc_id = 'login-panel'}">
<%@ include file="/pages/register/reg/inc/left/login-panel.inc.jsp" %>
</c:when>
</c:choose>