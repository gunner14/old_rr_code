<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ page import="java.util.List, sns.renren.chong.beans.ApplicantBean" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>申请用户</title>
</head>
<body bgcolor="LightGray">
	<p>您好，${host.name}
	<form action="showPage" method="post"><input type="text" name="pattern"/><input type="submit" value="搜索"/></form>
	<table border="3" width="800" background="/views/image/36.jpg">
		<tr><td colspan="5" align="center"><h3>申请用户</h3></td></tr>
		<tr>
			<th>用户名</th>
			<th>邮箱</th>
			<th>附加信息</th>
			<th>操作1</th>
			<th colspan="2">操作2</th>
		</tr>
		<c:forEach var="item" items="${users }">
			<tr>
				<td align="center">${item.name}</td>
				<td align="center">${item.mail}</td>
				<td align="center">${item.remark}</td>
				<td align="center">
					<form action="actionAgree" method="post">
						<input type="hidden" name="name" value="${item.name}"/>
						<input type="hidden" name="mail" value="${item.mail}"/>
						<input type="hidden" name="remark" value="${item.remark}"/>
						<input type="submit" value="同意"/>
					</form>
				</td>
				<td align="center">
					<form action="actionRefuse" method="post">
						<input type="hidden" name="name" value="${item.name}"/>
						<input type="hidden" name="mail" value="${item.mail}"/>
						<input type="hidden" name="remark" value="${item.remark}"/>
						<input type="submit" value="拒绝"/>
						理由：<input type="text" name="reason"/>
					</form>
				</td>	
			<tr>
		
		</c:forEach>
	
	</table>
	
	<p><a href="..">back</a>
</body>
</html>