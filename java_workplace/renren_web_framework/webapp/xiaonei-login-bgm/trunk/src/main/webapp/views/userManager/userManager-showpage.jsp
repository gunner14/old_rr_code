<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ page import="java.util.List, sns.renren.chong.beans.UserBean" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>用户管理</title>

<script type="text/javascript" src="/views/js/userManager.js"></script>
</head>

<body bgcolor="LightGray">
 	<p>您好，${host.name}
	<form action="showPage" method="post">
		<input type="text" name="pattern"/>
		<input type="submit" value="搜索"/>
	</form>
	<table border="3" width="800" background="/views/image/36.jpg">
		<tr><td colspan="4" align="center"><h3>users table</h3></td></tr>
		<tr><th>用户名</th><th>身份</th><th>邮箱</th><th>删除</th></tr>
		<c:forEach var="item" items="${users}">
			<tr>
				<td align="center">${item.name}</td>
				<td align="center">${item.identity}</td>
				<td align="center">${item.mail}</td>
				<c:choose>
					<c:when test="${item.name eq host.name}">
						<td align="center">-</td>
					</c:when>
					<c:when test='${item.identity eq "s" || item.identity eq "S"}'>
						<td align="center">no right</td>
					</c:when>
					<c:otherwise>
						<td align="center">
							<form action="actionDelete" method="post">
								<input type="hidden" name="name" value="${item.name}"/>
								<input type="submit" value="删除"/>
							</form>
						</td>
					</c:otherwise>
				</c:choose>
			</tr>
		</c:forEach>
	</table>
	<p><a href="..">back</a>
	
</body>
</html>