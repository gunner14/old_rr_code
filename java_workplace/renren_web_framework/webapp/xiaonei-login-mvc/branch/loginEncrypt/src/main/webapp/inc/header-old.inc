<%@ page pageEncoding="UTF-8" %>
<c:choose>
	<c:when test="${requestScope.visiter.loginCount <= 6}">
		<div id="header">
			<%@ include file="/inc/logo-old.inc" %>
			<p id="tagline"><a href="${applicationScope.urlHome}/Home.do">因为真实，所以精彩</a></p>
			<c:choose>
				<c:when test="${empty requestScope.host}">
					<script type="text/javascript">
						function addBookmark(title,url) {
							if( document.all ) {
								window.external.AddFavorite( url, title);
							} else if (window.sidebar) {
								window.sidebar.addPanel(title, url,"");
							} else if( window.opera && window.print ) {
								return true;
							}
						}
					</script>
					<div id="utility"><a href="#" onclick="this.style.behavior='url(#default#homepage)';this.setHomePage('${applicationScope.urlProfile}');">设为首页</a><span class="pipe">|</span><a href="javascript:addBookmark('${domainIdentified["siteName"]} - 因为真实，所以精彩','${applicationScope.urlWww}/');">加入收藏</a><span class="pipe">|</span><a href="${applicationScope.urlMain}/info/Help.do">帮助</a></div>
				</c:when>
				<c:otherwise>
					<div id="utility"><a href="${applicationScope.urlMain}/privacyhome.do">隐私设置</a><span class="pipe">|</span><a href="${applicationScope.urlMain}/info/Help.do">帮助</a><span class="pipe">|</span><a href="${applicationScope.urlMain}/Logout.do">退出</a></div>
				</c:otherwise>
			</c:choose>
			<div id="navigation">
				<h3>主导航:</h3>
				<ul>
					<li><a href="${applicationScope.urlHome}/Home.do">首页</a></li>
					<li><a href="${applicationScope.urlBrowse}/b.do">随便看看</a></li>
					<li><a href="${applicationScope.urlBrowse}/os.do">搜索</a></li>
					<li><a href="${applicationScope.urlGroup}/RandomTribe.do">群组</a></li>
					<li><a href="${applicationScope.urlInvite}/Invite.do">邀请</a></li>
				</ul>
			</div>
		</div>	
	</c:when>
	<c:otherwise>	
		<div id="header">			
			<%@ include file="/inc/logo-old.inc" %>
			<p id="tagline"><a href="${applicationScope.urlHome}/Home.do">因为真实，所以精彩</a></p>
			<c:choose>
				<c:when test="${empty requestScope.host}">
					<script type="text/javascript">
						function addBookmark(title,url) {
							if( document.all ) {
								window.external.AddFavorite( url, title);
							} else if (window.sidebar) {
								window.sidebar.addPanel(title, url,"");
							} else if( window.opera && window.print ) {
								return true;
							}
						}
					</script>
					<div id="utility"><a href="#" onclick="this.style.behavior='url(#default#homepage)';this.setHomePage('${applicationScope.urlProfile}');">设为首页</a><span class="pipe">|</span><a href="javascript:addBookmark('${domainIdentified["siteName"]} - 因为真实，所以精彩','${applicationScope.urlWww}/');">加入收藏</a><span class="pipe">|</span><a href="${applicationScope.urlMain}/info/Help.do">帮助</a></div>
				</c:when>
				<c:otherwise>
					<div id="utility">
					<a href="${applicationScope.urlMain}/privacyhome.do">隐私设置</a><span class="pipe">|</span><a href="${applicationScope.urlMain}/info/Help.do">帮助</a><span class="pipe">|</span><a href="${applicationScope.urlMain}/Logout.do">退出</a></div>
				</c:otherwise>
			</c:choose>
			<div id="navigation">
				<h3>主导航:</h3>
				<ul>
					<li><a href="${applicationScope.urlHome}/Home.do">首页</a></li>
					<li><a href="${applicationScope.urlBrowse}/b.do">随便看看</a></li>
					<li><a href="${applicationScope.urlBrowse}/os.do">搜索</a></li>
					<c:if test="${visiter.stage == 20}">
						<li><a href="${applicationScope.urlBlog}/BlogHome.do">日志</a></li>
						<li><a href="${applicationScope.urlPhoto}/freshalbum.do">相册</a></li>
					</c:if>
					<li><a href="${applicationScope.urlGroup}/RandomTribe.do">群组</a></li>
					<c:if test="${visiter.stage == 20}">
						<li><a href="${applicationScope.urlEvent}/GetEventList.do?univ=${requestScope.visiter.univ}">活动</a></li>			
					</c:if>
					<li><a href="${applicationScope.urlInvite}/Invite.do">邀请</a></li>
		 			<li><a href="${applicationScope.urlProfile}/im.do">聊天</a></li>
		 			<c:if test="${visiter.stage == 20}">
						<li><a href="${applicationScope.urlClub}/">论坛</a></li>
					</c:if>
				</ul>
			</div>
			<%@ include file="/inc/promo-wide.inc" %>
		</div>
	</c:otherwise>
</c:choose>