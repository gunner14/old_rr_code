<c:choose>
<c:when test="${not empty wiki}">
<c:set var="wikiId" value="${wiki.id}" />
</c:when>
<c:otherwise>
<c:set var="wikiId" value="${param.wikiId}"></c:set>
</c:otherwise>
</c:choose>