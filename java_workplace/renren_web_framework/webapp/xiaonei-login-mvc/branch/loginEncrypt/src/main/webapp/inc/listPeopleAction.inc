<%@ page pageEncoding="UTF-8" %>
<c:choose>
	<c:when test="${requestScope.visiter.status == 3 || requestScope.host == null}">
<!--		<li>打个招呼</li>  -->
		<li><a href="${applicationScope.urlMsg}/SendMessage.do?id=${item.id}&amp;${from}&amp;oriUrl=<str:encodeUrl>${requestScope.origURL}</str:encodeUrl>">发站内信</a></li>
	</c:when>
	<c:otherwise>
<!--		<li><a href="${applicationScope.urlMain}/Poke.do?id=${item.id}" onclick="return doPoke(event,'${item.id}','${item.name}')">打个招呼</a></li>  -->
		<li><a href="${applicationScope.urlMsg}/SendMessage.do?id=${item.id}&amp;${from}&amp;oriUrl=<str:encodeUrl>${requestScope.origURL}</str:encodeUrl>">发站内信</a></li>
<!--		<c:if test="${(item.id != requestScope.hostid)}"><li><a href="#" onclick="javascript:talkto(${item.id}, '${item.name}');return false">即时聊天</a></li></c:if>  -->
	</c:otherwise>
</c:choose>
