<%@ page pageEncoding="utf-8" %>
<c:if test="${domainIdentified['sys.domain']=='kaixin.com'}">
	<c:if test="${!empty requestScope.playedAppList}">
		<div class="app-box">
			<c:if test="${!empty requestScope.playedAppList}">
				<div class="common-app clearfix">
					<h1>你可能喜欢的应用<span class="open" onclick="$('guidePalyApp').toggleClass('hide');if($('guidePalyApp').hasClassName('hide')) this.innerHTML='展开';else this.innerHTML='收起'">收起</span></h1>
					<ul id="guidePalyApp" class="">
						<c:forEach begin="0" end="9" var="item" items="${requestScope.playedAppList}" varStatus="status">
							<li>
								<c:choose>
								    <c:when test="${fn:contains(item.appUrl,'?')}"> 
								    	<a href="${item.appUrl}&amp;ref=kaixin_nearapp_guide_${status.index}<c:if test="${item.hotApp}">_hot</c:if>" class="appIcons"><img src="${item.appLogo}" alt="${item.appName}"></a>
								    	<strong><a href="${item.appUrl}&amp;ref=kaixin_nearapp_guide_${status.index}<c:if test="${item.hotApp}">_hot</c:if>">${item.appName}</a></strong>
								    </c:when>
								    <c:otherwise>
								    	<a href="${item.appUrl}?ref=kaixin_nearapp_guide_${status.index}<c:if test="${item.hotApp}">_hot</c:if>" class="appIcons"><img src="${item.appLogo}" alt="${item.appName}"></a>
								    	<strong><a href="${item.appUrl}?ref=kaixin_nearapp_guide_${status.index}<c:if test="${item.hotApp}">_hot</c:if>">${item.appName}</a></strong>
								    </c:otherwise>
							    </c:choose>		
								<span>${item.lastAppTimeString}</span>								
								<c:if test="${item.hotApp}">
									<img src="${applicationScope.urlStatic}/imgpro/guide/KX_hot.gif" class="hot">
								</c:if>
							</li>
						</c:forEach>
					</ul>
				</div>
			</c:if>
			<c:if test="${!empty recAppList}">
				<div class="hot-app clearfix">
					<h1>热门推荐</h1>
					<c:forEach var="item" items="${requestScope.recAppList}" varStatus="status">
						<div class="hot-main">
							<c:choose>
							    <c:when test="${fn:contains(item.appUrl,'?')}"> 
							    	<a href="${item.appUrl}&amp;ref=kaixin_hotapp_guide_${status.index}"><img src="${item.appLogo}" alt="${item.appName}"></a>
									<p><a href="${item.appUrl}&amp;ref=kaixin_hotapp_guide_${status.index}"><strong>${item.appName}</strong></a><br>总用户${item.appTotalCount}人<br><c:if test="${item.appFriendCount >= 3}">${item.appFriendCount}个好友在玩</c:if></p>
									<a href="${item.appUrl}&amp;ref=kaixin_hotapp_guide_${status.index}" class="join-btn">立刻加入</a>
							    </c:when>
							    <c:otherwise>
							    	<a href="${item.appUrl}?ref=kaixin_hotapp_guide_${status.index}"><img src="${item.appLogo}" alt="${item.appName}"></a>
									<p><a href="${item.appUrl}?ref=kaixin_hotapp_guide_${status.index}"><strong>${item.appName}</strong></a><br>总用户${item.appTotalCount}人<br><c:if test="${item.appFriendCount >= 3}">${item.appFriendCount}个好友在玩</c:if></p>
									<a href="${item.appUrl}?ref=kaixin_hotapp_guide_${status.index}" class="join-btn">立刻加入</a>
							    </c:otherwise>
						    </c:choose>	
						</div>
					</c:forEach>
				</div>
			</c:if>
		</div>
	</c:if>
</c:if>