<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html"%>
<%@ taglib uri="http://www.renren.com/tags-core" prefix="xiaonei"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<%@page import="com.xiaonei.reg.guide.root.interfaces.IGuideUserLeadType"%>
<%@page import="com.xiaonei.platform.core.model.User"%>
<%@page import="com.renren.sns.lead.dataproviders.util.StatLogger"%><html xmlns="http://www.w3.org/1999/xhtml">
<%
try{
	User host = (User)request.getAttribute("visiter");
	Object type_o = request.getAttribute("user_type_string");
	StatLogger.info(host.getId(), ""+"type"+":"+type_o+"",StatLogger.S_ARV_ENTERINTOO_IM_,""+type_o);
}
catch(Throwable e){
	
}

%>
<head><!-- _im_gum_ -->
    <%@ include file="/inc/head-guide-flow.inc" %>
    <title>${domainIdentified["siteName_fillinfo"]} - 寻找朋友</title>
	<script>var guideNextStep = 'import?new=1&skip=1';</script>
	<script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/guide/xn.guide.darkcoral.js?ver=$revxxx$"></script>
	<link href="${applicationScope.urlSmallStatic}/csspro/apps/guide-reg.css?ver=$revxxx$" rel="stylesheet" rev="stylesheet" media="all" />
	
	<script type="text/javascript" src="http://s.xnimg.cn/jspro/friend/xn.ui.friend.importer.js?ver=$revxxx$"></script>
	<link href="http://s.xnimg.cn/n/apps/address/importer/importer-all-min.css?ver=$revxxx$" rel="stylesheet" rev="stylesheet" media="all" />
</head>
<body id="home" class="guide">
	<div class="main_wrap clearfix">
		<div class="reg_logo">10秒找到你所有朋友</div>
		<div class="guide-main-new clearfix">
			<span class="login"><a href="http://www.renren.com/Logout.do" >退出</a></span>
			<div class="steps second clearfix">
				<p>1.填写个人信息</p>

				<p class="online">2.找到朋友，完成</p>
			</div>
			<div id="" class="guide-content">
				<div id="importer">
						<!-- 这里放置导联系人组件 -->	
						
				</div>
					
				<div id="" class="side-mod">
					<img src="http://a.xnimg.cn/imgpro/reg/reg-guide/msn-import.png" alt="" />
				</div>
			</div>
			<div class="b_text"></div>

		</div>
	</div>
<script type="text/javascript">
	var guideTrace_pageId = 'p31';
	XN.dom.ready(function(){
		Import.init();  //初始化导人页面的js
	});
	var page_id = "p31";  //统计用的页面id
</script>
<%@ include file="/inc/guide_stat.inc" %>
</body>
</html>
