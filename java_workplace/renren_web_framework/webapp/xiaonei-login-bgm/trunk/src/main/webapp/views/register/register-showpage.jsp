<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>欢迎注册</title>

<script type="text/javascript" src="/views/js/check.js"></script>
</head>

<body  bgcolor="LightGray">
	<form action="actionRegister" method="post">
		<table width="500" height="350" background="/views/image/36.jpg">
			<tr><td colspan="2" align="center"><h3>欢迎注册</h3></td></tr>
			<tr>
				<td align="center">用户名: </td>
				<td ><input id="_name" type="text" name="name"/></td>
			</tr>
			<tr>
				<td align="center">邮箱：</td>
				<td ><input id="_mail" type="text" name="mail"/></td>
			</tr>
			<tr>
				<td align="center">密码 :</td>
				<td ><input id="_password" type="password" name="password"/></td>
			</tr>
			<tr>
				<td align="center">重复密码：</td>
				<td ><input id="_repeatPassword" type="password" name="rePsw"/></td>
			</tr>
			<tr>
				<td align="center">附加信息: </td>
				<td ><input type="text" name="remark"/></td>
			</tr>
			<tr>
				<td colspan="2" align="center">
		         <input type="submit" value="提交" onclick="return check();"/>
		        </td>
		    </tr>
		    <tr><td colspan="2" align="center"><a href="../login/showPage">go to login</a></td></tr>
		</table>
	</form>	

</body>
</html>