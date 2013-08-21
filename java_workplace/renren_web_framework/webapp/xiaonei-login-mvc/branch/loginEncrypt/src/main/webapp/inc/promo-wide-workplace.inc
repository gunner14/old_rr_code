<%@ page pageEncoding="UTF-8" %>
<c:choose>
	<c:when test="${channel=='首页'}">
		<c:set var="pw" value="219" />
		<c:set var="pr" value="466" />
	</c:when>
	<c:when test="${channel=='相册'}">
		<c:set var="pw" value="254" />
		<c:set var="pr" value="469" />
	</c:when>
	<c:when test="${channel=='相册终端'}">
		<c:set var="pw" value="259" />
		<c:set var="pr" value="472" />
	</c:when>
	<c:when test="${channel=='日志'}">
		<c:set var="pw" value="264" />
		<c:set var="pr" value="475" />
	</c:when>
	<c:when test="${channel=='日志终端'}">
		<c:set var="pw" value="269" />
		<c:set var="pr" value="478" />
	</c:when>
	<c:when test="${channel=='群'}">
		<c:set var="pw" value="274" />
		<c:set var="pr" value="481" />
	</c:when>
	<c:when test="${channel=='群终端'}">
		<c:set var="pw" value="279" />
		<c:set var="pr" value="484" />
	</c:when>
	<c:when test="${channel=='站内信件'}">
		<c:set var="pw" value="284" />
		<c:set var="pr" value="487" />
	</c:when>
	<c:when test="${channel=='搜索' || channel=='搜索首页'}">
		<c:set var="pw" value="294" />
		<c:set var="pr" value="490" />
	</c:when>
	<c:when test="${channel=='我的好友'}">
		<c:set var="pw" value="315" />
		<c:set var="pr" value="493" />
	</c:when>
	<c:when test="${channel=='我的分享'}">
		<c:set var="pw" value="325" />
		<c:set var="pr" value="496" />
	</c:when>
	<c:when test="${channel=='我的班级'}">
		<c:set var="pw" value="335" />
		<c:set var="pr" value="499" />
	</c:when>
	<c:when test="${channel=='校友会'}">
		<c:set var="pw" value="345" />
		<c:set var="pr" value="502" />
	</c:when>
	<c:otherwise>
		<c:set var="pw" value="219" />
		<c:set var="pr" value="466" />
	</c:otherwise>
</c:choose>

<c:if test="${channel!='space'}">
<div class="blank-holder">
<div id="banner" class="banner clearfix">
<div class="float-left">
<IFRAME id="p44" MARGINHEIGHT=0 MARGINWIDTH=0 FRAMEBORDER=0 WIDTH=690 HEIGHT=60 SCROLLING=NO SRC="http://gg.${applicationScope.domain}/view.jsp?p=${pw}">
<NOSCRIPT><A HREF="http://gg.${applicationScope.domain}/direct.jsp?p=${pw}"><IMG SRC="http://gg.${applicationScope.domain}/view.jsp?p=${pw}" WIDTH=690 HEIGHT=60 BORDER=0></a></NOSCRIPT></IFRAME>
</div>
<div class="float-right">
<IFRAME id="p${pr}" MARGINHEIGHT=0 MARGINWIDTH=0 FRAMEBORDER=0 WIDTH=264 HEIGHT=60 SCROLLING=NO SRC="http://gg.${applicationScope.domain}/view.jsp?p=${pr}">
<NOSCRIPT><A HREF="http://gg.${applicationScope.domain}/direct.jsp?p=${pr}"><IMG SRC="http://gg.${applicationScope.domain}/view.jsp?p=${pr}" WIDTH=264 HEIGHT=60 BORDER=0></a></NOSCRIPT></IFRAME>
</div>
</div>
</div>
</c:if>
