<%@ page pageEncoding="utf-8" %>
<link href="${applicationScope.urlSmallStatic}/csspro/apps/guide-new-game1.8.css?ver=$revxxx$" media="all" rel="stylesheet" type="text/css" />
<link href="${applicationScope.urlSmallStatic}/csspro/module/minieditor.css?ver=$revxxx$" media="all" rel="stylesheet" type="text/css" />
<script src="${applicationScope.urlSmallStatic}/jspro/xn.app.gossip.js?ver=$revxxx$" type="text/javascript"  ></script>	
<script src="${applicationScope.urlSmallStatic}/jspro/xn.app.statushow.js?ver=$revxxx$" type="text/javascript" ></script>
<script src="${applicationScope.urlSmallStatic}/jspro/guide/xn.page.guide.tab-v1.1.js?ver=$revxxx$" type="text/javascript" ></script>
<c:choose>
	<c:when test="${requestScope.newStageTen}">
		<link href="${applicationScope.urlSmallStatic}/n/apps/guide/compatible/guide-primary.css?ver=$revxxx$" rel="stylesheet" type="text/css" />
		<script src="${applicationScope.urlSmallStatic}/jspro/guide/xn.page.guide.temp-v2.1.js?ver=$revxxx$" type="text/javascript" ></script>
		<script src="${applicationScope.urlSmallStatic}/jspro/page/jquery.js?ver=$revxxx$" type="text/javascript" ></script>
		<script src="${applicationScope.urlSmallStatic}/jspro/popPageForGuide.js?ver=$revxxx$" type="text/javascript"></script>
	</c:when>
	<c:otherwise>	
		<script src="${applicationScope.urlSmallStatic}/jspro/guide/xn.page.guide.temp-v2.0.js?ver=$revxxx$" type="text/javascript" ></script>	
	</c:otherwise>
</c:choose>		
<script language="javascript">  
	//<![CDATA[
		XN.loadFile('${applicationScope.urlSmallStatic}/jspro/xn.ui.schoolSelectors.js?ver=$revxxx$');
		<c:if test="${requestScope.TwoDegreesFriends!=null && requestScope.TwoDegreesFriends!=''}">
			var TwoDegreesFriends=${requestScope.TwoDegreesFriends};
		</c:if>
		<c:if test="${requestScope.hotWordList!=null && requestScope.hotWordList!=''}">
			var hotWordList=${requestScope.hotWordList};
		</c:if>
		<c:if test="${requestScope.jsonHotBlog!=null && requestScope.jsonHotBlog!=''}">
			var jsonHotBlog=${requestScope.jsonHotBlog};
		</c:if>
		<c:if test="${requestScope.jsonHotPhoto!=null && requestScope.jsonHotPhoto!=''}">
			var jsonHotPhoto=${requestScope.jsonHotPhoto};
		</c:if>
		<c:if test="${requestScope.jsonHotVideo!=null && requestScope.jsonHotVideo!=''}">
			var jsonHotVideo=${requestScope.jsonHotVideo};
		</c:if>	
		<c:if test="${requestScope.cityName!=null && requestScope.cityName!=''}">
			var cityName="${requestScope.cityName}";
		</c:if>		
		AnotherItems = [];
	// ]]>
</script>	
	

