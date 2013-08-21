<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ page import="sns.renren.chong.beans.UserBean" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>welcome super user!</title>
</head>
<body background="/views/image/36.jpg">
	<div align="right"><a href="loginOut">login out</a></div>
	<br>
	<br>
	<table width="500" height="300" align="center"  background="/views/image/36.jpg" >
		<tr><td align="center" colspan="2"><h3>您好，${user.name}超级用户</h3></td></tr>
		<tr></tr>
		<tr><td align="left"><h3>以下为您的权限：</h3></td></tr>
		
	    <tr><td></td><td align="left"><h3><a href="tableManager/showPage">编辑login_config表</a></h3></td></tr>
	    <tr><td></td><td align="left"><h3><a href="tableManager/showAllHistory">login_config历史记录</a></h3></td></tr>
	    <tr><td></td><td align="left"><h3><a href="userManager/showPage">管理用户(数量：${userNum})</a></h3></td></tr>
	    <tr><td></td><td align="left"><h3><a href="applicantManager/showPage">处理申请(数量：${applicantNum})</a></h3></td></tr>
	</table>

</body>
</html>