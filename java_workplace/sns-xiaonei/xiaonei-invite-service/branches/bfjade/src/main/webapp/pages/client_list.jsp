<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Invite Service Manage - Client List</title>
</head>
<body>
	<h1>Invite Service Management</h1>
	<h1><a href=?action=client_list>Home</a></h1>
	<h1><a href=?action=logout>Logout</a></h1>
	<b>服务接口列表</b>&nbsp;<input type="button" onclick="if (!window.confirm('确认要重新加载服务状态？'))return;window.location.href='?action=action_status_reload';" value="重新载入">
	<hr border="1" color="blue">
	<table width="800" align="" border="1" cellpadding="2" cellspacing="2" style="border-collapse: collapse; border-bottom-width: 0" bordercolor="#111111">
		<tr bgcolor="#f2f2f2">
			<td>
				接口(action)
			</td>
			<td>
				状态(status)
			</td>
		</tr>
		<c:forEach items="${actionList}" var="kv">
		<tr>
			<td>
				${kv.key}
			</td>
			<td bgcolor="${kv.value == 'on' ? 'lightgreen' : 'red'}">
				${kv.value}
			</td>
		</tr>
		</c:forEach>
	</table>
	<br />
	
	<b>客户端列表</b>
	<hr border="1" color="blue">
	<table width="800" align="" border="1" cellpadding="2" cellspacing="2" style="border-collapse: collapse; border-bottom-width: 0" bordercolor="#111111">
		<tr bgcolor="#f2f2f2">
			<td>
				编号
			</td>
			<td>
				客户机名
			</td>
			<td>
				客户机IP
			</td>
			<td>
				状态
			</td>
			<td>
				密钥
			</td>
			<td>
				过期时间
			</td>
			<td>
				操作
			</td>
		</tr>
		<c:forEach items="${clientList}" var="client">
			<tr>
			<td>
				${client.id}
			</td>
			<td>
				${client.clientName}
			</td>
			<td>
				${client.clientIp}
			</td>
			<td bgcolor="${client.type == 1 ? 'lightgreen' : 'red'}">
				${client.type == 1 ? '白名单' : '黑名单'}
			</td>
			<td>
				${client.clientKey}
			</td>
			<td>
				${client.expireTime}
			</td>
			<td>
				<a href="?client_ip=${client.clientIp}&action=modshow">修改</a> | 
				<c:choose>
					<c:when test="${client.type == 1}">
						<a href="?client_ip=${client.clientIp}&action=disable">禁用</a>
					</c:when>
					<c:otherwise>
						<a href="?client_ip=${client.clientIp}&action=enable">启用</a>
					</c:otherwise>
				</c:choose>
			</td>
		</tr>
		</c:forEach>
	</table>
	<br />
	
	<b>增加新客户IP</b>
	<hr border="1" color="blue">
	<table width="800" align="" border="1" cellpadding="2" cellspacing="2" style="border-collapse: collapse; border-bottom-width: 0" bordercolor="#111111">
	<form action="?action=${clientInfo != null ? 'modsub' : 'add'}" method="post">
		<tr>
			<td align="right">ClientName:</td>
			<td align="left"><input name="client_name" value="${clientInfo.clientName}"></td>
		</tr>
		<tr>
			<td align="right">ClientIP:</td>
			<td align="left"><input name="client_ip" value="${clientInfo.clientIp}"></td>
		</tr>
		<tr>
			<td align="right">ClientKey:</td>
			<td align="left"><input name="client_key" value="${clientInfo.clientKey}"></td>
		</tr>
		<tr>
			<td align="right">ClientType:</td>
			<td align="left"><input name="type"  value="${clientInfo.type}"> (1：白名单，0：黑名单)</td>
		</tr>
		<tr>
			<td align="right">ExpireTime:</td>
			<td align="left"><input name="expire_time" value="${clientInfo.expireTime}"> (yyyy-MM-dd HH:mm:ss)</td>
		</tr>
		<tr>
			<td colspan="2">
			<input type="submit" value="  提  交  " />&nbsp;
			<input type="reset" value="  还  原  " />
			</td>
		</tr>
	</form>
	</table>
	
</body>
</html>