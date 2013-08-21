<%@ page pageEncoding="utf-8" %>
<div id="friendsSideItem" class="side-item footprint" <c:if test="${(empty requestScope.myFriendsList || fn:length(requestScope.myFriendsList)==0) && (empty requestScope.myConfirmingFriends || fn:length(requestScope.myConfirmingFriends)==0) }"> style="display:none"</c:if> >
	<div class="side-item-header clearfix">
		<h4>好友列表</h4>
	</div>
	<div class="side-item-body clearfix">
		<ul class="people-list">	
			<c:if test="${!empty requestScope.myFriendsList && fn:length(requestScope.myFriendsList)>0}">
				<c:forEach varStatus="status" var="friendsItem" items="${requestScope.myFriendsList}">	
					<li <c:if test="${friendsItem.online}"> class="online"</c:if>>
						<span class="headpichold">
							<a style="cursor:pointer;" href="${applicationScope.urlWww}/profile.do?portal=guideMF&ref=guide_fl_profile&id=${friendsItem.id}" title="查看${friendsItem.name}的个人主页">
								<img src="${friendsItem.tiny_Url}" onload="roundify(this)" />
							</a>
						</span>
						<span <c:if test="${friendsItem.online}"> class="olname clearfix"</c:if>>
							<c:if test="${friendsItem.online}">
								<img src="${applicationScope.urlStatic}/imgpro/icons/online_1.gif" width="13" height="12" title="点此和${friendsItem.name}聊天" onclick="javascript:talkto(${friendsItem.id}, '${friendsItem.name}');return false;" />
							</c:if>
							<a href="${applicationScope.urlWww}/profile.do?portal=guideMF&ref=guide_fl_profile&id=${friendsItem.id}">${friendsItem.name}</a>
						</span>						
					</li>
				</c:forEach>
			</c:if>				
			<c:if test="${!empty requestScope.myConfirmingFriends && fn:length(requestScope.myConfirmingFriends)>0}">
				<c:forEach varStatus="status" var="confirmingItem" items="${requestScope.myConfirmingFriends}" >	
					<li class="confirming">
						<span class="headpichold">
							<a style="cursor:pointer;" href="${applicationScope.urlWww}/profile.do?portal=guideMF&ref=guide_fl_profile&id=${confirmingItem.id}" title="查看${confirmingItem.name}的个人主页">
								<img src="${confirmingItem.tiny_Url}" onload="roundify(this)" />
							</a>
						</span>
						<span class="confirmed" style="cursor:pointer;" onclick="location.href='${applicationScope.urlWww}/profile.do?portal=guidecon&id=${confirmingItem.id}'">
							等TA确认
						</span>
						<span class="clearfix">
							<a href="${applicationScope.urlWww}/profile.do?portal=guideMF&ref=guide_fl_profile&id=${confirmingItem.id}">${confirmingItem.name}</a>
						</span>						
					</li>
				</c:forEach>
			</c:if>
		</ul>
	</div>
</div>

