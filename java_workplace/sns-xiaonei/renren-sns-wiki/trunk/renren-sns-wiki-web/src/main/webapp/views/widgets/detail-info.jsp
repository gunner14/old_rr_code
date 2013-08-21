<%@ page contentType="text/html;charset=UTF-8" language="java" session="false" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>

<div class="wiki-intro-detail">
<div>
<dl class="wiki-intro-col2"></dl>
<dl class="wiki-intro-col2"></dl>
<c:choose>
	<c:when test="${wiki.type eq 1}">
		<jsp:include page="/views/widgets/detail-info/wiki-info-movie.jsp"></jsp:include>
	</c:when>
	<c:when test="${wiki.type eq 2}">
		<jsp:include page="/views/widgets/detail-info/wiki-info-book.jsp"></jsp:include>
	</c:when>
	<c:when test="${wiki.type eq 3}">
		<jsp:include page="/views/widgets/detail-info/wiki-info-game.jsp"></jsp:include>
	</c:when>
	<c:when test="${wiki.type eq 4}">
		<jsp:include page="/views/widgets/detail-info/wiki-info-place.jsp"></jsp:include>
	</c:when>
</c:choose>
</div>
<div class="wiki-open-close">
	<a href="javascript:void(0)" class="wiki-more">详细资料</a>
	<a href="javascript:void(0)" class="wiki-less" style="display:none">收起资料</a>
</div>
</div>