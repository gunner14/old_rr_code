
<%@page import="com.xiaonei.reg.guide.root.miscs.MyStringUtils"%>
<%@page import="com.xiaonei.platform.core.opt.OpiConstants"%><%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%
if(MyStringUtils.equals("1",request.getParameter("skip"))){
    javax.servlet.RequestDispatcher a = request.getRequestDispatcher("/portraits");
    a.forward(request,response);
}
else{
    javax.servlet.RequestDispatcher a = request.getRequestDispatcher("/portrait");
    a.forward(request,response);
}
%>
