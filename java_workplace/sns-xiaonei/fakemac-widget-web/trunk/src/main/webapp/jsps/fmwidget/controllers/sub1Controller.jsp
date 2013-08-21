<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%!void get(HttpServletRequest request, HttpServletResponse response){
    try{
    javax.servlet.RequestDispatcher a = request.getRequestDispatcher("/jsps/views/View1.jsp");
    a.forward(request,response);
        }
        catch(Exception e){
            
        }
}
%>

<%
get(request,response);
%>
