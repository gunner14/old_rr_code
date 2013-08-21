<%@ page contentType="text/html;charset=UTF-8" session="false" %>
<%--
保存首次进入注册页面时带入的参数
author: wang-tai
--%> 
<c:if test="${!empty i}">
	<input type="hidden" name="i" value="${i}" />
</c:if>
<c:if test="${!empty code}">
	<input type="hidden" name="code" value="${code}" />
</c:if>
<c:if test="${!empty from}">
	<input type="hidden" name="from" value="${from}" />
</c:if>
<c:if test="${!empty appId}">
	<input type="hidden" name="appId" value="${appId}" />
</c:if>
<c:if test="${!empty inviteType}">
	<input type="hidden" name="inviteType" value="${inviteType}" />
</c:if>
<c:if test="${!empty e}">
	<input type="hidden" name="e" value="${e}" />
</c:if>
<c:if test="${!empty appToken}">
	<input type="hidden" name="appToken" value="${appToken}" />
</c:if>