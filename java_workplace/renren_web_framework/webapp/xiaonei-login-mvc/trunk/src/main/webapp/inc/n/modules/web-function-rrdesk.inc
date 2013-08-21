<c:set var="hasNew" value="true" />
<c:set var="newFlag">
<c:choose>
<c:when test="${hasNew}">
<img src="http://a.xnimg.cn/imgpro/icons/new-bullet.gif" class="new" />
</c:when>
<c:otherwise></c:otherwise>
</c:choose>
</c:set>
<c:set var="ads_random"><%=new java.util.Random().nextInt(5) %></c:set>
<c:choose>
<c:when test="${ads_random == 0 }">
<li>
<a href="http://im.renren.com/?subver=9&amp;word01"><img src="${applicationScope.urlStatic}/imgpro/icons/client3.png" width="15" height="15" /></a>50套免费皮肤随意换-<a stats="Side_Select_Desk"  href="http://im.${applicationScope.domain}/desktop/rrsetup-9.exe?word01" target="_blank">人人桌面</a>${newFlag}
</li>
</c:when>
<c:when test="${ads_random == 1 }">
<li>
<a href="http://im.renren.com/?subver=9&amp;word02"><img src="${applicationScope.urlStatic}/imgpro/icons/client3.png" width="15" height="15" /></a>偷偷看ta！小窗无痕查看-<a stats="Side_Select_Desk"  href="http://im.${applicationScope.domain}/desktop/rrsetup-9.exe?word02" target="_blank">人人桌面</a>${newFlag}
</li>
</c:when>
<c:when test="${ads_random == 2 }">
<li>
<a href="http://im.renren.com/?subver=9&amp;word03"><img src="${applicationScope.urlStatic}/imgpro/icons/client3.png" width="15" height="15" /></a>隐身挂机，不开网页玩人人-<a stats="Side_Select_Desk"  href="http://im.${applicationScope.domain}/desktop/rrsetup-9.exe?word03" target="_blank">人人桌面</a>${newFlag}
</li>
</c:when>
<c:when test="${ads_random == 3 }">
<li>
<a href="http://im.renren.com/?subver=9&amp;word04"><img src="${applicationScope.urlStatic}/imgpro/icons/client3.png" width="15" height="15" /></a>50套免费皮肤随意换-<a stats="Side_Select_Desk"  href="http://im.${applicationScope.domain}/desktop/rrsetup-9.exe?word04" target="_blank">人人桌面</a>${newFlag}
</li>
</c:when>
<c:when test="${ads_random == 4 }">
<li>
<a href="http://im.renren.com/?subver=9&amp;word05"><img src="${applicationScope.urlStatic}/imgpro/icons/client3.png" width="15" height="15" /></a>热门分享网罗最HOT相册-<a stats="Side_Select_Desk"  href="http://im.${applicationScope.domain}/desktop/rrsetup-9.exe?word05" target="_blank">人人桌面</a>${newFlag}
</li>
</c:when>
</c:choose>
