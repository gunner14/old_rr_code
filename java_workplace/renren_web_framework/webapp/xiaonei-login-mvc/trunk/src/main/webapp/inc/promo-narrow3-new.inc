<%@ page pageEncoding="UTF-8" %>
<c:choose>
        <c:when test="${applicationScope.domain == 'kaixin.com'}">
		<c:if test="${channel=='首页'}">
			<!--AdMop Begin:-->
			<div id="ad1000010004" class="adimgr"></div>
			<!--AdMop End-->
		</c:if>
        </c:when>
        <c:when test="${applicationScope.domain == 'renren.com'}">
		<c:if test="${empty newHome}">
			<c:if test="${channel=='首页'}">
				<!--AdMop Begin:-->
				<div id="ad1000000013" class="adimgr"></div>
				<!--AdMop End-->
			</c:if>
		</c:if>
        </c:when>
</c:choose>


