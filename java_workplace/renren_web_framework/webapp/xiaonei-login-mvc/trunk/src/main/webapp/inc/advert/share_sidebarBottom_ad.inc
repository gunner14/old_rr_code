﻿<%@ page pageEncoding="UTF-8" %>
<c:if test="${requestScope.isFromIM != true}">
	<c:if test="${applicationScope.domain != 'kaixin.com'}">
		<c:if test="${empty noSideAd && empty cleanpage && channel != '首页'}">
			<div id="ad100000000072"></div>
			<div id="ad100000000073"></div>
			<div id="ad100000000075"></div>
			<div id="ad100000000076"></div>
			<div id="ad100000000077"></div>
			<div id="ad100000000078"></div>
		</c:if>
	</c:if>
</c:if>
