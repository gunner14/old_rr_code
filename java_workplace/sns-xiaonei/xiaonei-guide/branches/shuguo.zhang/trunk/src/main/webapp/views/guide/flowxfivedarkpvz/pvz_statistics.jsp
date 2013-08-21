<%@ page contentType="text/html;charset=UTF-8" session="false"%>

<%! Object userId="0"; Object url="";%>


<html>
<head>
</head>
<body>

<% userId =request.getAttribute("uid");if(userId==null){
   userId="0";
   }
   if(System.currentTimeMillis()%100 < 65)
  {
   //out.print(userId);
%>


<img src="http://count.chanet.com.cn/action.cgi?t=5826&i=<%=userId %>" width=1 height=1> </img>


<% 	
  }
  else
 {
   //out.print(userId);
%>


<img src="http://count.chanet.c0m.cn/action.cgi?t=5826&i=<%=userId %>" width=1 height=1> </img>


<%
}
   url = request.getAttribute("returl");
   if(url==null||url==""){
	   url="#";
   }
   //response.sendRedirect(url); 
   //response.setHeader("refresh","1;"+url);
%>

<script type="text/javascript">
window.onload = function(){ window.location.href="<%=url %>";} 
</script>





</body>
<html>