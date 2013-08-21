<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ page import="sns.renren.chong.beans.LoginConfigBean" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>key:${loginConfigBean.configKey} 历史编辑记录</title>
<script type="text/javascript" src="/views/js/tableManager.js"></script>
</head>
<body bgcolor="LightGray">
	<div>
		<p>您好，${host.name}:</p>
		<form action="actionModifyInKeyHistory" method="post">
			configKey:<input id="id_key" type="text" name="configKey" value="${loginConfigBean.configKey}" readonly="readonly"/>
			<input id="oldValue" type="hidden" name="configValue" value="${loginConfigBean.configValue}"/>
			configValue:<input id="newValue" type="text" name="newValue" value="${loginConfigBean.configValue}"/>
			<input type="submit" onclick="return checkModify();" value="提交"/>
		</form>
	</div>
	<table border="3" width="800" background="/views/image/36.jpg">
		<tr>
			<th>Time</th><th>opType</th><th>configKey</th><th>oldValue</th><th>newValue</th><th>editor</th><th>comment</th>
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
	<p><a href="showPage">back</a></p>
</body>
</html>