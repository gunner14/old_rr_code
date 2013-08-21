<%@ page pageEncoding="utf-8" %>
<div id="replyDiv" class="reply-notify news-feed" style="display:none"><%-- 异步加载 --%>
	<div class="section-header">
		<h2>新留言及回复<span class="stat" id="gossipReplyCount"></span></h2>
	</div>
	<div id="opilist" class="feed-story"></div>
</div>
<c:if test="${!empty requestScope.guideBuddyApplies || !empty requestScope.visitList || !empty requestScope.friendListWithinTime 
	|| !empty requestScope.TwoDegreesFriends ||	!empty requestScope.guideVisitList || !empty requestScope.guideAppFootprint}">
	<c:set var="guideMyRRIndex" value="0" />
	<div class="myRR">
		<div class="title">
			<c:choose>
				<c:when test="${requestScope.firstArriveGuide}"><strong>我的人人</strong> 　HI，你可以从这里容易的开始了解人人网</c:when>
				<c:otherwise><strong>我的人人</strong> 　在你没来人人这些天，需要你知道的事情。</c:otherwise>
			</c:choose>			
			<a href="javascript:;" class="open pack" id="toggleRenrenHandle" >收起</a>
		</div>
		<div id="guidePalyApp">
			<c:if test="${!empty requestScope.guideBuddyApplies}">
				<c:set var="guideMyRRIndex" value="${guideMyRRIndex+1}" />
				<div class="myRR-box requests">
					<div class="myRR-box-header">
						<span class="array">${guideMyRRIndex}</span>
						<strong>
							<a href="${applicationScope.urlReq}/request/requestList.do?nur=guide_myrr_addfriend#nonapp_position_601" target="_blank">
								<c:forEach varStatus="status" var="item" items="${requestScope.guideBuddyApplies}" begin="0" end="2">
									<c:choose>
										<c:when test="${status.index==0}">${item.name}</c:when>
										<c:otherwise>、${item.name}</c:otherwise>
									</c:choose>
								</c:forEach><c:if test="${requestScope.buddyApplyCount>3}">等</c:if>申请加你为好友(${requestScope.buddyApplyCount})
							</a>
						</strong>
						<a class="more" href="${applicationScope.urlReq}/request/requestList.do?nur=guide_myrr_addcenter#nonapp_position_601" target="_blank">
							去请求中心&raquo;
						</a>
					</div>
					<div class="myRR-box-body"></div>
				</div>
			</c:if>
			<c:if test="${!empty requestScope.visitList || !empty requestScope.friendListWithinTime}">
				<c:set var="guideMyRRIndex" value="${guideMyRRIndex+1}" />
				<div class="myRR-box friends">
					<div class="myRR-box-header">
						<span class="array">${guideMyRRIndex}</span><strong>最新好友动态</strong>
					</div>	
					<div class="myRR-box-body" id="myRR_box_body">						
						<dl class="user-list clearfix" id="guideLastFriends" >
							<dt>刚刚成为我的好友 : </dt>
							<dd id="guideUserList" class="guideUserList clearfix"></dd>
							<a href="javascript:;" class="open pack" id="guideUserListOpen" style="display:none"></a>
						</dl>
						<div id="interactive_zone" class="interactive-zone clearfix loading" style="">
						</div><!--interactive-zone end-->						
						<dl class="user-list clearfix" id="guideLastVister" style="display:none">
							<dt>最近来访 : </dt>
							<dd style="width:85%">
								<c:forEach varStatus="status" var="item" items="${requestScope.visitList}">
									<span id="conuser_${item.guest}_b">
										<a ui-info="${item.guest}"  href="javascript:;" title="${item.guestName}">
											${fn:substring(item.guestName, 0, 3)}
										</a>
									</span>
								</c:forEach>
							</dd>
						</dl>
					</div>
				</div>
			</c:if>
			<c:if test="${!empty requestScope.TwoDegreesFriends}"><c:set var="guideMyRRIndex" value="${guideMyRRIndex+1}" /></c:if>
			<div class="myRR-box SG" style="display:none" id="myknowfriend_user">
				<div class="myRR-box-header">
					<span class="array">${guideMyRRIndex}</span>
					<strong>可能认识的人推荐</strong><span>已选(<em id="myknowfriend_con_count">0</em>)</span>
	            </div>
				<div class="myRR-box-body">
					<div class="friend-selector">
						<ul id="myknowfriend_con"></ul>
					</div><!--friend-selector end-->
					<div class="add-btn"><input type="button"  id="myknowfriend_btn" class="input-submit" value="＋ 加为好友"></div>
				</div><!--myRR-box-body end-->
			</div><!--myRR-box end-->
			<c:if test="${!empty requestScope.guideVisitList || !empty requestScope.guideAppFootprint}">
				<div class="myRR-box event">
					<div class="myRR-box-header"><strong>上次登录人人</strong></div>
					<div class="myRR-box-body">
						<c:if test="${!empty requestScope.guideVisitList}">
							<p><em>•</em>访问过 : 
								<c:forEach varStatus="status" var="item" items="${requestScope.guideVisitList}" begin="0" end="5">
									<a href="${applicationScope.urlWww}/profile.do?id=${item.id}&amp;nur=guide_myrr_last_footprint" target="_blank" title="${item.name}">
										${fn:substring(item.name, 0, 3)}
									</a>&nbsp;&nbsp;&nbsp;
								</c:forEach>						
							</p>
						</c:if>
						<c:if test="${!empty requestScope.guideAppFootprint}" >
							<p class="apps"><em>•</em>玩过 : 
								<c:forEach varStatus="status" var="item" items="${requestScope.guideAppFootprint}" begin="0" end="2">
									<span style="background-image:url(${item.appIcon})">
										<a href="${item.appUrl}?nur=guide_myrr_last_apps" target="_blank">${item.appName}</a>&nbsp;&nbsp;&nbsp;
									</span>
								</c:forEach>
							</p>
							<a href="${applicationScope.urlGame}?nur=guide_myrr_last_appcenter" class="more" target="_blank">去游戏大厅»</a>
						</c:if>						
					</div>
				</div>
			</c:if>
	        <%--没接口,暂时隐藏 //bobo注
			<div class="myRR-box feed">
				<div class="myRR-box-header">这几天
					<a href="javascript:;">陈诗论</a>、
					<a href="javascript:;">刘静</a>、
					<a href="javascript:;">小新</a>更新了他们的新鲜事
					<a href="javascript:;" class="more">更多好友新鲜事&raquo;</a>
				</div>
				<div class="myRR-box-body"></div>
			</div>
	        --%>
		</div>
	</div>
</c:if>
