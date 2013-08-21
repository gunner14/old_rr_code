<%@ page pageEncoding="utf-8" %>
<%@ page import="com.dodoyo.friends.utils.LogTimeUtils"%>
<%@ page import="com.xiaonei.platform.core.cache.MemCacheManager"%>
<%@ page import="com.xiaonei.platform.core.cache.MemCacheKeys"%>
<link href="${applicationScope.urlSmallStatic}/csspro/apps/guide_common.css?ver=$revxxx$" rel="stylesheet" type="text/css" />
<link href="${applicationScope.urlSmallStatic}/csspro/module/school-selector.css?ver=$revxxx$" rel="stylesheet" type="text/css" />
<link href="${applicationScope.urlSmallStatic}/n/apps/guide/compatible/guide-all-min.css?ver=$revxxx$" rel="stylesheet" type="text/css"  />
<script src="${applicationScope.urlSmallStatic}/jspro/guide/xn.page.guide.common.js?ver=$revxxx$" type="text/javascript" ></script>
<script src="${applicationScope.urlSmallStatic}/jspro/swfobject.js" type="text/javascript"  ></script>
<script src="${applicationScope.urlSmallStatic}/jspro/swfupload.v2.2.0.1/swfupload.js?ver=$revxxx$" type="text/javascript" ></script>
<script src="${applicationScope.urlSmallStatic}/jspro/xn.app.feed.js?ver=$revxxx$" type="text/javascript" ></script>
<script src="${applicationScope.urlSmallStatic}/jspro/xn.app.status.js?ver=$revxxx$" type="text/javascript" ></script>
<script src="${applicationScope.urlSmallStatic}/jspro/xn.app.ilike.js?ver=$revxxx$" type="text/javascript" ></script>
<script src="${applicationScope.urlSmallStatic}/n/apps/guide/compatible/guide.js?ver=$revxxx$" type="text/javascript" ></script>
<script src="${applicationScope.urlSmallStatic}/jspro/xn.ui.currentPlace.js?ver=$revxxx$" type="text/javascript" ></script>
<script src="${applicationScope.urlSmallStatic}/jspro/xn.ui.schoolpopdatum.js?ver=$revxxx$" type="text/javascript" ></script>
<%--<script src="${applicationScope.urlSmallStatic}/n/core/js/newsfeed-all.js?ver=$revxxx$" type="text/javascript" ></script> --%>
<%@ include	file="/views/guide/common/top/guide_common_css_js.jsp"%>
<%@ include file="/views/guide/xf/common/top/guide_xf_df_css_js.jsp"%>
<script type="text/javascript">
	//<![CDATA[
		//安全验证
		function safetyVerification(){
			$("verification").style.display="";
			XN.DO.confirm({
				title:"安全验证",
				message:$("verification"),
				width:425,
				callback:function(r){
					if(r){
						new XN.NET.xmlhttp({
							url:'',
							data:'',
							onSuccess:function(xmlHttp){
							//logic code
						}
					})
				}
			},
			params:{showCloseButton:true}
			});
		}
	// ]]>	
</script>
<script type="text/javascript">     
     window.userGender = ("${visiter.gender}"=="女生"?"female":"male");
</script>
<c:if test="${requestScope.popInfoUpload}"> 
	<link href="${applicationScope.urlSmallStatic}/csspro/module/modifyinfo_new.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
	<script src="${applicationScope.urlSmallStatic}/jspro/guide/xn.guide.newModify.js?ver=$revxxx$"  type="text/javascript" ></script>
	<script src="${applicationScope.urlSmallStatic}/jspro/xn.ui.modifyinfo_new.js?ver=$revxxx$" type="text/javascript"  ></script>
    <script type="text/javascript">
        XN.dom.ready(function(){
			newModifyInfo();
		});
   </script>
</c:if>

																																																																																																																																																																																																																																																																														
