<%@ page pageEncoding="utf-8" %>
<c:if test="${!empty displayList}">
	<div class="side-item newrequests">
		<div class="side-item-body clearfix">
			<ul class="icon">				
				<c:forEach varStatus="status" var="item" items="${requestScope.displayList}" begin="0" end="3">
					<c:when test="${item.count > 0 && item.count<100}">
						<li>
							<img class="${item.style} icon" src="${applicationScope.urlStatic}/a.gif" width="16" height="16" />
							<a href="${item.link}"><strong>${item.count}</strong>${item.word}</a>
						</li>
					</c:when>
					<c:when test="${item.count >= 100}">
						<li>
							<img class="${item.style} icon" src="${applicationScope.urlStatic}/a.gif" width="16" height="16" />
							<a href="${item.link}"><strong>99+</strong>${item.word}</a>
						</li>
					</c:when>							
				</c:forEach>								
			</ul>
		</div>
	</div>
</c:if>
