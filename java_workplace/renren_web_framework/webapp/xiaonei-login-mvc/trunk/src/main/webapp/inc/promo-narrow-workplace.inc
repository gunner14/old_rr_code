<%@ page pageEncoding="UTF-8" %>
<c:choose>
	<c:when test="${channel=='首页'}">
		<c:set var="pn" value="220" />
	</c:when>
	<c:when test="${channel=='相册'}">
		<c:set var="pn" value="255" />
	</c:when>
	<c:when test="${channel=='相册终端'}">
		<c:set var="pn" value="260" />
	</c:when>
	<c:when test="${channel=='日志'}">
		<c:set var="pn" value="265" />
	</c:when>
	<c:when test="${channel=='日志终端'}">
		<c:set var="pn" value="270" />
	</c:when>
	<c:when test="${channel=='群'}">
		<c:set var="pn" value="275" />
	</c:when>
	<c:when test="${channel=='群终端'}">
		<c:set var="pn" value="280" />
	</c:when>
	<c:when test="${channel=='站内信件'}">
		<c:set var="pn" value="285" />
	</c:when>
	<c:when test="${channel=='搜索' || channel=='搜索首页'}">
		<c:set var="pn" value="295" />
	</c:when>
	<c:when test="${channel=='我的好友'}">
		<c:set var="pn" value="316" />
	</c:when>
	<c:when test="${channel=='我的分享'}">
		<c:set var="pn" value="326" />
	</c:when>
	<c:when test="${channel=='我的班级'}">
		<c:set var="pn" value="336" />
	</c:when>
	<c:when test="${channel=='校友会'}">
		<c:set var="pn" value="346" />
	</c:when>
	<c:otherwise>
		<c:set var="pn" value="220" />
	</c:otherwise>
</c:choose>
<c:if test="${channel=='我的好友' || channel=='站内信件' || channel=='日志终端' || channel=='相册' || channel=='相册终端'}">
<IFRAME id="p44" MARGINHEIGHT=0 MARGINWIDTH=0 FRAMEBORDER=0 WIDTH=120 HEIGHT=280 SCROLLING=NO SRC="http://gg.${applicationScope.domain}/view.jsp?p=${pn}">
<NOSCRIPT><A HREF="http://gg.${applicationScope.domain}/direct.jsp?p=${pn}"><IMG SRC="http://gg.${applicationScope.domain}/view.jsp?p=${pn}" WIDTH=120 HEIGHT=280 BORDER=0></a></NOSCRIPT></IFRAME>
</c:if>