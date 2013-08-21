<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>welcome normal user!</title>
</head>

<body bgcolor="LightGray">
	<div align="right"><a href="loginOut">login out</a></div>
	<br><br><br><br>
	<table width="500" height="300" align="center"  background="/views/image/34.jpg" >
		<tr><td align="center" colspan="2"><h3>您好，${user.name}用户</h3></td></tr>
		<tr><td align="center"><h3>以下为您的权限：</h3></td></tr>
	    <tr><td></td><td align="left"><h3><a href="tableManager/showPage">编辑login_config表</a></h3></td></tr>
	    <tr><td></td><td align="left"><h3><a href="tableManager/showAllHistory">login_config历史记录</a></h3></td></tr>
	</table>
</body>
</html>