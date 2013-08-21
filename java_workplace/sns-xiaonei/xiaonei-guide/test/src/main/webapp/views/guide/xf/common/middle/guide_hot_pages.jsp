<%@ page pageEncoding="utf-8" %>
<c:if test="${!empty requestScope.gspList}">
	<div class="pages">
		<p class="legend">加个明星当好友</p>
		<span class="more"><a href="${applicationScope.urlPage}/home/allpages/?ref=guide_page_more">更多</a></span>
		<ul>
			<c:forEach var="pageItem" items="${requestScope.gspList}" begin="0" end="2" varStatus="status">
				<li>
					<div class="figure">
						<a href="${applicationScope.urlPage}/${pageItem.pageId}/?ref=guide_page">
							<img src="${pageItem.pageHeadUrl}" width="80" />
						</a>
					</div>
					<div class="info">
						<p class="title">
							<a href="${applicationScope.urlPage}/${pageItem.pageId}/?ref=guide_page">${pageItem.pageName}</a>
							<img alt="通过人人真实身份认证" src="${applicationScope.urlStatic}/imgpro/icons/follow.gif" title="通过人人真实身份认证"/>
							<span>${pageItem.pageFansCount}好友</span>
						</p>
						<p class="state">最新状态："${pageItem.pageDoing}"</p>
						<p class="add">
							<c:choose>
								<c:when test="${emailIsConfirmed==false}">
									<a href="javascript:showConfirmDialog();">加为好友</a>													
								</c:when>
								<c:otherwise>
									<a href="${applicationScope.urlPage}/pa/bf?pid=${pageItem.pageId}&amp;ref=guide_page_add">加为好友</a>	
								</c:otherwise>
							</c:choose>					
						</p>
					</div>
				</li>							
			</c:forEach>
		</ul>
	</div>
</c:if>