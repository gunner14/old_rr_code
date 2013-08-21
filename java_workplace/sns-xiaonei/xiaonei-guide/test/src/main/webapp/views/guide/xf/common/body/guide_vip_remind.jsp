<%@ page pageEncoding="utf-8" %>
<c:if test="${requestScope.days>0 && requestScope.days<=7}">
<%
	if(com.xiaonei.platform.core.opt.base.CookieManager.getInstance().getCookie(request, "vipExpire"+user.getId()) == null){
%>
	<div id="vipExpireTooltip" class="infoaltbox">
		<div class="headangle"></div>
		<span class="share-n-hide">
			<a onclick="XN.Cookie.set('vipExpire${visiter.id}', '0', 7, '/', '.${applicationScope.sysdomain}'); if($('vipExpireTooltip'))$('vipExpireTooltip').hide();" href="javascript:;" class="x-to-hide"/></a>
		</span>
		<div class="altcontentshadow">
			<div class="altcontent">
				<div class="header">
					<p>VIP会员还有<em class="expiryDays">${requestScope.days}</em>天到期</p>
					<p>记得续费哦！<a href="${applicationScope.urlI}/pay_pre.action?wc=380000">续费去</a></p>
				</div>
			</div>
		</div>
	</div>
<%
	}
%>
</c:if>

