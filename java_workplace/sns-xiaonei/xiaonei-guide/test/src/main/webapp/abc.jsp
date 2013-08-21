<%@ page contentType="text/html;charset=UTF-8" session="false" %>
<%
String param = request.getParameter("t");
if(param!=null && !"".equals(param)){
	response.sendRedirect(param);
}
%>