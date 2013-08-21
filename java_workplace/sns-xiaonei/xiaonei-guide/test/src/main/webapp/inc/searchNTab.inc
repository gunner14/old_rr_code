<%@ page pageEncoding="UTF-8" %>
<div id="sub-nav">
    <ul>
    		<li class="<c:if test='${tab == \'people\'}'>selected</c:if>"><a href="${applicationScope.urlBrowse}/s.do?portal=tab&act=people" onfocus="this.blur()" >
    		<c:if test="${requestScope.curStage!=30}">同学</c:if><c:if test="${requestScope.curStage==30}">朋友</c:if>
    		</a></li>
        <li class="<c:if test='${tab == \'group\'}'>selected</c:if>"><a href="${applicationScope.urlBrowse}/s.do?portal=tab&act=tribe"   onfocus="this.blur();">群组</a></li>
        <li class="<c:if test='${tab == \'advanced\'}'>selected</c:if>"><a href="${applicationScope.urlBrowse}/s.do?portal=tab&act=advanced" onfocus="this.blur()" >高级搜索</a></li>
    </ul>
</div>
