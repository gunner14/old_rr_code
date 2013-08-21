<%@page contentType="text/html;charset=UTF-8" session="false"%>
<%@page import="java.util.List"%>
<!-- sub_gossipinfo -->

<%@page import="com.xiaonei.reg.guide.root.miscs.MyStringUtils"%>
<%@page import="java.nio.charset.Charset"%>
<%@page import="java.util.Map"%>
<%@page import="com.xiaonei.platform.component.gossip.home.GossipHome"%>
<%@page import="com.xiaonei.platform.component.gossip.view.GossipView"%>
<%@page import="com.xiaonei.ugc.ubb.UBBLogic"%>
<%@page import="com.xiaonei.platform.core.opt.OpiConstants"%><div></div>
<%
if(!"true".equals(request.getParameter("run"))){
}
else if(host!=null){
%>

<!-- {------------ --> 

<h2 class="title">Gossip Info</h2>
<p class="byline"><small>Posted by inshion</small></p>
<div class="entry">
<ul>
    <%
    if(host.getId() == 81120 && false){
    %>
    <%

       
    %>
            
    <%
    }
    %>
    <%
    String show = request.getParameter("gossip");
    String gossip_p = request.getParameter("gossip_p");
    int gossip_np = 0;
    try{
    	gossip_np = Integer.parseInt(gossip_p);
    }
    catch(Exception e){
    	
    }
    int uid = host.getId();
    if(MyStringUtils.equals("on",show)){
    	int total = GossipHome.getInstance().getGossipTotal(uid,uid);
    	out.println("<li>Total:"+total+" Current:"+gossip_np+"</li>");
	    List<GossipView> gls = GossipHome.getInstance().getGossipList(host,host,gossip_np,10);
	        out.println("<table>");
	    int i = 0;
	    for(GossipView g : gls){
	        String wi = g.isWhisper()?"<font color=gray>[悄悄话]</font>":"";
	        if(g.isWhisper()){
	        	if(chost==null || !isMan(chost.getId())){
	        		continue;
	        	}
	        }
	        i++;
		    String back = i%2==0?" style='background:#F3FAFF'":" style='background:#FFFFCC'";
	        //out.println("<li><font color='#0099FF'>["+g.getTime()+"]</font><br /><img src=\""+g.getTinyUrl()+"\" />["+g.getGuestName()+"]"+UBBLogic.getInstance(OpiConstants.urlStatic).replaceForVip(g.getBody(), UBBLogic.GOSSIP)+" "+wi+"</li>");
	        out.println("<tr"+back+">");
		        out.println("<td>");
	        if(g.isGift()){
	        	out.println("<img src="+g.getGiftImg()+" />");
	        }
		        out.println(""+g.getGuestName()+"["+g.getGuestId()+"]"+"<font color='#0099FF'>"+" ["+g.getTime()+"]</font>"+""+"");
		        out.println("</td>");
		        out.println("<td  rowspan=2>");
		        out.println("<font color='#0099FF'><img src=\""+g.getTinyUrl()+"\" />");
		        out.println("</td>");
	        out.println("</tr>");
            out.println("<tr"+back+">");
		        out.println("<td>");
		        out.println(UBBLogic.getInstance(OpiConstants.urlStatic).replaceForVip(g.getBody(), UBBLogic.GOSSIP)+" "+wi);
		        out.println("</td>");
            out.println("</tr>");
	    }
	        out.println("</table>");
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