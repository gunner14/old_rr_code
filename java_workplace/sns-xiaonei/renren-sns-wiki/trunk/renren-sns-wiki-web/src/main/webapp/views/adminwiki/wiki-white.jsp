<%@ page pageEncoding="utf-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@include file="inc/modify-wikiid.jsp" %><!doctype html>
<html>
<head>
	<meta charset="utf-8" />
	<title>wiki运营后台-白名单管理</title>
	<link rel="stylesheet" type="text/css" href="http://wiki.renren.com/views/adminwiki/css/wiki.css" />
</head>
<body>
	<div class="pagehead">
		<div class="headcenter">
			<h1><a href="http://www.renren.com/"><img width=160 height="31" src="http://s.xnimg.cn/imgpro/v6/logo.png" alt="人人网" /></a></h1>
		</div>
	</div>
	<div class="pagecontent clearfix">
		<%@include file="inc/adminwiki-nav.jsp" %>
		<div class="right-con">
			<div>
			<form action="http://wiki.renren.com/adminwiki/wikiWhite/addWhiteUser" method="post">
				人人ID:<input name="userId" value="" />
				<input type="submit" value="添加" />
			</form>
			</div>
			<c:if test="${not empty result }">
			<div style="font-size: 14px;font-weight: bold;">
			${result }
			</div>
			</c:if>
		</div>
	</div>
</body>
</html>