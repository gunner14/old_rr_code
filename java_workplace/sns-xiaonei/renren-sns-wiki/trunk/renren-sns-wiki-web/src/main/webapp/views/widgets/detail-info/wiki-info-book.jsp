<%@ page contentType="text/html;charset=UTF-8" language="java"
	session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<dl class="wiki-intro-only">
	<c:if test="${not empty wikiInfo.author }">
		<dt>作者</dt>
		<dd>${wikiInfo.author }</dd>
	</c:if>
	<c:if test="${not empty wikiInfo.publishDate }">
		<dt>出版时间</dt>
		<dd>${wikiInfo.publishDate }</dd>
	</c:if>
	<c:if test="${not empty wikiInfo.publisher }">
		<dt title="出版社">出版社</dt>
		<dd>${wikiInfo.publisher }</dd>
	</c:if>
	<c:if test="${not empty wikiInfo.isbn }">
		<dt title="ISBN">ISBN</dt>
		<dd>${wikiInfo.isbn }</dd>
	</c:if>
	<c:if test="${not empty wikiInfo.summary }">
		<dt class="wiki-intro">简介</dt>
		<dd class="wiki-intro-note">${wikiInfo.summary }</dd>
	</c:if>
</dl>
