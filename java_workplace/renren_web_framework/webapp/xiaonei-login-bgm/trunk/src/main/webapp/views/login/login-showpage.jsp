<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<script type="text/javascript" src="/views/js/check.js"></script>
<title>欢迎登陆</title>

</head>
<body background="/views/image/21.jpg">
	<form action="actionLogin" method="post">
		<input type="hidden" name="href" value="${href}">
		<table align="right" width="400" height="200" background="/views/image/10.jpg">
			<tr><td colspan="2" align="center"><font size="6" color="blue">登陆</font></td></tr>
			<tr>
				<td align="right"><font color="blue">用户名 :</font></td>
				<td align="center"> <input id="name" type="text" name="name"/></td>
			</tr>
			<tr>
				<td align="right"><font color="blue">密码      :</font></td>
				<td align="center"><input id="password" type="password" name="password"/></td>
			</tr>
		    <tr>
		    	<td colspan="2" align="center"><input type="submit" value="提交" onclick="return loginCheck() ;"/></td>
		    </tr>
		    <tr>
		    	<td colspan="2" align="center"><a href="../register/showPage">go to register</a></td>
		    </tr>
		</table>
	</form>	
</body>
</html>