<%@ page pageEncoding="utf-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@include file="inc/modify-wikiid.jsp"%><!doctype html>
<html>
<head>
<title>wiki运营后台-白名单管理</title>
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
<h2>创建wiki</h2>
<c:if test="${not empty msg}">
	<div class="msg">${msg }</div>
</c:if>
<form action="http://wiki.renren.com/adminwiki/createWiki" method="post">
<table>
	<tr>
		<td>名称:</td>
		<td><input name="name" value="" /></td>
	</tr>
	<tr>
		<td>类型</td>
		<td><select name="type">
			<option value="1">电影</option>
			<option value="2">书籍</option>
			<option value="3">游戏</option>
			<option value="4">景点</option>
		</select></td>
	</tr>
	<tr>
		<td>isbn(图书必须):</td>
		<td><input name="isbn" value="" /></td>
	</tr>
	<tr>
		<td>iMDB(电影必须):</td>
		<td><input name="imdb" value="" /></td>
	</tr>
	<tr>
		<td>&nbsp;</td>
		<td><input type="submit" value="创建" /></td>
	</tr>
</table>
</form>
</div>
</div>
</body>
</html>