<%@ page pageEncoding="UTF-8" %>

<%@ include file="/inc/headpro.inc" %>

<c:forEach var="item" items="${requestScope.idList}">
	<c:if test="${item == visiter.id}">
		<c:set var="canntSee" value="true" />
	</c:if>
</c:forEach>
<c:if test="${canntSee == true}">
	<script type="text/javascript">XN.webpager=null</script>
</c:if>

