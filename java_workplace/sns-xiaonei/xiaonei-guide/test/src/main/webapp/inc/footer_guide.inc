<%@ page pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<c:if test="${requestScope.isFromIM != true}">
	<div id="footer">
		<c:if test="${INC_FOOTER_NOAD != 1}">
			<div class="blank-holder">
				<c:if test="${empty cleanpage}">
					<c:if test="${requestScope.visiter.loginCount > 6 && visiter.stage != 10}">
						<%@ include file="/inc/promo-footer-wide.inc" %>
					</c:if>
				</c:if>
			</div>
		</c:if>	
		<div class="copyright">
			<span class="float-right">
				<c:if test="${applicationScope.domain != 'kaixin.com'}"><a href="${applicationScope.urlPage}/600002246">关于</a></c:if>
				<c:if test="${applicationScope.domain != 'kaixin.com'}">
					<span class="pipe">|</span><a href="${applicationScope.urlDev}">开放平台</a>
					<span class="pipe">|</span><a href="http://widget.${applicationScope.domain}">小部件</a>				
				</c:if>
				<c:if test="${applicationScope.domain != 'kaixin.com'}">
					<span class="pipe">|</span><a href="http://mobile.${applicationScope.domain}/mobilelink.do?psf=40002">手机${domainIdentified["fullSiteName"]}</a>
				</c:if>
				<c:if test="${applicationScope.domain != 'kaixin.com'}">
					<span class="pipe">|</span><a href="http://ads.${applicationScope.domain}">自助广告</a>
				</c:if>
				<c:if test="${applicationScope.domain != 'kaixin.com'}">
					<span class="pipe">|</span><a href="${applicationScope.urlProfile}/info/jobs.jsp">招聘</a><span class="pipe">|</span>
				</c:if>
				<a href="${applicationScope.urlSupport}/GetGuestbookList.do">客服</a>
				<span class="pipe">|</span><a href="${applicationScope.urlProfile}/visitor/helpcenter/helpcenter">帮助</a>
				<c:if test="${applicationScope.domain != 'kaixin.com'}">
					<span class="pipe">|</span><a href="${applicationScope.urlProfile}/info/PrivacyClaim.do">隐私声明</a>
				</c:if>
				<c:if test="${applicationScope.domain != 'kaixin.com'}">
					<span class="pipe">|</span><a href="${applicationScope.urlProfile}/info/Link.do">友情链接</a>
				</c:if>
				<c:if test="${applicationScope.domain != 'kaixin.com'}">
					<span class="pipe">|</span><a href="http://www.kaixin.com/">开心网</a>
				</c:if>
				<c:if test="${applicationScope.domain != 'renren.com'}">
					<span class="pipe">|</span><a href="http://www.renren.com/">人人网</a>
				</c:if>
				<span class="pipe">|</span><a style="color:#808080" href="http://www.miibeian.gov.cn/" target="_blank">京ICP证090254号</a>
			</span>
			<span>千橡公司 <span title="revision$revxxx$; ${applicationScope.hostName}">&copy;</span> 2012</span>
		</div>
	</div>
	<div class="hidden-area"></div>
	<c:if test="${INC_FOOTER_MODE != 0}"> 
		<c:if test="${INC_FOOTER_NOAD != 1}">
			<script src="${applicationScope.urlSmallStatic}/utm/urchin.js?ver=$revxxx$" type="text/javascript"></script>
			<script type="text/javascript">
				(function(){
					var id = XN.Cookie.get("id");
				    if( id && id.slice(-1) == "6" ){
				        _userv = 0;
				        urchinTracker();
				    }
				})();
			</script>
		</c:if>
		<%@ include file="chat.inc"%>
	</c:if>
</c:if>
