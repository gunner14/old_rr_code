<%@ page pageEncoding="UTF-8" %>
				<div class="blank-bar">
					<c:if test="${empty cleanpage}">
						<c:if test="${requestScope.visiter.loginCount > 6}">
								<c:if test="${visiter.stage == 10}">
								<div class="ad-bar">
									<%@ include file="/inc/promo-narrow-highschool.inc" %>
									<%@ include file="/inc/promo-activity-highschool.inc" %>
									<%@ include file="/inc/promo-hotcommend-highschool.inc" %>
								</div>
								</c:if>
								<c:if test="${visiter.stage == 20}">
								<div class="ad-bar">
									<%@ include file="/inc/promo-narrow-new.inc" %>
									<%@ include file="/inc/promo-site-ad.inc" %>		
									<%@ include file="/inc/promo-narrow2-new.inc" %>
									<%@ include file="/inc/promo-hotcommend.inc" %>
									<%@ include file="/inc/promo-activity.inc" %>
								</div>
								</c:if>
								<c:if test="${visiter.stage == 30}">
								<div class="ad-bar">
									<%@ include file="/inc/promo-narrow-new.inc" %>
									<%--<%@ include file="/inc/promo-hotcommend-workplace.inc" %>--%>
									<%@ include file="/inc/promo-narrow2-new.inc" %>
								</div>
								</c:if>
						</c:if>
					</c:if>
				</div>
			</div>
		</div>
	</div>
	<div id="footer">
		<div class="blank-holder">
			<c:if test="${empty cleanpage}">
				<c:if test="${requestScope.visiter.loginCount > 6 && visiter.stage != 10}">
						<%@ include file="/inc/promo-footer-wide.inc" %>
				</c:if>
			</c:if>
		</div>
		<div class="copyright">
			<span class="float-right">
				<%--<a href="${applicationScope.urlPage}/600002100">${domainIdentified["siteName"]}日志</a><span class="pipe">|</span>--%>
				<c:if test="${applicationScope.domain != 'kaixin.com'}"><a href="${applicationScope.urlPage}/600002246">关于</a></c:if>
				<c:if test="${applicationScope.domain != 'kaixin.com'}"><span class="pipe">|</span><a href="${applicationScope.urlDev}">开放平台</a></c:if>
				<c:if test="${applicationScope.domain != 'kaixin.com'}"><span class="pipe">|</span><a href="http://mobile.${applicationScope.domain}/home.do?psf=40002">手机${domainIdentified["fullSiteName"]}</a></c:if>
				<c:if test="${applicationScope.domain != 'kaixin.com'}"><span class="pipe">|</span><a href="${applicationScope.urlProfile}/info/jobs.jsp">招聘</a>
				<span class="pipe">|</span></c:if><a href="${applicationScope.urlSupport}/GetGuestbookList.do">客服</a>
				<span class="pipe">|</span><a href="${applicationScope.urlProfile}/info/Help.do">帮助</a>
				<c:if test="${applicationScope.domain != 'kaixin.com'}"><span class="pipe">|</span><a href="${applicationScope.urlProfile}/info/PrivacyClaim.do">隐私声明</a></c:if>
				<c:if test="${applicationScope.domain != 'kaixin.com'}"><span class="pipe">|</span><a href="${applicationScope.urlProfile}/info/Link.do">友情链接</a></c:if>
				<c:if test="${applicationScope.domain != 'kaixin.com'}"><span class="pipe">|</span><a href="http://www.kaixin.com/">开心网</a></c:if><span class="pipe">|</span><a style="color:#808080" href="http://www.miibeian.gov.cn/" target="_blank">京ICP证090254号</a>
			</span>
			<span>千橡公司 <span title="revision$revxxx$; ${applicationScope.hostName}">&copy;</span> 2012</span>
		</div>
	</div>
	<div class="hidden-area"></div>
	<%@ include file="chat.inc"%>
<script src="${applicationScope.urlSmallStatic}/utm/urchin.js" type="text/javascript"></script>
<script type="text/javascript">
load_jebe_ads(1);<%-- shiyan广告 --%>
(function()
{
    var p = 1;
    if( parseInt( Math.random() * ( 10 - p ) ) + 1 == 1 )
    {
        _userv = 0;
        urchinTracker();
    }
})();
</script>
