<%@ page pageEncoding="UTF-8" %>
<c:choose>
	<c:when test="${channel=='首页'}">
		<c:set var="ph" value="222" />
	</c:when>
	<c:when test="${channel=='相册'}">
		<c:set var="ph" value="257" />
	</c:when>
	<c:when test="${channel=='相册终端'}">
		<c:set var="ph" value="262" />
	</c:when>
	<c:when test="${channel=='日志'}">
		<c:set var="ph" value="267" />
	</c:when>
	<c:when test="${channel=='日志终端'}">
		<c:set var="ph" value="272" />
	</c:when>
	<c:when test="${channel=='群'}">
		<c:set var="ph" value="277" />
	</c:when>
	<c:when test="${channel=='群终端'}">
		<c:set var="ph" value="282" />
	</c:when>
	<c:when test="${channel=='站内信件'}">
		<c:set var="ph" value="287" />
	</c:when>
	<c:when test="${channel=='搜索' || channel=='搜索首页'}">
		<c:set var="ph" value="298" />
	</c:when>
	<c:when test="${channel=='我的好友'}">
		<c:set var="ph" value="318" />
	</c:when>
	<c:when test="${channel=='我的分享'}">
		<c:set var="ph" value="328" />
	</c:when>
	<c:when test="${channel=='我的班级'}">
		<c:set var="ph" value="338" />
	</c:when>
	<c:when test="${channel=='校友会'}">
		<c:set var="ph" value="348" />
	</c:when>
	<c:otherwise>
		<c:set var="ph" value="222" />
	</c:otherwise>
</c:choose>
<c:if test="${channel!='space' && channel!='搜索首页'}">
<div class="block announcement link">
<h3>热点推荐</h3>
<div class="blockcont text">
<c:choose>
	<c:when test="${requestScope.visiter.loginCount <= 10}">
		<IFRAME id="231" MARGINHEIGHT=0 MARGINWIDTH=0 FRAMEBORDER=0 WIDTH=100% HEIGHT=16 SCROLLING=NO SRC="http://gg.${applicationScope.domain}/view.jsp?p=231">
		</IFRAME>
	</c:when>
	<c:otherwise>
		<IFRAME id="221" MARGINHEIGHT=0 MARGINWIDTH=0 FRAMEBORDER=0 WIDTH=100% HEIGHT=16 SCROLLING=NO SRC="http://gg.${applicationScope.domain}/view.jsp?p=221">
		</IFRAME>
	</c:otherwise>
</c:choose>
<IFRAME id="${ph}" MARGINHEIGHT=0 MARGINWIDTH=0 FRAMEBORDER=0 WIDTH=100% HEIGHT=16 SCROLLING=NO SRC="http://gg.${applicationScope.domain}/view.jsp?p=${ph}">
</IFRAME>
<IFRAME id="${ph+1}" MARGINHEIGHT=0 MARGINWIDTH=0 FRAMEBORDER=0 WIDTH=100% HEIGHT=16 SCROLLING=NO SRC="http://gg.${applicationScope.domain}/view.jsp?p=${ph+1}">
</IFRAME>
<IFRAME id="224" MARGINHEIGHT=0 MARGINWIDTH=0 FRAMEBORDER=0 WIDTH=100% HEIGHT=16 SCROLLING=NO SRC="http://gg.${applicationScope.domain}/view.jsp?p=224">
</IFRAME>
<IFRAME id="225" MARGINHEIGHT=0 MARGINWIDTH=0 FRAMEBORDER=0 WIDTH=100% HEIGHT=16 SCROLLING=NO SRC="http://gg.${applicationScope.domain}/view.jsp?p=225">
</IFRAME>
<IFRAME id="p245" MARGINHEIGHT=0 MARGINWIDTH=0 FRAMEBORDER=0 WIDTH=100% HEIGHT=16 SCROLLING=NO SRC="http://gg.${applicationScope.domain}/view.jsp?p=245">
</IFRAME>
<IFRAME id="p246" MARGINHEIGHT=0 MARGINWIDTH=0 FRAMEBORDER=0 WIDTH=100% HEIGHT=16 SCROLLING=NO SRC="http://gg.${applicationScope.domain}/view.jsp?p=246">
</IFRAME>
<IFRAME id="p247" MARGINHEIGHT=0 MARGINWIDTH=0 FRAMEBORDER=0 WIDTH=100% HEIGHT=16 SCROLLING=NO SRC="http://gg.${applicationScope.domain}/view.jsp?p=247">
</IFRAME>
</div>
</div>
</c:if>