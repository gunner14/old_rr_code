<%@ page pageEncoding="UTF-8" %>
<c:choose>
	<c:when test="${applicationScope.domain == 'kaixin.com'}">
	<%--<c:if test="${(channel=='首页' || channel=='日志' || channel=='日志终端')}">
			<div class="blank-holder">
				<div id="banner" class="banner clearfix">
					<div class="float-left" id="ad1000000051"></div>
					<div class="float-right" id="ad1000000052"></div>
				</div>
			</div>
		</c:if>--%>
	</c:when>
	<c:when test="${applicationScope.domain == 'renren.com'}">
		<c:if test="${(channel=='首页' || channel=='日志' || channel=='日志终端')}">
			<div class="blank-holder">
				<div id="banner" class="banner clearfix">
					<div class="float-left" id="ad1000000051"></div>
					<div class="float-right" id="ad1000000052"></div>
				</div>
			</div>
		</c:if>
	</c:when>
</c:choose>