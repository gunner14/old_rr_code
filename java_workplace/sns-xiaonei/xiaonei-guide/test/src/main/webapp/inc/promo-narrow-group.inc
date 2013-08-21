<%@ page pageEncoding="UTF-8" %>
<c:if test="${empty cleanpage}">
		<c:if test="${visiter.stage == 10}">
		</c:if>
		<c:if test="${requestScope.visiter.loginCount > 6}">
		<c:if test="${visiter.stage == 20}">
			<div class="xp-n">
				<IFRAME id="p44" MARGINHEIGHT=0 MARGINWIDTH=0 FRAMEBORDER=0 WIDTH=120 HEIGHT=280 SCROLLING=NO SRC="http://gg.${applicationScope.domain}/view.jsp?p=84">
				<NOSCRIPT><A HREF="http://gg.${applicationScope.domain}/direct.jsp?p=84"><IMG SRC="http://gg.${applicationScope.domain}/view.jsp?p=84" WIDTH=120 HEIGHT=280 BORDER=0></a></NOSCRIPT></IFRAME>
			</div>
		</c:if>
		</c:if>
		<c:if test="${visiter.stage == 30}">
			<div class="xp-n">
				<IFRAME id="p356" MARGINHEIGHT=0 MARGINWIDTH=0 FRAMEBORDER=0 WIDTH=120 HEIGHT=280 SCROLLING=NO SRC="http://gg.${applicationScope.domain}/view.jsp?p=356">
				<NOSCRIPT><A HREF="http://gg.${applicationScope.domain}/direct.jsp?p=356"><IMG SRC="http://gg.${applicationScope.domain}/view.jsp?p=356" WIDTH=120 HEIGHT=280 BORDER=0></a></NOSCRIPT></IFRAME>
			</div>
		</c:if>
</c:if>


