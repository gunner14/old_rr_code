<%@ page pageEncoding="utf-8" %>
<%
Object obj = request.getAttribute("jsonTwoDegreesFriends");
if(obj == null || "".equals((String)obj)){
	out.println("{\"candidate\":[],\"qkey\":\"friend\",\"groups\":[]}");
}
else{
	out.println((String)obj);
}
//${requestScope.jsonTwoDegreesFriends}
%>