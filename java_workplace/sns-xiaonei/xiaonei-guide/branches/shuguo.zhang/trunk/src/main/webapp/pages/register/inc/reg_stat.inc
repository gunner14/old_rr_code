<%
	String ayid = "";
	try {
		ayid = request.getParameter("ayid");
		if (ayid != null) {
			com.xiaonei.platform.core.opt.base.CookieManager.getInstance().saveCookie(response, "_ayid", ayid, 60*60, "/");
		}
	} catch(Exception e) {}
%>

<c:if test="${requestScope.from == '32'}">
<%
	try {
		String a_id = request.getParameter("a_id");
		String b_id = request.getParameter("b_id");
		String e1 = request.getParameter("e1");
		if (a_id!=null && b_id!=null && e1!=null && a_id.length()>0 && b_id.length()>0 && e1.length()>0) {
			com.xiaonei.platform.core.opt.base.CookieManager.getInstance().saveCookie(response, "EqifaInfo", a_id + "|" + b_id + "|" + request.getRemoteAddr() + "|" + e1 + "|" + request.getParameter("eu_id"), 60*60, "/");
		}
	} catch(Exception e) {
		e.printStackTrace(System.err);
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