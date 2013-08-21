<%@ page pageEncoding="UTF-8" %>
<div id="footer">
	<c:if test="${requestScope.visiter.loginCount < 0}">
		<c:if test="${empty cleanpage}"><%@ include file="/inc/aroundSchoolAD.inc" %></c:if>
	</c:if>
	<p>
		<span class="float-right">				
			<c:if test="${applicationScope.domain != 'kaixin.com'}"><a href="${applicationScope.urlPage}/600002246">关于</a></c:if>
			<c:if test="${applicationScope.domain != 'kaixin.com'}"><span class="pipe">|</span><a href="${applicationScope.urlDev}">开放平台</a></c:if>
			<c:if test="${applicationScope.domain != 'kaixin.com'}"><span class="pipe">|</span><a href="http://mobile.${applicationScope.domain}/home.do?psf=40002">手机${domainIdentified["fullSiteName"]}</a></c:if>
			<c:if test="${applicationScope.domain != 'kaixin.com'}"><span class="pipe">|</span><a href="${applicationScope.urlProfile}/info/jobs.jsp">招聘</a>
			<span class="pipe">|</span></c:if><a href="${applicationScope.urlSupport}/GetGuestbookList.do">客服</a>
			<span class="pipe">|</span><a href="${applicationScope.urlProfile}/info/Help.do">帮助</a>
			<c:if test="${applicationScope.domain != 'kaixin.com'}"><span class="pipe">|</span><a href="${applicationScope.urlProfile}/info/PrivacyClaim.do">隐私声明</a></c:if>
			<c:if test="${applicationScope.domain != 'kaixin.com'}"><span class="pipe">|</span><a href="${applicationScope.urlProfile}/info/Link.do">友情链接</a></c:if>
			<c:if test="${applicationScope.domain != 'kaixin.com'}"><span class="pipe">|</span><a href="http://www.kaixin.com/">开心网</a></c:if>
		</span>
		千橡公司 <span title="revision$revxxx$; ${applicationScope.hostName}">&copy;</span> 2012 
		<a href="http://www.miibeian.gov.cn/" target="_blank" class="shy">京ICP证090254号</a></p>
</div>
<%--统计代码--%>
<%@ include file="stats.inc"%>
<%@ include file="chat.inc"%>
