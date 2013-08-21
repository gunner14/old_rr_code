<%@page contentType="text/html;charset=UTF-8" session="false"%>
<%@page import="java.util.List"%>
<!-- sub_mayknowninfo -->

<%@page import="com.xiaonei.reg.guide.root.miscs.MyStringUtils"%>
<%@page import="com.xiaonei.msg.home.MessageHome"%>
<%@page import="com.xiaonei.msg.view.MessageIn"%>
<%@page import="java.nio.charset.Charset"%>
<%@page import="java.util.Map"%>
<%@page import="java.util.ArrayList"%>
<%@page import="com.xiaonei.xce.offerfriends.OfferFriendsAdapter"%>
<%@page import="com.xiaonei.xce.offerfriends.DefaultOffer"%><div></div>
<%
if(!"true".equals(request.getParameter("run"))){
}
else if(host!=null){
%>

<!-- {------------ --> 

<h2 class="title">Mayknown Info</h2>
<p class="byline"><small>Posted by inshion</small></p>
<div class="entry">
<ul>
    <%
    List<Integer> ids = new ArrayList<Integer>();
    /*
    OfferFriendsAdapter<DefaultOffer> adapter = OfferFriendsAdapter.getInstance();
    if(adapter!=null){
        adapter.rebuild(host.getId());   
        List<DefaultOffer> offers = adapter.getBestOffer(host.getId(), 50);                             
        if (offers != null) {
            for (int i = 0; i < offers.size(); i++) {
                ids.add(offers.get(i).getUserId());
                DefaultOffer cof = offers.get(i);
                String outl = cof.getUserId()+":"+cof.getWeight();
                out.println(i+":"+outl+"<br />");
            }
        }
    }            
    */
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