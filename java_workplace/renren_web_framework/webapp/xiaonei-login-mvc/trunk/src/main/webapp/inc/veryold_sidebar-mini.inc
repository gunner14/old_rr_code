<%@ page pageEncoding="UTF-8" %>
<div id="sidebar">
	<img src="${applicationScope.urlStatic}/img/reg/lbar.gif"/>
</div>
<div id="widebar">
		<div id="utility">
		<c:if test="${empty hideLink}">
			<c:if test="${!empty invite}">
				<a href="${applicationScope.urlInvite}/Invite.do">邀请</a><span class="pipe">|</span>
			</c:if>
			<a href="#" onclick="this.style.behavior='url(#default#homepage)';this.setHomePage('http://${applicationScope.sysdomain}');">设为首页</a>
			<span class="pipe">|</span><a href="javascript:addBookmark('人人网 - 因为真实，所以精彩','http://www.${applicationScope.sysdomain}/');">加入收藏</a>
			<span class="pipe">|</span><a href="${applicationScope.urlMain}/info/Help.do">帮助</a>
		</c:if>
		</div>
		<div id="navigation"></div>