<%@ page pageEncoding="UTF-8" %>
<c:if test="${channel!='space' && channel!='搜索首页' && channel!='app'}">
<c:set var="control" value="<%=java.lang.Math.abs(new java.util.Random().nextInt())%2%>"/>
<c:choose>
<c:when test="${control == 0}">
<div class="block announcement link">
<h3 style="font-weight:bold;color:#333">特别推荐</h3>
<div class="blockcont text">
<IFRAME id="358" MARGINHEIGHT=0 MARGINWIDTH=0 FRAMEBORDER=0 WIDTH=100% HEIGHT=16 SCROLLING=NO SRC="http://gg.${applicationScope.domain}/view.jsp?p=358">
</IFRAME> 
<IFRAME id="241" MARGINHEIGHT=0 MARGINWIDTH=0 FRAMEBORDER=0 WIDTH=100% HEIGHT=16 SCROLLING=NO SRC="http://gg.${applicationScope.domain}/view.jsp?p=241">
</IFRAME>
<IFRAME id="242" MARGINHEIGHT=0 MARGINWIDTH=0 FRAMEBORDER=0 WIDTH=100% HEIGHT=16 SCROLLING=NO SRC="http://gg.${applicationScope.domain}/view.jsp?p=242">
</IFRAME>
</div>
</div>
</c:when>
<c:otherwise>
<div class="block announcement link">
<h3 style="font-weight:bold;color:#333">热点活动</h3>
<div class="blockcont text">
<IFRAME id="358" MARGINHEIGHT=0 MARGINWIDTH=0 FRAMEBORDER=0 WIDTH=100% HEIGHT=16 SCROLLING=NO SRC="http://gg.${applicationScope.domain}/view.jsp?p=359">
</IFRAME> 
<IFRAME id="241" MARGINHEIGHT=0 MARGINWIDTH=0 FRAMEBORDER=0 WIDTH=100% HEIGHT=16 SCROLLING=NO SRC="http://gg.${applicationScope.domain}/view.jsp?p=360">
</IFRAME>
</div>
</div>
</c:otherwise>
</c:choose>
</c:if>

