<%@ page pageEncoding="UTF-8" %>
<c:if test="${channel!='space'}">
<div class="block announcement link">
<h3>站内活动</h3>
<div class="blockcont text">
	<c:forEach var="item" items="${activityList}">
		<c:if test="${!empty item.content}">
			<p><a style="color:${item.color}" href="${item.linkurl}" target="_blank">${item.content}</a></p>
		</c:if>
	</c:forEach>
</div>
</div>
</c:if>