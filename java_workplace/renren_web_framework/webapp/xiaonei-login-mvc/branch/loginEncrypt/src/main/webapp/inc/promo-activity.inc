<%@ page pageEncoding="UTF-8" %><c:if test="${channel!='space' && channel!='搜索首页' && channel!='日志终端' && channel!='相册终端'}">
<div class="block announcement link">
<h3>站内活动</h3>
<div class="blockcont text">
<p><a style="color:red" href="http://09graduate.${applicationScope.domain}/graduate09/home.do" target="_blank" >难舍毕业真情流露</a></p>
<p><a href="${applicationScope.urlGroup}/GetTribe.do?id=242805090" target="_blank">${domainIdentified["siteName"]}新版站衫义卖</a></p>
<p><a href="${applicationScope.urlGroup}/GetTribe.do?id=246157366" target="_blank">精彩活动依然继续</a></p>
</div>
</div></c:if>