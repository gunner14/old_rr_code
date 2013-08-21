<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html"%>
<%@ taglib uri="http://www.renren.com/tags-core" prefix="xiaonei"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<%@ include file="/inc/head-guide-flow.inc" %>
	<link href="${applicationScope.urlSmallStatic}/csspro/apps/guide-new-game1.2.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all">
	
	<title>${domainIdentified["siteName_fillinfo"]} - 选择你目前的身份</title>
</head>
<body id="home" class="guide">
<%--menu-bar begin--%>
<%@ include file="/inc/header-guide-flow.inc" %>
<%--menu-bar end--%><%--header-information-guide.inc end--%>
<div id="content">
	<div class="user-data">
		<div class="title">
		<h1>请选择你目前的身份 ：</h1>
		</div><%--title end--%>
		<div class="identity-data">
			<ul class="clearfix" id="choose-identity">
				<li class="me-work"><a href="#" onfocus="this.blur()">已工作</a></li>
				<li class="me-university"><a href="#" onfocus="this.blur()">大学生</a></li>
				<li class="me-secondary">
					<a href="#" id="me-secondary-text" onfocus="this.blur()">中学生</a>
					<div class="choose-box" id="choose-box" style="display:none">
						<a href="#" id="me-secondary-junior" onfocus="this.blur()">初中生</a>
						<a href="#" id="me-secondary-high" onfocus="this.blur()">高中生</a>
						<a href="#" id="me-secondary-tech" onfocus="this.blur()">中专生</a>
					</div>
				</li>
				<li class="me-other"><a  href="#" onfocus="this.blur()">其他</a></li>
			</ul>
			<div class="tips" id="tips" style="display:none;"></div>
		</div><%--identity-data end--%>
		<form action="ficorals" method="post" id="chooseStage">
		<input type="hidden" value="" name="stage" id="stage"/>
		<div class="btn-box"><input type="submit" value="确定" class="input-next" id="submit-btn" /></div>
		</form>
	</div><%--user-data end--%>
</div><%--content end--%>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/guide/xn.page.guide.stormAddStage-v2.2.js?ver=$revxxx$"></script>
<%-- 特定于页面的代码 --%>
<script type="text/javascript">
	XN.dom.ready(function(){
		XN.page.guide.stormAddStage.initStage();
        
        <%--//guide统计--%>
        guideStats.send('rggp1v1a0');

        <%--//确定按钮统计--%>
        XN.event.addEvent($('submit-btn'),'click', function(){
             guideStats.send('rggp1v1a1')
        });
	});
</script>
<%@ include file="/inc/footer-guide-flow.inc" %>
</body>
</html>
