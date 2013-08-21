<%@ page contentType="text/html;charset=UTF-8" language="java"
	session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<dl class="wiki-intro-only">
	<c:if test="${not empty wikiInfo.developer }">
		<dt>开发商</dt>
		<dd>${wikiInfo.developer }</dd>
	</c:if>
	<c:if test="${not empty wikiInfo.genre }">
		<dt title="游戏类型">游戏类型</dt>
		<dd>${wikiInfo.genre }</dd>
	</c:if>
	
	<c:if test="${not empty wikiInfo.platform }">
		<dt title="游戏平台">游戏平台</dt>
		<dd>${wikiInfo.platform }</dd>
	</c:if>
	<c:if test="${not empty wikiInfo.role }">
		<dt title="主要角色">主要角色</dt>
		<dd>${wikiInfo.role }</dd>
	</c:if>
	<c:if test="${not empty wikiInfo.releaseDate }">
		<dt title="发行时间">发行时间</dt>
		<dd>${wikiInfo.releaseDate }</dd>
	</c:if>
	<c:if test="${not empty wikiInfo.summary }">		
		<dt class="wiki-intro">简介</dt>
		<dd class="wiki-intro-note">${wikiInfo.summary}</dd>
	</c:if>
</dl>
