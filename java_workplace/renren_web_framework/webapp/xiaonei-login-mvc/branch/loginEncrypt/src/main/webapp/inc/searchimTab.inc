<%@ page pageEncoding="UTF-8" %>
<div id="toc">
	<ul id="tabs">
		<li class="<c:if test='${tab != \'people\'}'>in</c:if>activetab"><a href="${applicationScope.urlSearch}/searchim.do?act=people">人名</a></li>
		<li class="<c:if test='${tab != \'group\'}'>in</c:if>activetab"><a href="${applicationScope.urlSearch}/searchim.do?act=tribe">群</a></li>
		<li class="<c:if test='${tab != \'society\'}'>in</c:if>activetab"><a href="${applicationScope.urlSearch}/searchim.do?act=society">社团</a></li>
		<li class="<c:if test='${tab != \'hs\'}'>in</c:if>activetab"><a href="${applicationScope.urlSearch}/searchim.do?act=highschool">高中</a></li>
		<li class="<c:if test='${tab != \'advanced\'}'>in</c:if>activetab"><a href="${applicationScope.urlSearch}/searchim.do?act=advanced">高级搜索</a></li>
	</ul>
</div>