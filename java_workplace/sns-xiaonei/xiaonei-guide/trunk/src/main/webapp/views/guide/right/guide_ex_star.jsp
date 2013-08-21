<%@ page pageEncoding="utf-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>

<c:if test="${ (visiter.stage==10) && (!empty requestScope.starList) }">
	<div class="side-item high-school-star" id="highSchoolStar">
		<div class="side-item-header clearfix">
			<h4><c:if test="${empty requestScope.starNetwork.name}">本校</c:if>${requestScope.starNetwork.name}人气之星</h4>
		</div>
		<div class="side-item-body">
			<div class="star-list-holder">
				<ul class="star-list clearfix">
				<c:forEach var="item" items="${requestScope.starList}" begin="0" end="8">
					<li>
						<a href="${applicationScope.urlMain}/profile.do?portal=homeStar&amp;id=${item.id}" class="avatar">
							<img src="${item.tiny_Url}" />
						</a>
						<span class="name-holder clearfix <c:if test="${item.online}">is-online</c:if>">
							<c:if test="${item.online}">
								<c:choose>
									<c:when test="${item.vipMember}">
										<img class="on-line" src="${applicationScope.urlStatic}/imgpro/icons/vip-online.gif" width="13" height="12" title="VIP在线,点此和${item.name}聊天" onclick="javascript:talkto(${item.id}, '${item.name}');return false;" />
									</c:when>
									<c:otherwise>
										<img class="on-line" src="${applicationScope.urlStatic}/imgpro/icons/online_1.gif" width="13" height="12" title="点此和${item.name}聊天" onclick="javascript:talkto(${item.id}, '${item.name}');return false;" />
									</c:otherwise>
								</c:choose>
							</c:if>
							<a href="${applicationScope.urlMain}/profile.do?portal=homeStar&id=${item.id}" class="name" title="${item.name}">${item.name}</a>
						</span>
					</li>
				</c:forEach>
				</ul>
			</div>
			<a href="javascript:;" class="foot-forward foot-forward-disable" title="下一页">下一页</a>
			<a href="javascript:;" class="foot-back foot-back-disable" title="上一页">上一页</a>
		</div>
	</div>
</c:if>
