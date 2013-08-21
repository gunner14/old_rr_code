<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<%
	boolean isComplete =false;
	if(!"true".equals(request.getParameter("run"))|| 
			StringUtils.isEmpty(request.getParameter("type"))){
		out.print("hello world");
		return;
	}
	
	if("resource".equals(request.getParameter("type"))&&
		!"".equals(request.getParameter("baseName"))){
		DodoyoResource.reloadResource(request.getParameter("baseName"));
		out.print("reload ok");
		isComplete = true;
	}
	
	if(isComplete){
		return;
	}
%>

<%@page import="com.dodoyo.opt.DodoyoResource"%>
<%@page import="org.apache.commons.lang.StringUtils"%><html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Insert title here</title>
</head>
<body>
<form action="/pages/manager-platform/manager.jsp" >
	<input type="hidden" name="type" value="resource">
	<input type="text" name="baseName">
	<input type="submit">
</form>
</body>
</html>