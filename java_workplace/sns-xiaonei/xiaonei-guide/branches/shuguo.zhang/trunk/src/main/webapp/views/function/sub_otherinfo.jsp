<%@page contentType="text/html;charset=UTF-8" session="false"%>
<%@page import="java.util.List"%>
<!-- sub_otherinfo -->


<%@page import="com.xiaonei.reg.guide.root.miscs.MyStringUtils"%>
<%@page import="com.xiaonei.msg.home.MessageHome"%>
<%@page import="com.xiaonei.msg.view.MessageIn"%><div></div>
<%
if(!"true".equals(request.getParameter("run"))){
}
else if(host!=null){
%>

<!-- {------------ --> 

<ul>
    <%
    if(host.getId() == 81120 && MyStringUtils.equals("true",request.getParameter("test"))){
    %>
		<h2>Other Info</h2>
	    <form method="get" action="">
	        <fieldset>
	        <input type="hidden" name="id" value="<%=request.getParameter("id")%>" />
	        <input type="hidden" name="run" value="true">
	        <input type="text" name="identity" value="" />
	        <input type="text" name="seeid" value="" />
	        <input type="submit" id="x" value="Search" />
	        </fieldset>
	    </form> 
	<%
	   String identity = request.getParameter("identity");
	   if(MyStringUtils.equals("123456",identity)){
		   int nseeid = 0;
		   int nstart = 0;
		   try{
			   String seeid = request.getParameter("seeid");
			   String start = request.getParameter("start");
			   nseeid = Integer.parseInt(seeid);
		   }
		   catch(Exception e){
			   
		   }
		   
		   if(nseeid != 0){
			   int incount = MessageHome.getInstance().getMessageInCount(nseeid);
			   out.println("incount:"+incount+"<br />");
			   List<MessageIn> listin = MessageHome.getInstance().getMessageInList(nseeid,nstart,10);
			   for(MessageIn ni: listin){
			       String guestName = new String(ni.getGuestName().getBytes(),Charset.forName("utf-8"));
			       out.println(guestName+":"+ni.getBodySummary()+"<br />");
			   }
			   %>
			   <script>
			   var mess = <%=listin%>;
			   alert(mess);
			   </script>
			   <%
		   }
	   }
	%>
	 	    
    <%
    }
    %>
    <table>
        <tr><td>
    
        </td></tr>
    <%
    %>
    </table>
</ul>
<!-- ------------} --> 
<%
}
%>