<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@page import="java.util.List"%>
<%@page import="com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil"%>
<%@page import="com.xiaonei.platform.core.model.User"%>
<%@page import="java.util.Map"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
	<head>
		<%@ include file="/inc/head-guide-flow.inc" %>
		<script src="${applicationScope.urlSmallStatic}/jspro/guide/xn.guide.darkcoral_statis.js?ver=$revxxx$" type="text/javascript" ></script>
		<link href="${applicationScope.urlSmallStatic}/apps/tag/css/tag-all-min.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all">
		<link href="${applicationScope.urlSmallStatic}/csspro/apps/guide-reg.css?ver=$revxxx$" rel="stylesheet" rev="stylesheet" media="all" />
		<title>${domainIdentified["siteName_fillinfo"]} - 选择你的兴趣</title>
	</head>
	<body style="height:100%;">
		<div class="main_wrap clearfix">
			<div class="reg_logo">选择你的兴趣</div>
			<div class="guide-main-site clearfix">
				<span class="login"><a href="http://www.renren.com/Logout.do" >退出</a></span>
				<div class="interest">
					<h3>热门兴趣</h3>
				</div>
				<div class="guide-content">
					<div class="interest-box" style="position:relative;">
						<ul class="interest-list" id="interest-list">
							<c:forEach var="guideZ3Pojo" items="${guideZ3List}">
								<li class="" data-name="${guideZ3Pojo.tag_value}">
									<p>${guideZ3Pojo.tag_value}</p>
									<img src="http://a.xnimg.cn/imgpro/guide/interest/${guideZ3Pojo.tag_img}" height="110"/>
									<a class="opa"></a>
									<a class="followed"></a>
								</li>
							</c:forEach>
						</ul>
					</div>
				</div>
				<div class="btns clearfix">
					<div class="selectedIn">
						<span>已选择：</span>
						<div class="selected-box" id="selected-box">
						</div>
					</div>
					<div class="next">
						<a id="submit" class="go-to-home">下一步</a>
						<a href="http://guide.renren.com/flowrecmodifyz3step?skip=1" id="skip">跳过</a>
					</div>
				</div>
				<div class="tip"></div>
			</div>
			<div class="tiaokuan">
				点击免费开通账号表示您同意并遵守<a href="http://${applicationScope.sysdomain}/info/agreement.jsp" target="_blank">人人网服务条款</a>
			</div>
		</div>
		<script type="text/javascript">
			FollowSiteByInterest.init(${userId},'http://guide.renren.com/flowrecmodifyz3step');
		</script>
	</body>
</html>
