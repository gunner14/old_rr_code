<%@ page pageEncoding="UTF-8" %>
<div id="toc">
	<ul id="tabs">
		<li class="<c:if test='${tab != \'mine\'}'>in</c:if>activetab"><a href="${applicationScope.urlMain}/myannounce.do">我的布告</a></li>
		<li class="<c:if test='${tab != \'list\'}'>in</c:if>activetab"><a href="${applicationScope.urlMain}/getannouncelist.do?id=${requestScope.visiter.univ}">本校布告</a></li>
		<li class="<c:if test='${tab != \'new\'}'>in</c:if>activetab"><a href="${applicationScope.urlMain}/newannounce.do">张贴新布告</a></li>
	</ul>
</div>