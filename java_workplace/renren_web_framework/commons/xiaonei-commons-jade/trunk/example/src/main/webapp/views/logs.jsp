<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ taglib uri="/WEB-INF/tlds/c.tld" prefix="c"%>

<html>

<head>

<meta http-equiv="Content-Type" content="UTF-8" />

</head>

<body>

<h2>Rose-Jade Example - 日志列表</h2>

<input type="button" name="Back" value="返回"
	onclick="location = '..'" />

<form method="GET" action="find_log">
	<p>用户 ID：<input type="text" name="userid" value="${userid}" /></p>
	<p>关 键 字：<input type="text" name="keyword" value="${keyword}" /></p>
	<p>记录时间：<input type="text" name="timestamp" value="${timestamp}" /> (yyyy-MM-dd)</p>
	<input type="submit" name="OK" value="查找" />
</form>

<table border="1" cellpadding="8">

	<thead>
		<tr>
			<td>用户 ID</td>
			<td>日志内容</td>
			<td>记录时间</td>
		</tr>
	</thead>

	<tbody>

	<c:forEach items="${logInfoList}" var="logInfo"
		varStatus="status">

		<tr>
			<td>${logInfo.userId}</td>
			<td>${logInfo.message}</td>
			<td>${logInfo.timestamp}</td>
		</tr>	

	</c:forEach>

	</tbody>

</table>

<form method="POST" action="write_log">
	<input type="hidden" name="userid" value="0" />
	<p>日志内容：<input type="text" name="message" /></p>
	<p>记录时间：<input type="text" name="timestamp" /> (yyyy-MM-dd hh:mm:ss)</p>
	<input type="submit" name="OK" value="增加" />
</form>

<form method="POST" action="clear_log">
	<input type="submit" name="OK" value="清除" />
</form>

</body>

</html>
