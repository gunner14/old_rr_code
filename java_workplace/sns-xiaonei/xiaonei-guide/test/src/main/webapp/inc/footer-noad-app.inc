<%@ page pageEncoding="UTF-8" %>
				</div>
			</div>
		</div>
	</div>
	<c:if test="${requestScope.isFromIM != true}">
	<div id="footer">
		<div class="copyright">
		  <%--是否取消链接--%>
		 <c:if test="${!hideFooterLink}">
			<span class="float-right">
				<%--<a href="${applicationScope.urlPage}/600002100">${domainIdentified["siteName"]}日志</a><span class="pipe">|</span>--%>
				<c:if test="${applicationScope.domain != 'kaixin.com'}"><a href="http://www.oakpacific.com/">关于</a></c:if>
				<c:if test="${applicationScope.domain != 'kaixin.com'}">
					<span class="pipe">|</span><a href="${applicationScope.urlDev}">开放平台</a>	
					<span class="pipe">|</span><a href="http://widget.${applicationScope.domain}">小部件</a>	
				</c:if>
				<c:if test="${applicationScope.domain != 'kaixin.com'}"><span class="pipe">|</span><a href="http://mobile.${applicationScope.domain}/mobilelink.do?psf=40002">手机${domainIdentified["fullSiteName"]}</a></c:if>
				<c:if test="${applicationScope.domain != 'kaixin.com'}"><span class="pipe">|</span><a href="http://ads.${applicationScope.domain}">自助广告</a></c:if>
				<c:if test="${applicationScope.domain != 'kaixin.com'}"><span class="pipe">|</span><a href="http://job.oakpacific.com/">招聘</a>
				<span class="pipe">|</span></c:if><a href="${applicationScope.urlSupport}/GetGuestbookList.do">客服</a>
				<span class="pipe">|</span><a href="${applicationScope.urlSupport}/visitor/helpcenter">帮助</a>
				<c:if test="${applicationScope.domain != 'kaixin.com'}"><span class="pipe">|</span><a href="${applicationScope.urlProfile}/info/PrivacyClaim.do">隐私声明</a></c:if>
				<c:if test="${applicationScope.domain != 'kaixin.com'}"><span class="pipe">|</span><a href="http://www.nuomi.com/">糯米网</a></c:if>
				<span class="pipe">|</span><a style="color:#808080" href="http://www.miibeian.gov.cn/" target="_blank">京ICP证090254号</a>
			</span>
		</c:if>
		<span>千橡公司 <span title="revision$revxxx$; ${applicationScope.hostName}">&copy;</span> 2012</span>
	</div>
</div>
<div class="hidden-area"></div>
<%--是否取消webpager--%>
<script src="${applicationScope.urlSmallStatic}/utm/urchin.js" type="text/javascript"></script>
<script type="text/javascript">
(function()
{
	var id = XN.Cookie.get("id");
    if( id && id.slice(-1) == "6" )
    {
        _userv = 0;
        urchinTracker();
    }
})();
</script>
<!-- Begin comScore Tag -->
<script src="${applicationScope.urlSmallStatic}/a12023/jspro/beacon.js" type="text/javascript"></script>
<script>
  COMSCORE.beacon({
    c1:2,
    c2:6934070,
    c3:"",
    c4:"",
    c5:"",
    c6:"",
    c15:""
  });
</script>
<noscript>
  <img src="http://b.scorecardresearch.com/p?c1=2&c2=6934070&c3=&c4=&c5=&c6=&c15=&cj=1" />
</noscript>
<!-- End comScore Tag -->
<c:if test="${!noWebPager}">
	<%@ include file="chat.inc"%>
</c:if>

</c:if>
