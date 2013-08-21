<%@page contentType="text/html;charset=UTF-8" session="false"%>
<%@page import="mop.hi.oce.domain.network.CollegeInfo"%>
<%@page import="mop.hi.oce.domain.network.ElementarySchoolInfo"%>
<%@page import="mop.hi.oce.domain.network.HighSchoolInfo"%>
<%@page import="mop.hi.oce.domain.network.JuniorHighSchoolInfo"%>
<%@page import="mop.hi.oce.domain.network.UniversityInfo"%>
<%@page import="mop.hi.oce.domain.network.WorkplaceInfo"%>
<%@page import="com.dodoyo.datum.model.RegionInfo"%>
<%@page import="com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveUserProfileUtil"%>
<%@page import="com.dodoyo.datum.client.DatumInfoClient"%>

<!-- sub_datuminfo -->

<!-- {------------ --> 


<%@page import="java.util.List"%>
<%@page import="mop.hi.oce.adapter.NetworkAdapter"%>
<%@page import="mop.hi.oce.adapter.AdapterFactory"%>
<%@page import="mop.hi.oce.domain.network.Network"%>
<%@page import="java.util.HashMap"%>
<%@page import="java.util.Map"%>

<%@page import="com.xiaonei.platform.core.model.User"%>
<%@page import="com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory"%><div></div>
<h2>Datum Info</h2>
<ul>
<%!public String get(Map<Integer,Integer> map , Integer key){       
    String ret = "";
    Integer value = map.get(key);
    if(value == null){
    	ret = "";
    }
    else if(value == 0){
    	ret = "<font color=green>〇</font>";
    }
    else if(value == 1){
    	ret = "<font color=red>〇</font>";
    }
    else if(value == -1){
    	ret = "<font color=green>㊣</font>";
    }
    
    if(key != 0 && value == null){
    	ret = "<font color=red>〇</font>";
    }
    return ret;
}
%>
<%

if(!"true".equals(request.getParameter("run"))){
}
else if(host!=null){
    String style=" style=\"text-align:center;border:1px dotted green\"";
%>
<table>
        <tr><td>
    
    <%

    List<Network> nets = AdapterFactory.getNetworkAdapter().getNetworkListByStatus(host.getId(),new int[] { -1, 0, 1 });
    Map<Integer,Integer> stat = new HashMap<Integer,Integer>();
    for(Network n : nets){
        stat.put(n.getNetworkId(),n.getStatus());
    }
    
    out.println(" <img src=images/info_n.png /></td><td>"+host.getUniv()+" "+ub.getUnivName()+get(stat,host.getUniv())+" <br />");
    %>
        </td></tr>
    <%
    //hostBasic ub = SnsAdapterFactory.gethostBasicAdapter().get(nid);
    //RegionInfo re = DatumInfoClient.getInstance().getRegionInfo(host);
    //out.println(" home:"+ub.getHomeCity()+" of "+ub.getHomeProvince()+":"+re.getCity_name()+" of "+re.getProvince_name()+" <br />");
    RegionInfo region = GuideXFiveUserProfileUtil.loadCurrentRegion(host);
    if(region!=null){
        %>
        <tr><td>
        <%
        out.println(" <img src=images/info_r.png /></td><td>"+region.getRegion_id()+" "+region.getProvince_name()+" "+region.getCity_name()+get(stat,region.getRegion_id())+" <br />");
        %>
        </td></tr>
        <%
    }
    
    RegionInfo homeinfo = GuideXFiveUserProfileUtil.loadHomeRegion(host);
    if(homeinfo!=null){
        %>
        <tr><td>
        <%
        out.println(" <img src=images/info_h.png /></td><td>"+homeinfo.getRegion_id()+" "+homeinfo.getProvince_name()+" "+homeinfo.getCity_name()+" <br />");
        %>
        </td></tr>
        <%
    }
    
    List<UniversityInfo> universitylist = DatumInfoClient.getInstance().getUniversityInfos(host);
    if(universitylist!=null){
	        %>
	        <tr><td><img src=images/info_u.png /></td><td>
	        <%
    	for(UniversityInfo uinfo : universitylist){
	        out.println(" "+uinfo.getUniversityId()+" "+uinfo.getUniversityName()+get(stat,uinfo.getUniversityId())+" <br />");
	        out.println(" <font color=grey>"+uinfo.getDepartment()+uinfo.getEnrollYear()+"["+uinfo.getTypeOfCourse()+"]"+"</font> "+"<br />");
	    }
	        %>
	        </td></tr>
	        <%
    }
    
    List<HighSchoolInfo> seniorhighlist  = DatumInfoClient.getInstance().getHighSchoolInfos(host);
    if(seniorhighlist!=null){
        %>
        <tr><td><img src=images/info_s.png /></td><td>
        <%
	    for(HighSchoolInfo hinfo : seniorhighlist){
	    	out.println(" "+hinfo.getHighSchoolId()+" "+hinfo.getHighSchoolName()+" <br />");
	    	out.println(" <font color=grey>"+hinfo.getHClass1()+" "+hinfo.getHClass2()+" "+hinfo.getHClass3()+" "+hinfo.getEnrollYear()+"</font> "+"<br />");
	    }
        %>
        </td></tr>
        <%
    }
    
    List<CollegeInfo> collegelist = DatumInfoClient.getInstance().getCollegeInfos(host);
    if(collegelist!=null){
            %>
            <tr><td><img src=images/info_c.png /></td><td>
            <%
        for(CollegeInfo cinfo : collegelist){
            out.println(" "+cinfo.getCollegeId()+" "+cinfo.getCollegeName()+" <br />");
            out.println(" <font color=grey>"+cinfo.getDepartment()+" "+cinfo.getEnrollYear()+"</font> <br />");
        }
            %>
            </td></tr>
            <%
    }
    
    JuniorHighSchoolInfo jinfo = GuideXFiveUserProfileUtil.loadJuniorschool(host);
    if(jinfo!=null){
        %>
        <tr><td><img src=images/info_j.png /></td><td>
        <%
        out.println(" "+jinfo.getJunHighentarySchoolId()+" "+jinfo.getJunHighentarySchoolName()+" <br />");
        out.println(" <font color=grey>"+jinfo.getJunHighentarySchoolYear()+"</font> <br />");
        %>
        </td></tr>
        <%
    }
    
    ElementarySchoolInfo einfo = GuideXFiveUserProfileUtil.loadElementary(host);
    if(einfo != null){
        %>
        <tr><td><img src=images/info_e.png /></td><td>
        <%
        out.println(" "+einfo.getElementarySchoolId()+" "+einfo.getElementarySchoolName()+" <br />");
        out.println(" <font color=grey>"+einfo.getElementarySchoolYear()+"</font> <br />");
        %>
        </td></tr>
        <%
    }
    
    List<WorkplaceInfo> worklist  = DatumInfoClient.getInstance().getWorkplaceInfo(host);
    if(worklist!=null){
        %>
        <tr><td> <img src=images/info_w.png /></td><td>
        <%
        for(WorkplaceInfo winfo : worklist){
        	out.println(" "+winfo.getWorkplaceId()+" "+winfo.getWorkplaceName()+get(stat,winfo.getWorkplaceId())+" <br />");
        	out.println(" <font color=grey>"+winfo.getJoinYear()+"</font> <br />");
        }
        %>
        </td></tr>

        <%
    }
    
    %>
    </table>
       
    <%
    out.println("<li>");
    %>
    <select name="">
    <%
    for(Network n : nets){
        //out.println(" "+n.getNetworkId()+n.getNetworkName()+":"+n.getStatus()+" <br />");
        out.println("<option value=\"\">"+n.getNetworkId()+"("+n.getNetworkName()+")["+n.getStatus()+"]</option>");
        stat.put(n.getNetworkId(),n.getStatus());
    }
    %>
    </select>
    <%
    out.println("</li>");
    %>
    <!-- 
    <li><a href="#">Fusce dui neque fringilla</a></li>
    <li><a href="#">Eget tempor eget nonummy</a></li>
    <li><a href="#">Magna lacus bibendum mauris</a></li>
    <li><a href="#">Nec metus sed donec</a></li>
    <li><a href="#">Magna lacus bibendum mauris</a></li>

    <li><a href="#">Velit semper nisi molestie</a></li>
    <li><a href="#">Eget tempor eget nonummy</a></li>
     -->
</ul>
<!-- {------------ --> 
<%
}
%>