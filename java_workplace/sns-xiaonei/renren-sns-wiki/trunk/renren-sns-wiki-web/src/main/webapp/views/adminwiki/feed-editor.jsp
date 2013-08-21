<%@ page pageEncoding="utf-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@include file="inc/modify-wikiid.jsp"%><!doctype html>
<html>
<head>
<title>wiki运营后台-内容运营</title>
<link rel="stylesheet" type="text/css"
	href="http://wiki.renren.com/views/adminwiki/css/wiki.css" />
</head>
<body>
<div class="pagehead">
<div class="headcenter">
<h1><a href="http://www.renren.com/"><img width=160 height="31"
	src="http://s.xnimg.cn/imgpro/v6/logo.png" alt="人人网" /></a></h1>
</div>
</div>
<div class="pagecontent clearfix"><%@include
	file="inc/adminwiki-nav.jsp"%>
<div class="right-con">
<h2>根据关键字删除最热日志</h2>
<c:if test="${not empty msg}">
	<div class="msg" style="color:red">${msg }</div>
</c:if>
<form action="http://wiki.renren.com/adminwiki/${wiki.id}/fdeditor/del" method="post">
<table>
	<tr>
		<td>Wiki ID：</td>
		<td><a href="http://wiki.renren.com/${wiki.id}" target="_blank">${wiki.id}</a></td>
		<td>&nbsp;</td>
	</tr>
	<tr>
		<td>Wiki 名字：</td>
		<td><a href="http://wiki.renren.com/${wiki.id}" target="_blank">${wiki.name}</a></td>
		<td>&nbsp;</td>
	</tr>
	<tr>
		<td>关键字:</td>
		<td><input name="kw" value="" width="200"/></td>
		<td><input type="submit" value="清除" /></td>
	</tr>
</table>
</form>
</div>
</div>
</body>
</html>