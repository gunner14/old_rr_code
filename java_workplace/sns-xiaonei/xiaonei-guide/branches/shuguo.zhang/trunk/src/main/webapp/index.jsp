<%@ page contentType="text/html;charset=UTF-8" session="false" %>
<%@page import="com.xiaonei.reg.common.constants.Globals"%>

<%
try {
	String server = request.getServerName();
	if(Globals.urlGuide.toString().equalsIgnoreCase("http://"+server)){
		response.sendRedirect(Globals.urlGuide+"/guide");
		return;	
	}
	response.sendRedirect(Globals.urlRegJump+"/xn.do?ss=10113&rt=27");
	return;
} catch(Exception e) {
	
}
%>


<a herf="${applicationScope.sysdomain}">人人网</a>