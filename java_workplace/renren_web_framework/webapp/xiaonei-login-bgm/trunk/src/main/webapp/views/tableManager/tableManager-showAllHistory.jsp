<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Edited History</title>
</head>
<body bgcolor="LightGray">
	<font size="6" color="green">您好，${host.name}!</font>
	<br><br><br>
	<div align="center">
		<form action="showAllHistory" method="get">
			<a href="showAllHistory?sortPattern=time">按时间排序</a>
			<a href="showAllHistory?sortPattern=key">按configKey排序</a>
			<input type="text" name="keyPattern"/>
			<input type="submit" value="搜索"/>(只搜索configKey)
			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			<a href="showPage">back</a>
		</form>
	<table border="3" width="800" background="/views/image/36.jpg" >
		<tr>
			<td colspan="7" align="center"><h3>Edited History</h3></td>
		</tr>
		<tr>
			<th>Time</th>
			<th>opType</th>
			<th>configKey</th>
			<th>oldValue</th>
			<th>newValue</th>
			<th>editor</th>
			<th>comment</th>
		</tr>
		<tr>
			<c:forEach var="item" items="${histories}">
				<tr>
					<td align="center">${item.time }</td>
					<td align="center">${item.opType }</td>
					<td align="center">${item.configKey}</td>
					<td align="center">${item.oldValue }</td>
					<td align="center">${item.newValue }</td>
					<td align="center">${item.editor }</td>
					<td align="center">${item.comment }</td>
				</tr>
			</c:forEach>
		</tr>
	</table>
		<font size="5"><a href="showPage">back</a></font>
	</div>
</body>
</html>