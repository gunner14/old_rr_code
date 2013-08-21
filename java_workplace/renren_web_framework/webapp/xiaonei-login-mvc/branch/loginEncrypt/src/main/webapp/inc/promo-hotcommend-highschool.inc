<%@ page pageEncoding="UTF-8" %>
<c:choose>
	<c:when test="${applicationScope.domain == 'kaixin.com'}">
		<%--<c:if test="${channel=='首页' || channel=='日志' || channel=='日志终端'}">
			<div class="block announcement link">
				<h3>热点推荐</h3>
				<div class="blockcont text" id="ad1000000054"></div>
			</div>
			<div class="block announcement link">
				<h3>今日之星</h3>
				<div class="blockcont text" id="ad1000000055"></div>
			</div>
		</c:if>--%>
	</c:when>
	<c:when test="${applicationScope.domain == 'renren.com'}">
		<c:if test="${channel=='首页' || channel=='日志' || channel=='日志终端'}">
			<div class="block announcement link">
				<h3>热点推荐</h3>
				<div class="blockcont text" id="ad1000000054"></div>
			</div>
			<div class="block announcement link">
				<h3>今日之星</h3>
				<div class="blockcont text" id="ad1000000055"></div>
			</div>
		</c:if>
	</c:when>
</c:choose>