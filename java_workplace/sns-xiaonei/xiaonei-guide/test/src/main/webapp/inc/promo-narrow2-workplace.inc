<%@ page pageEncoding="UTF-8" %>
<c:choose>
	<c:when test="${channel=='首页'}">
		<c:set var="pn2" value="253" />
	</c:when>
	<c:when test="${channel=='相册'}">
		<c:set var="pn2" value="256" />
	</c:when>
	<c:when test="${channel=='相册终端'}">
		<c:set var="pn2" value="261" />
	</c:when>
	<c:when test="${channel=='日志'}">
		<c:set var="pn2" value="355" />
	</c:when>
	<c:when test="${channel=='日志终端'}">
		<c:set var="pn2" value="271" />
	</c:when>
	<c:when test="${channel=='群'}">
		<c:set var="pn2" value="276" />
	</c:when>
	<c:when test="${channel=='群终端'}">
		<c:set var="pn2" value="281" />
	</c:when>
	<c:when test="${channel=='站内信件'}">
		<c:set var="pn2" value="286" />
	</c:when>
	<c:when test="${channel=='搜索' || channel=='搜索首页'}">
		<c:set var="pn2" value="297" />
	</c:when>
	<c:when test="${channel=='我的好友'}">
		<c:set var="pn2" value="317" />
	</c:when>
	<c:when test="${channel=='我的分享'}">
		<c:set var="pn2" value="327" />
	</c:when>
	<c:when test="${channel=='我的班级'}">
		<c:set var="pn2" value="337" />
	</c:when>
	<c:when test="${channel=='校友会'}">
		<c:set var="pn2" value="347" />
	</c:when>
	<c:otherwise>
		<c:set var="pn2" value="253" />
	</c:otherwise>
</c:choose>
<c:if test="${channel!='space' && channel!='搜索首页'}">
<IFRAME id="p44" MARGINHEIGHT=0 MARGINWIDTH=0 FRAMEBORDER=0 WIDTH=120 HEIGHT=180 SCROLLING=NO SRC="http://gg.${applicationScope.domain}/view.jsp?p=${pn2}">
<NOSCRIPT><A HREF="http://gg.${applicationScope.domain}/direct.jsp?p=${pn2}"><IMG SRC="http://gg.${applicationScope.domain}/view.jsp?p=${pn2}" WIDTH=120 HEIGHT=180 BORDER=0></a></NOSCRIPT></IFRAME>
</c:if>