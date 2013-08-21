<%@page pageEncoding="UTF-8" %>
<%--
pageStatus
如果包含这个属性，说明在状态页
pageHome
如果包含这个属性，表示在首页，仅首页显示推广位
 --%>
				<%-- 
				<span id="currentStatus_bak" class="currentStatus" style="display:none;"><c:choose><c:when test="${!empty userBasic.doing}">${userBasic.doing}</c:when><c:otherwise>什么都没做</c:otherwise></c:choose></span>
				 --%>
				 <!--span><a id="statusUpdateLink" href="javascript:void(0)">编辑</a><span class="pipe">|</span-->
				 
				<!--首页,自己的个人主页,状态页显示编辑框-->
                <c:if test="${empty guestBase||guestBase.id == host.id}">
					<div id="statusEdit" class="status-composer-bubble">
	            		<div id="statusContent" class="status-composer-bubble-inner">
							<textarea id="publisher_statusInput" class="input-text status-composer-input itsempty"></textarea>
						</div>
						<span class="status-composer-bubble-btn"><input type="submit" id="publisher_statusSubmit" class="input-button" value="发布" ${pageStatus?'':'style="display: none;"'} /></span>
					</div>
					<c:if test="${not empty pageHome && empty cookie.sta1 && not empty requestScope.recommendDoingUBB.largeLogoUrl}">
					<div class="commend-status" id="commendStatus">
						<div class="border-top"></div>
						<img src="${applicationScope.urlStatic}${requestScope.recommendDoingUBB.largeLogoUrl}" onclick="statusEditor.addEmotion('${requestScope.recommendDoingUBB.ubb}${requestScope.recommendDoingUBB.ubbDescription}', true);" />
						<a href="#nogo" onclick="statusEditor.addEmotion('${requestScope.recommendDoingUBB.ubb}${requestScope.recommendDoingUBB.ubbDescription}', true);">${requestScope.recommendDoingUBB.altDescription}</a>
					</div>
					</c:if>
					<!--<p style="position: absolute; margin-top: 5px; right: 72px;" id="hostStatus"><a target="_blank" href="http://hudong.${applicationScope.domain}/hudong/home.do?hid=1" style="color:rgb(102, 153, 0);"><img src="${applicationScope.urlSmallStatic}/imgpro/icons/512leaf.gif" style="vertical-align:text-bottom;"/> 关爱继续 情暖中国</a></p>-->

					<div id="statusCount" class="statuscount" ${not empty pageStatus?'':'style="display: none"'}>0/70</div>
					<div class="${not empty pageStatus?'collapsed':''} facecube" id="status_emotion" ${not empty pageStatus?'':'style="display: none"'}>
						<c:if test="${not empty pageStatus}">
							<div class="legend" id="status_emotion_legend"><img src="${applicationScope.urlStatic}/imgpro/icons/statusface/1.gif" /></div>
						</c:if>
						<ul id="status_emotions" ${empty pageStatus?'':'style="display: none"'}>
							<c:forEach var="item" items="${requestScope.recommendDoingUBB.duList}">
								<li><a href="#" onfocus="this.blur();"><img emotion="${item.ubb}${item.ubbDescription}" alt="${item.alt}" title="${item.alt}" src="${applicationScope.urlStatic}${item.src}"/></a></li>
							</c:forEach>
						</ul>
					</div>

				</c:if>
