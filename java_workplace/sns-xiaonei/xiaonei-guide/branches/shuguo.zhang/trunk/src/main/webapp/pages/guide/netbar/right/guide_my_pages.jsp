<%@ page pageEncoding="utf-8" %>
<c:if test="${domainIdentified['siteNameAbbr']=='rr'}">
	<c:if test="${!empty requestScope.myPages}">
		<div class="side-item pages">
			<div class="side-item-header clearfix">
				<h4>我关注的公共主页</h4>
				<div class="option"><a href="${applicationScope.urlPage}/home/mypages?actionid=600600" rel="nofollow">更多</a></div>
			</div>
			<div class="side-item-body clearfix">
				<ul class="people-list">
					<c:forEach var="item" items="${requestScope.myPages}" varStatus="status">
						<li>
							<span class="headpichold">
								<a style="cursor:pointer;" href="${applicationScope.urlPage}/${item.id}?f=gp" >
									<img src="${item.tinyUrl}" onload="roundify(this);" alt="${item.name}的公共主页" title="${item.name}的公共主页"/>
								</a>
							</span>
							<span class="clearfix">
								<a href="${applicationScope.urlPage}/${item.id}?f=gn" title="${item.name}的公共主页">${item.name}</a>
							</span>
						</li>
					</c:forEach>				
				</ul>
			</div>
		</div>
	</c:if>
</c:if>