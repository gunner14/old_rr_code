<div class="m-composer">	
	<div class="m-composer-header">
		<div class="headerimg 
			<c:if test="${visiter.headUrl == '0/0/ban_head.jpg'}">no-headerimg</c:if>" 
			<c:if test="${visiter.headUrl == '0/0/ban_head.jpg'}">title="请上传你的头像,更好的享受${domainIdentified["siteName"]}功能"</c:if>
			<c:choose>
				<c:when test="${visiter.selected==1}">	
					onclick="javascript:window.location.href='${applicationScope.urlPhoto}/getalbumprofile.do?owner=${userBasic.id}'" 	
				</c:when>
				<c:otherwise>
					onclick="javascript:window.location.href='http://head.upload.${applicationScope.domain}/Upload.do'" 
				</c:otherwise>
			</c:choose>		
			style="cursor:pointer; background-image:url(${userBasic.head_Url})">		
		</div>
		<div class="statuspanel">
			<span class="myname">${userBasic.name}
				<c:if test="${!empty userCache && userCache.vipMember}">
					<a  alt="紫豆会员在线" href="http://i.${applicationScope.domain}"><img alt="紫豆会员在线" src="${applicationScope.urlSmallStatic}/imgpro/icons/vip_online_s.gif"/></a>
				</c:if>
			</span>
				<span id="currentStatus" class="currentStatus"><c:choose><c:when test="${!empty userBasic.doing2}">${userBasic.doing2}</c:when><c:otherwise>什么都没做</c:otherwise></c:choose></span><span id="currentStatus_bak" class="currentStatus" style="display:none;"><c:choose><c:when test="${!empty userBasic.doing}">${userBasic.doing}</c:when><c:otherwise>什么都没做</c:otherwise></c:choose></span><span id="statusUpdateTime" class="changetime"><c:choose>
				 <c:when test="${requestScope.doneMinutes < 1}">刚刚更新</c:when>
					<c:when test="${requestScope.doneMinutes < 60}"><fmt:formatNumber value="${requestScope.doneMinutes}" pattern="#"/>分钟前</c:when>
					<c:when test="${requestScope.doneMinutes < 1440}"><fmt:formatNumber value="${requestScope.doneMinutes/60}" pattern="#"/>小时前</c:when>
					<c:when test="${requestScope.doneMinutes < 4320}"><fmt:formatNumber value="${requestScope.doneMinutes/1440}" pattern="#"/>天前</c:when>
					<c:otherwise>3天前</c:otherwise>
				 </c:choose></span><!--span><a id="statusUpdateLink" href="javascript:void(0)">编辑</a><span class="pipe">|</span--><a href="${applicationScope.urlProfile}/getdoing.do">更多</a></span>
				<div class="status-composer-bubble">
            	<div class="status-composer-bubble-inner">
            		<input id="publisher_statusInput" type="text" class="input-text status-composer-input itsempty" value="" /> 
                </div><span class="status-composer-bubble-btn"><input style="display:none;" type="submit" id="publisher_statusSubmit" class="input-button" value="提交" /></span>
            </div>
		</div>
		<%@ include file="/inc/publisher/publisher_infoFin.inc"%>
	</div>
	<div id="publisherModule" class="m-composer-content" style="display:none;">
	</div>
	<!--module composer end-->
</div>
