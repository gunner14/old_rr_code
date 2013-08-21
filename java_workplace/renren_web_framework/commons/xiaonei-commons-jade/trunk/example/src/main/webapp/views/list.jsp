<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ taglib uri="/WEB-INF/tlds/c.tld" prefix="c"%>

<html>

<head>

<meta http-equiv="Content-Type" content="UTF-8" />

</head>

<body>

<h2>Rose-Jade Example - 联系人列表</h2>

<input type="button" name="Back" value="返回" 
	onclick="location = '..'" />

<p>用户 ID：${userid}</p>

<p>联系人数目：${contactInfoSize}</p>

<form method="GET" action="find">
	<p>用户 ID：<input type="text" name="userid" value="${userid}" /></p>
	<p>联系人姓名：<input type="text" name="name" value="${name}" /></p>
	<p>联系人号码：<input type="text" name="phonecode" value="${phonecode}" /></p>
	<input type="submit" name="OK" value="查找" />
</form>

<table border="1" cellpadding="8">

	<thead>
		<tr>
			<td>ID</td>
			<td>联系人姓名</td>
			<td>联系人号码</td>
		</tr>
	</thead>

	<tbody>

	<c:forEach items="${contactInfoList}" var="contactInfo"
		varStatus="status">

		<tr>
			<td>${contactInfo.id}</td>
			<td><a href="info?id=${contactInfo.id}">${contactInfo.name}</a></td>
			<td>${contactInfo.phoneCode}</td>
		</tr>	

	</c:forEach>

	</tbody>

</table>

<form method="POST" action="insert">
	<input type="hidden" name="userid" value="${userid}" />
	<p>联系人姓名：<input type="text" name="name" /></p>
	<p>联系人号码：<input type="text" name="phonecode" /></p>
	<input type="submit" name="OK" value="增加" style="margin-right: 10px" />
	<input type="button" name="Batch" value="批量增加" 
		onclick="location = 'batch?userid=${userid}'" />
</form>

<c:if test="${contactWarning}">
	<p>${contactWarning}</p>
</c:if>

</body>

</html>
