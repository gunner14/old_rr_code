<%@ page pageEncoding="utf-8" %>
<%@ page import="com.renren.sns.guide.view.UserTinyView"%>
<c:if test="${!empty friendBirthdayList}">
	<div id="homeBirthdayPart" >		
		<div class="side-item birthdays">
			<div class="side-item-header clearfix">
				<h4>好友生日</h4><!-- v2 -->
				<div class="option">
					<a href="${applicationScope.urlGift}/show/birth/monthly?from=guide">更多</a>
				</div>
			</div>
			<div class="side-item-body clearfix">
				<ul><c:forEach var="item" items="${requestScope.friendBirthdayList}" varStatus="status"><c:if test="${status.index <5}">
					<li>
						<span class="user">
							<a stats="Side_Birth" class="name" href="${applicationScope.urlMain}/profile.do?id=${item.id}">${item.name}</a> 
							<a href="${applicationScope.urlI}/index.action?wc=40000"><c:if test="${item.vipMember }">
								<img class="vip" src="${applicationScope.urlStatic}/imgpro/icons/vip.gif" alt="VIP" title="VIP" /></c:if></a> 
							<a class="birthday" href="${applicationScope.urlGift}/show/birth/same?month=${item.birth_month}&amp;day=${item.birth_day}&amp;from=guide">
								(<c:choose><c:when test="${item.birth_month == THIS_MONTH && item.birth_day == THIS_DATE}">今天</c:when><c:otherwise>${item.birth_month}-${item.birth_day}</c:otherwise></c:choose>)
							</a>
						</span> 								
						<span class="actions">
							<a class="gift" href="${applicationScope.urlGift}/show/gift/home?fid=${item.id}&amp;tagId=45&amp;from=guide">送礼物</a><span class="pipe">|</span><a class="leave-message" href="javascript:showFriendBirthdayWindow('${item.id}')">留言</a>
						</span>
				</li></c:if></c:forEach></ul>
			</div>
		</div>		
	</div>
</c:if>
