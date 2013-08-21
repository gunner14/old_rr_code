<%@ page pageEncoding="UTF-8" %>
<c:choose>
	<c:when test="${channel=='首页'}">
		<c:set var="pw2" value="467" />
		<c:set var="pr2" value="468" />
	</c:when>
	<c:when test="${channel=='相册'}">
		<c:set var="pw2" value="470" />
		<c:set var="pr2" value="471" />
	</c:when>
	<c:when test="${channel=='相册终端'}">
		<c:set var="pw2" value="473" />
		<c:set var="pr2" value="474" />
	</c:when>
	<c:when test="${channel=='日志'}">
		<c:set var="pw2" value="476" />
		<c:set var="pr2" value="477" />
	</c:when>
	<c:when test="${channel=='日志终端'}">
		<c:set var="pw2" value="479" />
		<c:set var="pr2" value="480" />
	</c:when>
	<c:when test="${channel=='群'}">
		<c:set var="pw2" value="482" />
		<c:set var="pr2" value="483" />
	</c:when>
	<c:when test="${channel=='群终端'}">
		<c:set var="pw2" value="485" />
		<c:set var="pr2" value="486" />
	</c:when>
	<c:when test="${channel=='站内信件'}">
		<c:set var="pw2" value="488" />
		<c:set var="pr2" value="489" />
	</c:when>
	<c:when test="${channel=='搜索' || channel=='搜索首页'}">
		<c:set var="pw2" value="491" />
		<c:set var="pr2" value="492" />
	</c:when>
	<c:when test="${channel=='我的好友'}">
		<c:set var="pw2" value="494" />
		<c:set var="pr2" value="495" />
	</c:when>
	<c:when test="${channel=='我的分享'}">
		<c:set var="pw2" value="497" />
		<c:set var="pr2" value="498" />
	</c:when>
	<c:when test="${channel=='我的班级'}">
		<c:set var="pw2" value="500" />
		<c:set var="pr2" value="501" />
	</c:when>
	<c:when test="${channel=='校友会'}">
		<c:set var="pw2" value="503" />
		<c:set var="pr2" value="504" />
	</c:when>
	<c:otherwise>
		<c:set var="pw2" value="467" />
		<c:set var="pr2" value="468" />
	</c:otherwise>
</c:choose>
<c:if test="${channel!='space'}">
<div id="bannerBottom" class="banner clearfix">
	<div class="float-left">
		<IFRAME id="p${pw2}" MARGINHEIGHT=0 MARGINWIDTH=0 FRAMEBORDER=0 WIDTH=690 HEIGHT=60 SCROLLING=NO SRC="http://gg.${applicationScope.domain}/view.jsp?p=${pw2}">
		<NOSCRIPT><A HREF="http://gg.${applicationScope.domain}/direct.jsp?p=${pw2}"><IMG SRC="http://gg.${applicationScope.domain}/view.jsp?p=${pw2}" WIDTH=690 HEIGHT=60 BORDER=0></a></NOSCRIPT></IFRAME>
	</div>
	<div class="float-right">
		<IFRAME id="p${pr2}" MARGINHEIGHT=0 MARGINWIDTH=0 FRAMEBORDER=0 WIDTH=264 HEIGHT=60 SCROLLING=NO SRC="http://gg.${applicationScope.domain}/view.jsp?p=${pr2}">
		<NOSCRIPT><A HREF="http://gg.${applicationScope.domain}/direct.jsp?p=${pr2}"><IMG SRC="http://gg.${applicationScope.domain}/view.jsp?p=${pr2}" WIDTH=264 HEIGHT=60 BORDER=0></a></NOSCRIPT></IFRAME>
	</div>
</div>
</c:if>