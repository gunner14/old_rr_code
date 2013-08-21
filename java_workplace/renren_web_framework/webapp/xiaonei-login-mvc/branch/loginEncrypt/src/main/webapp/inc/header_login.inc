<%@ page pageEncoding="UTF-8" %>
<div id="header">
	<%@ include file="/inc/logo.inc" %>
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
			<div id="utility">你好，<a href="${applicationScope.urlMain}/BasicInfo.do">${requestScope.visiter.name}</a><span class="pipe">|</span><a href="${applicationScope.urlMain}/info/Help.do">帮助</a><span class="pipe">|</span><a href="${applicationScope.urlMain}/Logout.do">退出</a></div>
		</c:otherwise>
	</c:choose>
	<div id="navigation">
		<h3>主导航:</h3>
		<ul>
			<li><a href="${applicationScope.urlHome}/Home.do">首页</a></li>
			<li><a href="${applicationScope.urlBrowse}/SocialNet.do">随便看看</a></li>
			<li><a href="${applicationScope.urlBrowse}/Search.do">搜索</a></li>
			<li><a href="${applicationScope.urlSchool}/university.do?id=${requestScope.visiter.univ}">学校</a></li>
			<li><a href="${applicationScope.urlBlog}/BlogHome.do">日志</a></li>
			<li><a href="${applicationScope.urlPhoto}/freshalbum.do">相册</a></li>
			<li><a href="${applicationScope.urlGroup}/RandomTribe.do">群组</a></li>
			<li><a href="${applicationScope.urlEvent}/GetEventList.do?univ=${requestScope.visiter.univ}">活动</a></li>			
			<li><a href="${applicationScope.urlInvite}/Invite.do">邀请</a></li>
 			<li><a href="${applicationScope.urlProfile}/im.do">聊天</a></li>
			<li><a href="${applicationScope.urlClub}/external_rk.php">${domainIdentified["siteName"]}广场</a></li>
		</ul>
	</div>
	
</div>
