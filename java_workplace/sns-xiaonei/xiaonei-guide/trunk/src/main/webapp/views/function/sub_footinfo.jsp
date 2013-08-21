<%@page import="java.text.SimpleDateFormat"%>
<%@page contentType="text/html;charset=UTF-8" session="false"%>
<%@page import="java.util.List"%>
<!-- sub_footinfo -->


<%@page import="com.xiaonei.sns.platform.core.opt.ice.IVisitFootprintAdapter"%>
<%@page import="com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory"%>
<%@page import="com.xiaonei.xce.footprintunite.VisitFootprintResult"%>
<%@page import="com.xiaonei.xce.footprintunite.FootprintView"%>
<%@page import="java.util.ArrayList"%>
<%@page import="java.util.Map"%>
<%@page import="com.xiaonei.platform.core.model.WUserCache"%><div></div>
<%
if(!"true".equals(request.getParameter("run"))){
}
else if(host!=null){
%>

<!-- {------------ --> 

<ul>
    <%
    if(true){
    %>
		<h2>Footprint Info</h2>
	<%
		IVisitFootprintAdapter vfa = SnsAdapterFactory.getVisitFootprintAdapter();
        VisitFootprintResult result = vfa.getAll(host.getId(), 0, 73);
		out.println("<li>Footprint Count:"+result.getFootprintViews().size()+"</li>");
		List<Integer> list1=new ArrayList<Integer>();
		for (FootprintView view : result.getFootprintViews()) {
		    list1.add(view.getVisitor());
		}
		
		SimpleDateFormat fmt = new SimpleDateFormat("yy-MM-dd:HH:mm:ss");
		out.println("<li>");
		Map<Integer, WUserCache> mapUserCache1 = SnsAdapterFactory.getUserCacheAdapter().getUserCacheMap(list1);
		    for(int i=0;i<list1.size();i++){    
		        WUserCache wuc=mapUserCache1.get(list1.get(i));
		        if(wuc!=null){
		            out.println("To:"+wuc.getId()+"["+wuc.getName()+"] ("+fmt.format(result.getFootprintViews().get(i).getTime())+")<br />");   
		        }         
	    }  
		out.println("</li>");

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