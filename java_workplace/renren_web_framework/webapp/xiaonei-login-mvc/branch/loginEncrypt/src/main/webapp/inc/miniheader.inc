<%@ page pageEncoding="UTF-8" %>
<c:set var="cleanpage" value="ture" />
<div id="header">
	<%@ include file="/inc/logo.inc" %>
	<p id="tagline"><a href="${applicationScope.urlHome}/Home.do">${domainIdentified["siteName"]} - 因为真实，所以精彩</a></p>
<%--	<c:choose>
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
	</c:choose>--%>
</div>
