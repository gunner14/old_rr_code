<%@page contentType="text/html;charset=UTF-8" session="false"%>
<%@page import="java.util.List"%>
<!-- sub_messageinfo -->

<%@page import="com.xiaonei.reg.guide.root.miscs.MyStringUtils"%>
<%@page import="com.xiaonei.msg.home.MessageHome"%>
<%@page import="com.xiaonei.msg.view.MessageIn"%>
<%@page import="java.nio.charset.Charset"%>
<%@page import="java.util.Map"%>
<%@page import="com.xiaonei.msg.view.BaseMessage"%>
<%@page import="com.xiaonei.msg.view.NewMessage"%>
<%@page import="com.xiaonei.msg.view.SessionBody"%><div></div>
<%
if(!"true".equals(request.getParameter("run"))){
}
else if(host!=null && chost != null && isMan(chost.getId())){
%>

<!-- {------------ --> 

<h2 class="title">Message Info</h2>
<p class="byline"><small>Posted by inshion</small></p>
<div class="entry">
<ul>

    <%
    String message = request.getParameter("message");
    String start = request.getParameter("message_s");
    if(MyStringUtils.equals("on",message)){
	    int incount = MessageHome.getInstance().getMessageInCount(nid);
	    int nstart = (start == null)?0:(Integer)MyStringUtils.stringToObject(start,Integer.class);
	    out.println("InBox Count:"+incount+" Start:"+nstart+"<br />");
	    List<MessageIn> listin = MessageHome.getInstance().getMessageInList(host.getId(),nstart,10);
	    int message_id = nstart;
	    for(MessageIn ni: listin){
	        String guestName = ni.getGuestName();
	        if(guestName.endsWith(",")){
	        	guestName = guestName.substring(0,ni.getGuestName().length()-1);
	        }
	        request.setAttribute("gn",guestName);
	        out.println("<li>"+message_id+++":");
	        %>
	        
	        <script language="javascript">
	        var gn = ${requestScope.gn};
	        //document.getElementById('m_<%=message_id%>').innerHTML = gn.name;
	        document.write(gn.id+"["+gn.name+"]");
	        </script>
	        <%
	        out.println(":"+""+" "+ni.getSubject()+" "+ni.getTime()+"<br />");
	        out.println(" <font color=grey>("+ni.getBodySummary()+")</font></li>");
	        if(message_id -1  == nstart){
	        	BaseMessage m = MessageHome.getInstance().getMessage(0,host.getId(),ni.getId());
	        	if ( m instanceof NewMessage){
	        		NewMessage nm = (NewMessage) m;
	        		//MessageHome.getInstance().getSessionBody(ni.getSessionId(),);
	        		List<SessionBody> bodys = nm.getSessionBody();
	        		for(SessionBody sb : bodys){
	        			out.println("-"+sb.getSenderId()+"["+sb.getSenderName()+"]");
	        			out.println("<font color=green>"+sb.getBody()+"</font>"+"<br />");
	        		}
	        	}
	        	
	        }
	    }
    }
    %>
    <table>
        <tr><td>
    
        </td></tr>
    <%
    %>
    </table>
</ul>
</div>
<!-- {------------ --> 
<%
}
%>