<%@ page pageEncoding="GBK" %>
<div id="footer">
	<p><%--<a href="${applicationScope.urlPage}/600002100">${domainIdentified["siteName"]}日志</a><span class="pipe">|</span>--%><a href="${applicationScope.urlMain}/info/Help.do">帮助</a><span class="pipe">|</span><a href="${applicationScope.urlMain}/GetGuestbookList.do">给管理员留言</a><c:if test="${applicationScope.domain != 'kaixin.com'}"><span class="pipe">|</span><a href="${applicationScope.urlMain}/info/About.do">关于本站</a></c:if><span class="pipe">|</span><a href="${applicationScope.urlMain}/info/PrivacyClaim.do">隐私声明</a></p>
	<p>千橡公司 <a href="http://www.miibeian.gov.cn/" class="shy">京ICP证090254号</a> <span title="revisionXXXX; ${applicationScope.hostName}">&copy;</span> 1997-2009 All Rights Reserved.</p>
</div>