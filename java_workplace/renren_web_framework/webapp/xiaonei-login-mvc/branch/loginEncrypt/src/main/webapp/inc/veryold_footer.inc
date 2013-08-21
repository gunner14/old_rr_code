<%@ page pageEncoding="UTF-8" %>
</div>
<div id="adbar">
	<c:if test="${empty cleanpage}">
		<c:if test="${requestScope.host.loginCount > 6}">
			<c:if test="${host.stage == 10}">
				<%@ include file="/inc/promo-narrow-highschool.inc" %>
				<%@ include file="/inc/promo-activity-highschool.inc" %>
				<%@ include file="/inc/promo-hotcommend-highschool.inc" %>
			</c:if>	
			<c:if test="${host.stage == 20}">
				<%@ include file="/inc/promo-site-ad.inc" %>		
				<%@ include file="/inc/promo-narrow-new.inc" %>
				<%@ include file="/inc/promo-hotcommend.inc" %>
				<%@ include file="/inc/promo-activity.inc" %>
				<%@ include file="/inc/promo-narrow2-new.inc" %>
			</c:if>	
			<c:if test="${host.stage == 30}">
				<%@ include file="/inc/promo-narrow-workplace.inc" %>
				<%@ include file="/inc/promo-hotcommend-workplace.inc" %>
				<%@ include file="/inc/promo-narrow2-workplace.inc" %>
			</c:if>
		</c:if>
	</c:if>
</div>
<div id="footer">
	<p>
		<span class="float-right">
			<a href="${applicationScope.urlMain}/info/About.do">关于</a>
			<span class="pipe">|</span>
			<a href="${applicationScope.urlDev}/developers/portal.do">开放平台</a>
			<span class="pipe">|</span>
			<a href="${applicationScope.urlMain}/info/jobs.do">招聘</a>
			<span class="pipe">|</span>
			<a href="http://poll.${applicationScope.sysdomain}">调查服务</a>
			<span class="pipe">|</span>
			<a href="http://supprot.${applicationScope.sysdomain}/GetGuestbookList.do">客服</a>
			<span class="pipe">|</span>
			<a href="${applicationScope.urlMain}/info/Help.do">帮助</a>
			<span class="pipe">|</span>
			<a href="${applicationScope.urlMain}/info/PrivacyClaim.do">隐私声明</a>
		</span>千橡公司 
		<span title="revision$revxxx$; ${applicationScope.hostName}">&copy;</span> 2008
	</p>
</div>
<script src="${applicationScope.urlSmallStatic}/utm/urchin.js" type="text/javascript"></script> 
<script type="text/javascript"> 
   _userv=0; 
   urchinTracker(); 
</script> 