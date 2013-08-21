
<%@page import="com.xiaonei.platform.core.opt.OpiConstants"%><%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%
try{
	String id = request.getParameter("rrmm");
	if(id != null){
	    response.sendRedirect("http://mm.renren.com/?from=mm&id="+id);
	}
	else{
	    response.sendRedirect("http://mm.renren.com/");
	}
}
catch(Exception e){
    
}
%>
