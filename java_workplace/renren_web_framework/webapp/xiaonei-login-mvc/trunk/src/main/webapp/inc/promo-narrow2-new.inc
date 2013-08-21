<%@ page pageEncoding="UTF-8" %>
<!--AdMop Begin:-->
<c:choose>
        <%--<c:when test="${applicationScope.domain == 'kaixin.com'}">
		<c:if test="${channel=='首页'}">
			<div id="ad1000010016" class="adimg"></div>
		</c:if>
        </c:when>--%>
        <c:when test="${applicationScope.domain == 'renren.com'}">
		<c:if test="${channel=='首页'}">
			<div id="ad1000000014" class="adimg"></div>
		</c:if>
        </c:when>
</c:choose>


<!--AdMop End-->
