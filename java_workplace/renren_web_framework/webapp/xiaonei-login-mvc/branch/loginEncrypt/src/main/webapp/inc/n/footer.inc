<%@ page pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<c:if test="${requestScope.isFromIM != true}">
	<c:if test="${!NO_FOOTER_LINKS}">
	<div id="footer">
		<c:if test="${!KAIXIN && requestScope.visiter.loginCount > 6 && visiter.stage != 10}">
			<c:set var="footerAd1">
				<c:choose>
					<c:when test="${channel=='首页'}">ad1000000041</c:when>
					<c:when test="${channel!='space' && channel!='app'}" >ad1000000007</c:when>
				</c:choose>
			</c:set>
			<c:set var="footerAd2">
				<c:choose>
					<c:when test="${channel=='首页'}">ad1000000042</c:when>
					<c:when test="${channel!='space' && channel!='app'}" >ad1000000008</c:when>
				</c:choose>
			</c:set>	
		</c:if>	
		<div class="site-footer" style="background:#fff;position:relative;">
			<div class="main">
				<ul class="link">
					<li><a href="${applicationScope.urlWww}/siteinfo/about">关于</a></li>
					<li><a href="${applicationScope.urlDev}">开放平台</a></li>
					<li><a href="http://wan.renren.com">人人游戏</a></li>
					<li><a href="${applicationScope.urlPage}/register/regGuide/">公共主页</a></li>
					<li><a href="http://mobile.${applicationScope.domain}/mobilelink.do?psf=40002">手机人人</a></li>
					<li><a href="http://www.nuomi.com">团购</a></li>
					<li><a href="http://www.jiexi.com">皆喜网</a></li>
					<li><a href="http://ads.${applicationScope.domain}">营销服务</a></li>
					<li><a href="http://job.renren-inc.com/">招聘</a></li>
					<li><a href="${applicationScope.urlSupport}/helpcenter">客服帮助</a></li>
					<li><a href="${applicationScope.urlWww}/siteinfo/privacy">隐私</a></li>
				</ul>
				<span class="copyright">
					人人网<span title="revision$revxxx$; ${applicationScope.hostName}">&copy;</span><%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>
				</span>
			</div>
			<div class="haoes">
				<span class="company-name">北京千橡网景科技发展有限公司：</span>
				<span>文网文[2009]169号</span><span class="dot">·</span><a href="http://www.miibeian.gov.cn/" target="_blank">京ICP证090254号</a><span class="dot">·</span><span>京公网安备110000000009号</span><span class="dot">·</span><span>甲测资字11002066</span><span class="dot">·</span><span>文化部监督电子邮箱：wlwh@vip.sina.com</span><span class="dot">·</span><a href="http://a.xnimg.cn/n/core/res/certificate.jpg" target="_blank">互联网药品信息服务资格证</a><br>
				<p class="special-add"><span>文明办网文明上网举报电话：4000803580 举报邮箱：admin1@renren-inc.com</span>&nbsp;&nbsp;&nbsp;&nbsp;<a href="http://www.bjjubao.org/" target="_blank"><img src="http://s.xnimg.cn/imgpro/civilization/c_1.jpg" width="25" height="24">不良信息举报中心</a>&nbsp;&nbsp;&nbsp;&nbsp;<a href="http://www.bj.cyberpolice.cn/index.jsp" target="_blank"><img src="http://s.xnimg.cn/imgpro/civilization/c_2.jpg" width="25" height="24">网络110报警服务</a>&nbsp;&nbsp;&nbsp;&nbsp;<a href="http://www.bjwhzf.gov.cn/accuse.do" target="_blank"><img src="http://s.xnimg.cn/imgpro/civilization/c_3.jpg" width="25" height="24">北京文化市场举报热线</a>
			</div>
		</div>
	</div>
	</c:if>
</c:if>
<c:if test="${!NO_EXTRA_DIV}"></div></c:if></div><%--container的结尾--%>
<%--webpager部分--%>
<c:if test="${!empty requestScope.host && !INC_NO_WEBPAGER}">
	<%@ include file="/inc/chat-v6.inc"%>
</c:if>
<%--webpager部分结束--%>
<c:if test="${!noBackTool}">
	<div id="toolBackTo" class="back-to" style="display:none;">
		<a stats="site_footer_back_to_top" class="backtotop" href="#top" onclick="if(Sizzle('#sidebar2 .ready-to-fix')[0]) Sizzle('#sidebar2 .ready-to-fix')[0].style.position = 'static';window.scrollTo(0,0);if(Sizzle('#sidebar2 .ready-to-fix')[0]) Sizzle('#sidebar2 .ready-to-fix')[0].style.position = '';return false;">返回顶部
			<img stats="site_footer_back_to_top" src="http://a.xnimg.cn/imgpro/button/back-tip.png" class="back-tip" />
		</a>
	</div>
	<script type="text/javascript">
		XN.dom.ready(function(){
			var odom;
			object.use('dom', function(dom){
				odom = dom;
			});
			var bt = $('toolBackTo'),
				hl = XN.browser.WebKit ? (Sizzle('body')[0]) : (Sizzle('html')[0]);
			var winWidth =  window.innerWidth || document.body.clientWidth;
			var nav = Sizzle('.navigation')[0];
			if(winWidth >= 1120){
				bt.style.left = XN.element.getPosition(nav).left + 990 +'px';
			}else{
				bt.style.right = '10px';
			}
			window.addEvent('changeLayout', function(e){
				if (e.layout) {
					bt.style.left = '';
					bt.style.right = '210px';
				} else {
					winWidth =  window.innerWidth || document.body.clientWidth;
					if(winWidth >= 1120){
						bt.style.right = '';
						bt.style.left = XN.element.getPosition(nav).left + 990 +'px';
					}else{
						bt.style.left = '';
						bt.style.right = '10px';
					}
				}
			});
			XN.event.addEvent(window, 'scroll', function(e){
				var st = hl.scrollTop,
					vh = hl.clientHeight,
					show = (st>30);
				if( show ){
					bt.style.display = '';
				}else{
					bt.style.display = 'none';
				}
			});
			var	pagerCon = odom.id('navigation-for-pager');
			XN.event.addEvent(window, 'resize', function(e){
				if ((pagerCon && pagerCon.hasClass('fixWidthForPager')) || (XN.smartyBuddy && XN.smartyBuddy.frameLayout())) {
					bt.style.right = '210px';
					bt.style.left = '';
				} else {
					winWidth =  window.innerWidth || document.body.clientWidth;
					if(winWidth >= 1120){
						//timeline页面ie7、8、9下，取回来的left值为0，所以加一个setTimeout
						setTimeout(function(){
							bt.style.right = '';
							bt.style.left = XN.element.getPosition(nav).left + 990 +'px';
						}, 0);
					}else{
						bt.style.left = '';
						bt.style.right = '10px';
					}
				}
			});
		});
	</script>
</c:if>
<%@ include file="/inc/n/photoStatic.inc"%>
<%@ include file="/inc/pingIDC.inc"%>
<%--统计--%>
<%@ include file="/inc/stats.inc"%>
