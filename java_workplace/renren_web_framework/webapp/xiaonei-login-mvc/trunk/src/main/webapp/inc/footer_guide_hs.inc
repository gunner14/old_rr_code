<%@ page pageEncoding="UTF-8" %>
				</div>
				<div class="blank-bar">
					<c:if test="${requestScope.visiter.loginCount > 6}">
							<c:if test="${visiter.stage == 10}">
							<div class="ad-bar">
								<%@ include file="/inc/promo-activity-highschool.inc" %>
								<%@ include file="/inc/promo-hotcommend-highschool.inc" %>
							</div>
							</c:if>
					</c:if>
				</div>
			</div>
		</div>
	</div>
	<div id="footer">
		<div class="blank-holder">
			<c:if test="${empty cleanpage}">
				<c:if test="${requestScope.visiter.loginCount > 6}">
					<c:if test="${visiter.stage == 10}">
						
					</c:if>
					<c:if test="${visiter.stage == 20}">
						<%@ include file="/inc/promo-footer-wide.inc" %>
					</c:if>
					<c:if test="${visiter.stage == 30}">
						<%--
						<%@ include file="/inc/promo-footer-wide-workplace.inc" %>
						--%>
					</c:if>
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
				<c:if test="${applicationScope.domain != 'kaixin.com'}"><span class="pipe">|</span><a href="http://www.kaixin.com/">开心网</a></c:if>
			</span>
			<span>千橡公司 <span title="revision$revxxx$; ${applicationScope.hostName}">&copy;</span> 2012</span>
		</div>
	</div>
	<div class="hidden-area"></div>
	<%@ include file="chat.inc"%>
<script src="${applicationScope.urlSmallStatic}/utm/urchin.js" type="text/javascript">
</script> 
<script type="text/javascript"> 
   _userv=0; 
   urchinTracker(); 
</script>
