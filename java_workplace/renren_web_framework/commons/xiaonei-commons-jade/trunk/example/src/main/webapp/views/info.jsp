<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ taglib uri="/WEB-INF/tlds/c.tld" prefix="c" %>

<html>

<head>

<meta http-equiv="Content-Type" content="UTF-8" />

</head>

<body>

<h2>Rose-Jade Example - 联系人信息</h2>

<form method="POST" action="update">

<input type="hidden" name="id" value="${contactInfo.id}" />
<input type="hidden" name="userid" value="${contactInfo.userId}" />

<p>ID：${contactInfo.id}</p>

<p>联系人姓名：<input type="text" name="name" value="${contactInfo.name}" /></p>

<p>联系人号码：<input type="text" name="phonecode" value="${contactInfo.phoneCode}" /></p>

<input type="submit" name="Update" value="修改" />

</form>

<form method="POST" action="delete">

<input type="hidden" name="id" value="${contactInfo.id}" />
<input type="hidden" name="userid" value="${contactInfo.userId}" />

<input type="submit" name="Delete" value="删除" />

</form>

<input type="button" name="Back" value="返回" onclick="history.back()" />

</body>

</html>
