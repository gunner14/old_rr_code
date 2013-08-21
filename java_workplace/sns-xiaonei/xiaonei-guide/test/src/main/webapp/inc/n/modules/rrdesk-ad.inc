<%@ page contentType="text/html;charset=UTF-8" %>
<c:set var="hasNew" value="false" />
<c:set var="newFlag">
<c:choose>
<c:when test="${hasNew}">
<img src="http://a.xnimg.cn/imgpro/icons/new-bullet.gif" class="new" />
</c:when>
<c:otherwise></c:otherwise>
</c:choose>
</c:set>
<c:set var="ads_random"><%=new java.util.Random().nextInt(6) %></c:set>
<c:choose>
<c:when test="${ads_random == 0 }">
<div class="rrdesk clearfix " id="rrdesk">
<a href="http://im.renren.com/?subver=8&amp;word01"><img class="rrdesk-icon" src="${applicationScope.urlStatic}/a.gif" style="background: url(${applicationScope.urlStatic}/imgpro/icons/client3.png) left center no-repeat;" /></a>
<a stats="home_seeindex" target="_blank" href="http://im.renren.com/desktop/rrsetup-8.exe?word01" >下载人人桌面3.1</a>${newFlag}
<p>不开网页上人人？</p>
<a style="margin-left:-2px" href="http://g.renren.com/?subver=5&amp;word01"><img class="rrgame-icon" src="${applicationScope.urlStatic}/a.gif" /></a>
<a stats="home_seeindex" target="_blank" href="http://g.renren.com/lobby/rrgamesetup-5.exe?word01" >下载游戏大厅2.1</a>&nbsp;<span>游戏定时提醒</span>
</div>
</c:when>
<c:when test="${ads_random == 1 }">
<div class="rrdesk clearfix " id="rrdesk">
<a href="http://im.renren.com/?subver=8&amp;word02"><img class="rrdesk-icon" src="${applicationScope.urlStatic}/a.gif" style="background: url(${applicationScope.urlStatic}/imgpro/icons/client3.png) left center no-repeat;" /></a>
<a stats="home_seeindex" target="_blank" href="http://im.renren.com/desktop/rrsetup-8.exe?word02" >下载人人桌面3.1</a>${newFlag}
<p>唯一隐身登录器！</p>
<a style="margin-left:-2px" href="http://g.renren.com/?subver=5&amp;word02"><img class="rrgame-icon" src="${applicationScope.urlStatic}/a.gif" /></a>
<a stats="home_seeindex" target="_blank" href="http://g.renren.com/lobby/rrgamesetup-5.exe?word02" >下载游戏大厅2.1</a>&nbsp;<span>批量快速加好友</span>
</div>
</c:when>
<c:when test="${ads_random == 2 }">
<div class="rrdesk clearfix " id="rrdesk">
<a href="http://im.renren.com/?subver=8&amp;word03"><img class="rrdesk-icon" src="${applicationScope.urlStatic}/a.gif" style="background: url(${applicationScope.urlStatic}/imgpro/icons/client3.png) left center no-repeat;" /></a>
<a stats="home_seeindex" target="_blank" href="http://im.renren.com/desktop/rrsetup-8.exe?word03" >下载人人桌面3.1</a>${newFlag}
<p>相片全屏查看，任你下载</p>
<a style="margin-left:-2px" href="http://g.renren.com/?subver=5&amp;word03"><img class="rrgame-icon" src="${applicationScope.urlStatic}/a.gif" /></a>
<a stats="home_seeindex" target="_blank" href="http://g.renren.com/lobby/rrgamesetup-5.exe?word03" >下载游戏大厅2.1</a>&nbsp;<span>帐号多开</span>
</div>
</c:when>
<c:when test="${ads_random == 3 }">
<div class="rrdesk clearfix " id="rrdesk">
<a href="http://im.renren.com/?subver=8&amp;word04"><img class="rrdesk-icon" src="${applicationScope.urlStatic}/a.gif" style="background: url(${applicationScope.urlStatic}/imgpro/icons/client3.png) left center no-repeat;" /></a>
<a stats="home_seeindex" target="_blank" href="http://im.renren.com/desktop/rrsetup-8.exe?word04" >下载人人桌面3.1</a>${newFlag}
<p>♥个性回复签名随意加♥</p>
<a style="margin-left:-2px" href="http://g.renren.com/?subver=5&amp;word04"><img class="rrgame-icon" src="${applicationScope.urlStatic}/a.gif" /></a>
<a stats="home_seeindex" target="_blank" href="http://g.renren.com/lobby/rrgamesetup-5.exe?word04" >下载游戏大厅2.1</a>&nbsp;<span>帐号多开</span>
</div>
</c:when>
<c:when test="${ads_random == 4 }">
<div class="rrdesk clearfix " id="rrdesk">
<a href="http://im.renren.com/?subver=8&amp;word05"><img class="rrdesk-icon" src="${applicationScope.urlStatic}/a.gif" style="background: url(${applicationScope.urlStatic}/imgpro/icons/client3.png) left center no-repeat;" /></a>
<a stats="home_seeindex" target="_blank" href="http://im.renren.com/desktop/rrsetup-8.exe?word05" >下载人人桌面3.1</a>${newFlag}
<p>【热】截图+上传</p>
<a style="margin-left:-2px" href="http://g.renren.com/?subver=5&amp;word05"><img class="rrgame-icon" src="${applicationScope.urlStatic}/a.gif" /></a>
<a stats="home_seeindex" target="_blank" href="http://g.renren.com/lobby/rrgamesetup-5.exe?word05" >下载游戏大厅2.1</a>&nbsp;<span>一键批量加好友</span>
</div>
</c:when>
<c:when test="${ads_random == 5 }">
<div class="rrdesk clearfix " id="rrdesk">
<a href="http://im.renren.com/?subver=8&amp;word06"><img class="rrdesk-icon" src="${applicationScope.urlStatic}/a.gif" style="background: url(${applicationScope.urlStatic}/imgpro/icons/client3.png) left center no-repeat;" /></a>
<a stats="home_seeindex" target="_blank" href="http://im.renren.com/desktop/rrsetup-8.exe?word06" >下载人人桌面3.1</a>${newFlag}
<p>40套免费皮肤随意换</p>
<a style="margin-left:-2px" href="http://g.renren.com/?subver=5&amp;word06"><img class="rrgame-icon" src="${applicationScope.urlStatic}/a.gif" /></a>
<a stats="home_seeindex" target="_blank" href="http://g.renren.com/lobby/rrgamesetup-5.exe?word06" >下载游戏大厅2.1</a>&nbsp;<span>多帐号一键登录</span>
</div>
</c:when>
<c:otherwise>
</c:otherwise>
</c:choose>
