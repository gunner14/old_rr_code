<%@ page pageEncoding="utf-8" %>
<div class="side-item friend-list guide-x5" id="friendsSideItem" <c:if test="${(empty requestScope.myFriendsList || fn:length(requestScope.myFriendsList)==0) && (empty requestScope.myConfirmingFriends || fn:length(requestScope.myConfirmingFriends)==0) }"> style="display:none"</c:if> >
	<div class="side-item-header clearfix">
	    <h4>好友列表</h4>
	</div>
	<div class="side-item-body clearfix">
		<ul class="people-list">
			<c:if test="${!empty requestScope.myFriendsList && fn:length(requestScope.myFriendsList)>0}">
				<c:forEach varStatus="status" var="friendsItem" items="${requestScope.myFriendsList}">	
					<li>
  						<span class="headpichold">
							<a style="cursor:pointer;" href="${applicationScope.urlWww}/profile.do?portal=guideMF&amp;ref=guide_fl_profile&amp;pma=p_guide_m_friendlist_a_head&id=${friendsItem.id}" title="查看${friendsItem.name}的个人主页">
								<img src="${friendsItem.tiny_Url}" onload="roundify(this);"/>
							</a>
						</span>
						<span class="name">
							<a href="${applicationScope.urlWww}/profile.do?portal=guideMF&amp;ref=guide_fl_profile&amp;pma=p_guide_m_friendlist_a_name&amp;id=${friendsItem.id}">${friendsItem.name}</a>
						</span>						
                    </li>
				</c:forEach>
			</c:if>
			<c:if test="${!empty requestScope.myConfirmingFriends && fn:length(requestScope.myConfirmingFriends)>0}">
				<c:forEach varStatus="status" var="confirmingItem" items="${requestScope.myConfirmingFriends}">	
					<li>
  						<span class="headpichold">
							<a style="cursor:pointer;" href="${applicationScope.urlWww}/profile.do?portal=guideMF&amp;ref=guide_fl_profile&amp;pma=p_guide_m_friendlist_a_head&id=${confirmingItem.id}" title="查看${confirmingItem.name}的个人主页">
								<img src="${confirmingItem.tiny_Url}" onload="roundify(this);"/>
							</a>
						</span>
						<span class="name">
							<a href="${applicationScope.urlWww}/profile.do?portal=guideMF&amp;ref=guide_fl_profile&amp;pma=p_guide_m_friendlist_a_name&amp;id=${confirmingItem.id}">${confirmingItem.name}</a>
						</span>
						<span class="wait">等TA确认</span>						
                    </li>
				</c:forEach>
			</c:if>
		</ul>
	</div>
</div>
