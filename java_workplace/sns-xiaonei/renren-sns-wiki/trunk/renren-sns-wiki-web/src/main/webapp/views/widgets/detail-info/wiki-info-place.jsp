<%@ page contentType="text/html;charset=UTF-8" language="java"
	session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<dl class="wiki-intro-only">
	<c:if test="${not empty wikiInfo.level }">
		<dt title="景点级别">景点级别</dt>
		<dd><a href="">${wikiInfo.level }</a></dd>
	</c:if>
	<c:if test="${not empty wikiInfo.ticketPrice }">
		<dt title="门票价格">门票价格</dt>
		<dd>${wikiInfo.ticketPrice }</dd>
	</c:if>
	<c:if test="${not empty wikiInfo.location }">
		<dt title="地址">地址</dt>
		<dd>${wikiInfo.location }</dd>
	</c:if>
	<c:if test="${not empty wikiInfo.summary }">
		<dt class="wiki-intro">简介</dt>
		<dd class="wiki-intro-note">${wikiInfo.summary }</dd>
	</c:if>
</dl>