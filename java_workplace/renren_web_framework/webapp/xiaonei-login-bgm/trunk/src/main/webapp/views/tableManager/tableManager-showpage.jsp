<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ page import="sns.renren.chong.beans.LoginConfigBean, java.util.*" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>表格管理</title>
<script type="text/javascript" src="/views/js/tableManager.js"></script>
<% 
	String image = "";
	Random random = new Random();
	int index = Math.abs(random.nextInt()) % 79 + 1;
	image = "/views/image/" + index + ".jpg"; 
	request.setAttribute("image", image);
%>
</head>
<body bgcolor="LightGray">
	<font size="6" color="green">您好，${host.name}!</font>
	<br><br><br><br>
	
	<form action="showPage" method="post">
		<input type="text" name="pattern" value="${pattern}"/>
		<input type="submit" value="搜索"/>
	</form>
	<table border="3" width="800" background="/views/image/36.jpg" >
		<tr><td colspan="6" align="center"><h3>login_config表</h3></td></tr>
		<tr><td align="right" colspan="6"><button onclick="reset()">恢复</button></td></tr>
		<tr><th>ID</th><th>configKey</th><th>configValue</th><th>修改</th><th>删除</th><th>编辑历史记录</th></tr>
		<c:forEach var="item" items="${content}">
			<tr>
				<td align="center">${item.id}</td>
				<td align="center">${item.configKey}</td>
				<td align="center">${item.configValue}</td>
				<td align="center">
					<button onclick="modifyReplace(this)">修改</button>
				</td>
				<td align="center">
					<form action="actionDelete" method="post">
						<input type="hidden" name="pattern" value="${pattern}"/>
						<input type="hidden" name="currentPage" value="${currentPage}"/>
						<input type="hidden" name="configKey" value="${item.configKey}"/>
						<input type="hidden" name="configValue" value="${item.configValue}"/>
						<input type="submit" value="删除"/>
					</form>
				</td>
				<td align="center">
					<a href="showSpecifiedKeyHistory?configKey=${item.configKey}&configValue=${item.configValue}">查看</a>
				</td>
			</tr>
		</c:forEach>
		<tr><td colspan="6"><button onclick="addReplace(this)">添加新项</button></td></tr>
		<tr>
			<td colspan="6">
				查看全部编辑历史记录:
				<a href="showAllHistory?sortPattern=time">按时间排序</a>
				<a href="showAllHistory?sortPattern=key">按configKey排序</a>
			</td>
		</tr>
	</table>
	<br/>
	<div>
		<form action="actionSpecifiedPage" method="post">
			<input type="hidden" name="pattern" value="${pattern}"/>
			<input type="submit" value="跳转"/>
			<input type="text" size="4" name="specifiedPage" value="${currentPage}"/>页		
		</form> 
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
		<form action="actionPreviousPage" method="post">
			<input type="hidden" name="pattern" value="${pattern}"/>
			<input type="hidden" name="currentPage" value="${currentPage}"/>
			<input type="submit" value="上一页"/>
		</form>
		<form action="actionNextPage" method="post">
			<input type="hidden" name="pattern" value="${pattern}"/>
			<input type="hidden" name="currentPage" value="${currentPage}"/>
			<input type="submit" value="下一页"/>
		</form>
	</div>
	
	<div id="Page" currentPage="${currentPage}" sumPages="${sumPages}" pattern="${pattern}">
		<p>第${currentPage}页     共${sumPages}页  </p>                      
	</div>
	<font size="5"><a href="..">back</a></font>
</body>
</html>