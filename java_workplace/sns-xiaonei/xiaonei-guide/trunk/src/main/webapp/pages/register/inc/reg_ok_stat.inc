<c:if test="${requestScope.from == '29'}">
<%String cookie = com.xiaonei.platform.core.opt.base.CookieManager.getInstance().getCookie(request, "_ayid");%>
<img src="http://61.152.242.247/sm/tb/xiaonei/getxn.php?uid=${requestScope.userId}&ayid=<%=cookie%>&flag=0" width="0" height="0" />
<script language='JavaScript'>
document.write('<script language=javascript src=http://smarttrade.allyes.com/main/adftrack?db=smarttrade&point=3187&js=on&cache='+(new Date()).getTime()+'&pre='+escape(document.referrer)+'><\/s'+'cript>');
</script>
</c:if>
<c:if test="${requestScope.from == '32'}">
<%
	String cookie = com.xiaonei.platform.core.opt.base.CookieManager.getInstance().getCookie(request, "EqifaInfo");
	if (cookie != null && cookie.length() > 0) {
%>
<script type="text/javascript" src='http://service.eqifa.com/purchase_new_cpa.aspx?a_id=<%=cookie%>&m_id=&mbr_id=${requestScope.userId}&mbr_type=0&mbr_note=xiaonei'></script>
<%
	}
%>
</c:if>
<c:if test="${requestScope.from == '35'}">
<script type="text/Javascript">
var alimm_cid="11073610_10000142";
var alimm_ts="24";
</script>
<script type="text/Javascript" src="http://stats.alimama.com/etev.js"></script>
</c:if>
<%
try {
		javax.servlet.http.Cookie co_reg_stage = new Cookie("_reg_stage", "reg_complete");
		co_reg_stage.setDomain(com.xiaonei.reg.common.constants.Globals.domain.toString());
		co_reg_stage.setMaxAge(60 * 1000);
		response.addCookie(co_reg_stage);
		javax.servlet.http.Cookie co_user_stage = new Cookie("_user_stage", ""+request.getAttribute("stage"));
		co_user_stage.setDomain(com.xiaonei.reg.common.constants.Globals.domain.toString());
		co_user_stage.setMaxAge(60 * 1000);
		response.addCookie(co_user_stage);		
} catch (Exception e) {
}
%>