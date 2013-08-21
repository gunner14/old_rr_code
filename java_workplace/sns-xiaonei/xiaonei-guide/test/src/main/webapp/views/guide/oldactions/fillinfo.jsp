
<%@page import="com.xiaonei.platform.core.opt.OpiConstants"%><%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%
javax.servlet.RequestDispatcher a = request.getRequestDispatcher("/fillinfo");
a.forward(request,response);
%>
