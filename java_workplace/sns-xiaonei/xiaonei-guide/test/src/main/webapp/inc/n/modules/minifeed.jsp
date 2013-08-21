<%@ page contentType="text/html;charset=UTF-8" session="false" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>

<%--
@param inited 是否展示默认新鲜事
@param loadOnScroll 是否在滚屏时自动加载
--%>

<div class="feed-module" data-loadonscroll="${param.loadOnScroll}">

	<div class="feed-list">
		<c:if test="${param.inited != false}">
			${fn:trim(requestScope.userFeed)}
		</c:if>
	</div>

	<div class="feed-footer">
		<div class="feed-loading" style="display:none"><p>新鲜事读取中...</p></div>
		<div class="nomore-feed" style="display:none"><p>没有更多新鲜事了</p></div>
		<div class="more-feed"><a id="more-feed" href="#">更多新鲜事</a></div>
	</div>

</div>

