<%@ page pageEncoding="UTF-8" %>
</div>
<div id="footer">
	<p>
		<c:if test="${empty hideLink}">
			<span class="float-right">
				<a href="${applicationScope.urlMain}/info/About.do">关于</a>
				<span class="pipe">|</span>
				<a href="${applicationScope.urlMain}/info/jobs.jsp">招聘</a>
				<span class="pipe">|</span>
				<a href="http://supprot.${applicationScope.sysdomain}/GetGuestbookList.do">客服</a>
				<span class="pipe">|</span>
				<a href="${applicationScope.urlMain}/info/Help.do">帮助</a>
				<span class="pipe">|</span>
				<a href="${applicationScope.urlMain}/info/PrivacyClaim.do">隐私声明</a>
			</span>
		</c:if>
		千橡公司
		<span title="revision$revxxx$; ${applicationScope.hostName}">&copy;</span> 2008
	</p>
</div>
<script src="${applicationScope.urlSmallStatic}/utm/urchin.js" type="text/javascript"></script>
<script type="text/javascript"> 
   _userv=0; 
   urchinTracker(); 
</script> 