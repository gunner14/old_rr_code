<%@ page pageEncoding="UTF-8" %>
<c:choose>
	<c:when test="${channel=='首页'}">
		<c:set var="pn" value="134" />
	</c:when>
	<c:when test="${channel=='相册'}">
		<c:set var="pn" value="146" />
	</c:when>
	<c:when test="${channel=='相册终端'}">
		<c:set var="pn" value="172" />
	</c:when>
	<c:when test="${channel=='日志'}">
		<c:set var="pn" value="142" />
	</c:when>
	<c:when test="${channel=='日志终端'}">
		<c:set var="pn" value="174" />
	</c:when>
	<c:when test="${channel=='群'}">
		<c:set var="pn" value="150" />
	</c:when>
	<c:when test="${channel=='群终端'}">
		<c:set var="pn" value="170" />
	</c:when>
	<c:when test="${channel=='站内信件'}">
		<c:set var="pn" value="162" />
	</c:when>
	<c:when test="${channel=='搜索' || channel=='搜索首页'}">
		<c:set var="pn" value="140" />
	</c:when>
	<c:when test="${channel=='我的好友'}">
		<c:set var="pn" value="136" />
	</c:when>
	<c:when test="${channel=='我的分享'}">
		<c:set var="pn" value="166" />
	</c:when>
	<c:when test="${channel=='活动'}">
		<c:set var="pn" value="154" />
	</c:when>
	<c:when test="${channel=='我的课程'}">
		<c:set var="pn" value="164" />
	</c:when>
	<c:when test="${channel=='我的班级'}">
		<c:set var="pn" value="158" />
	</c:when>
	<c:when test="${channel=='邀请'}">
		<c:set var="pn" value="160" />
	</c:when>
	<c:when test="${channel=='跳蚤市场'}">
		<c:set var="pn" value="443" />
	</c:when>
	<c:when test="${channel=='电影'}">
		<c:set var="pn" value="364" />
	</c:when>
	<c:when test="${channel=='新鲜事更多页'}">
		<c:set var="pn" value="463" />
	</c:when>
	<c:otherwise>
		<c:set var="pn" value="134" />
	</c:otherwise>
</c:choose>
<c:if test="${channel!='space' && channel!='请求' && channel!='日志终端' && channel!='相册终端' && channel!='搜索首页' && channel!='app'}">
<div class="block announcement link">
<h3>热点推荐</h3>
<div class="blockcont text">
<IFRAME id="197" MARGINHEIGHT=0 MARGINWIDTH=0 FRAMEBORDER=0 WIDTH=100% HEIGHT=16 SCROLLING=NO SRC="http://gg.${applicationScope.domain}/view.jsp?p=197">
</IFRAME>
<IFRAME id="${pn}" MARGINHEIGHT=0 MARGINWIDTH=0 FRAMEBORDER=0 WIDTH=100% HEIGHT=16 SCROLLING=NO SRC="http://gg.${applicationScope.domain}/view.jsp?p=${pn}">
</IFRAME>
<IFRAME id="${pn+1}" MARGINHEIGHT=0 MARGINWIDTH=0 FRAMEBORDER=0 WIDTH=100% HEIGHT=16 SCROLLING=NO SRC="http://gg.${applicationScope.domain}/view.jsp?p=${pn+1}">
</IFRAME>
<IFRAME id="198" MARGINHEIGHT=0 MARGINWIDTH=0 FRAMEBORDER=0 WIDTH=100% HEIGHT=16 SCROLLING=NO SRC="http://gg.${applicationScope.domain}/view.jsp?p=198">
</IFRAME>
<IFRAME id="199" MARGINHEIGHT=0 MARGINWIDTH=0 FRAMEBORDER=0 WIDTH=100% HEIGHT=16 SCROLLING=NO SRC="http://gg.${applicationScope.domain}/view.jsp?p=199">
</IFRAME>
</div>
</div>
</c:if>