<%@ page pageEncoding="utf-8" %>
<div id="friendsSideItem" class="side-item friend-list" <c:if test="${(empty requestScope.myFriendsList || fn:length(requestScope.myFriendsList)==0) && (empty requestScope.myConfirmingFriends || fn:length(requestScope.myConfirmingFriends)==0) }"> style="display:none"</c:if> >
	<div class="side-item-header clearfix">
		<h4>好友列表</h4>
	</div>
	<div class="side-item-body clearfix">
		<ul>	
			<c:if test="${!empty requestScope.myFriendsList && fn:length(requestScope.myFriendsList)>0}">
				<c:forEach varStatus="status" var="friendsItem" items="${requestScope.myFriendsList}">	
					<li>
                    	<span class="avatar">
                        	<a style="cursor:pointer;" href="${applicationScope.urlWww}/profile.do?portal=guideMF&amp;ref=guide_fl_profile&amp;id=${friendsItem.id}" title="查看${friendsItem.name}的个人主页">
                        		<img src="${friendsItem.tiny_Url}" />
                        	</a>
                        </span>
						<div class="uinfo">
							<p>
								<a href="${applicationScope.urlWww}/profile.do?portal=guideMF&amp;ref=guide_fl_profile&amp;id=${friendsItem.id}">${friendsItem.name}</a>
							</p>							
							<p>
								<a href="${applicationScope.urlWww}/profile.do?portal=guideMF&amp;ref=guide_fl_profile&amp;id=${friendsItem.id}" class="lookup">
									看TA的个人主页
								</a>
							</p>
						</div>
                    </li>
				</c:forEach>
			</c:if>				
			<c:if test="${!empty requestScope.myConfirmingFriends && fn:length(requestScope.myConfirmingFriends)>0}">
				<c:forEach varStatus="status" var="confirmingItem" items="${requestScope.myConfirmingFriends}"  begin="0" end="59">	
					<li>
                    	<span class="avatar">
                        	<a style="cursor:pointer;" href="${applicationScope.urlWww}/profile.do?portal=guideMF&amp;ref=guide_fl_profile&amp;id=${confirmingItem.id}" title="查看${friendsItem.name}的个人主页">
                        		<img src="${confirmingItem.tiny_Url}" />
                        	</a>
                        </span>
						<div class="uinfo">
							<p>
								<a href="${applicationScope.urlWww}/profile.do?portal=guideMF&amp;ref=guide_fl_profile&amp;id=${confirmingItem.id}">
									${confirmingItem.name}
								</a>
							</p>
							<p class="waiting">等TA确认</p>														
							<p>
								<a href="${applicationScope.urlWww}/profile.do?portal=guideMF&amp;ref=guide_fl_profile&amp;id=${confirmingItem.id}" class="lookup">
									看TA的个人主页
								</a>
							</p>
						</div>
                    </li>					
				</c:forEach>
			</c:if>
		</ul>
	</div>
</div>

