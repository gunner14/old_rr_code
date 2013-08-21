
<div class="page-title">
	<h2>
		我的好友
	</h2>
	<c:if test="${friendIndex == 0}">
		<input class="float-right inputtext" type="text" id="query_name" name="query_name" style="margin:0 10px 10px; width:105px" class="inputtext" value="搜索你的好友"  />
		<span class="subtitle">好友列表</span><span class="subtitle" id="sub2">全部好友</span><span id="loading" class="loading" style="display:none;"><img height="10" src="${applicationScope.urlSmallStatic}/imgpro/bg/indicator_blue_small.gif"/></span>
	</c:if>
	<c:if test="${friendIndex == 1}">
		<span class="subtitle">寻找好友</span>
	</c:if>
	<c:if test="${friendIndex == 2}">
		<span class="subtitle">特别好友</span>
	</c:if>
	<c:if test="${friendIndex == 4}">
		<span class="subtitle">你可能认识的人</span>
	</c:if>
</div>

<div id="sub-nav" class="narrow noborder_bottom nomargin">
	<ul>
		<li <c:if test="${friendIndex == 0}">class="selected"</c:if>><a onfocus="this.blur()" href="${applicationScope.urlFriend}/myfriendlistx.do">我的好友</a></li>
		<li <c:if test="${friendIndex == 1}">class="selected"</c:if>><a onfocus="this.blur()" href="${applicationScope.urlInvite}/cfindfriends.do?action=find">寻找好友</a></li>
		<li <c:if test="${friendIndex == 2}">class="selected"</c:if>><a onfocus="this.blur()" href="${applicationScope.urlFriend}/myhotfriendlist.do">特别好友</a></li>
		<li <c:if test="${friendIndex == 4}">class="selected"</c:if>><a onfocus="this.blur()" href="${applicationScope.urlFriend}/myrelationperson.do">你可能认识的人</a></li>
		<li class="showmodel"><a href="${applicationScope.urlInvite}/Invite.do">邀请朋友加入</a></li>
	</ul>
</div>
