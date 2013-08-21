<%@ page pageEncoding="UTF-8" %>
<div id="toc">
	<ul id="tabs">
		<li class="<c:if test='${tab != \'people\'}'>in</c:if>activetab"><a href="${applicationScope.urlBrowse}/Search.do?portal=tab&amp;act=people">同学</a></li>
		<li class="<c:if test='${tab != \'group\'}'>in</c:if>activetab"><a href="${applicationScope.urlBrowse}/Search.do?portal=tab&amp;act=tribe">群</a></li>
	</ul>
</div>