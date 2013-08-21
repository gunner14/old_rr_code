<%@ page contentType="text/html;charset=UTF-8" language="java"
	session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<dl class="wiki-intro-only">
	<c:if test="${not empty wikiInfo.director }">
		<dt title="导演">导演</dt>
		<dd>${wikiInfo.director }</dd>
	</c:if>
	<c:if test="${not empty wikiInfo.starring }">
		<dt title="主演">主演</dt>
		<dd>${wikiInfo.starring }</dd>
	</c:if>
	<c:if test="${not empty wikiInfo.type }">
		<dt title="类型">类型</dt>
		<dd>${wikiInfo.type }</dd>
	</c:if>
	<c:if test="${not empty wikiInfo.releaseDate }">
		<dt title="上映日期">上映日期</dt>
		<dd>${wikiInfo.releaseDate }</dd>
	</c:if>
	<c:if test="${not empty wikiInfo.summary }">
		<dt class="wiki-intro">简介</dt>
		<dd class="wiki-intro-note">${wikiInfo.summary }</dd>
	</c:if>
</dl>
