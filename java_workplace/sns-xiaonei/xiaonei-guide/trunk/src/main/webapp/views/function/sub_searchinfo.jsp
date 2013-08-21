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

<!-- sub_searchinfo -->

<!-- {------------ --> 
<script language="javascript">
function resetForm(){
	document.getElementById('s').value='';
	document.getElementById('fid').value='';
	document.getElementById('feed').value='';
	document.getElementById('photo_aid').value='';
	document.getElementById('photo_pid').value='';
	document.getElementById('message_s').value='';
	document.getElementById('blog_s').value='';
	document.getElementById('blog_bid').value='';
	document.getElementById('gossip_p').value='';
}
</script>

<%@page import="java.util.List"%><div></div>
	<h2>Search</h2>
	<form method="post" action="profile.jsp">
	    <fieldset>
	    <input type="text" id="s" name="id" value="<%=host==null?"":host.getId() %>" />
	    <input type="hidden" name="run" value="true">
	    <input type="submit" id="x" value="Search" />
	    <input type="button" id="x" value="Clear" onclick="resetForm()"/>
	    <br />
	    </fieldset>
	    <label for="minifeed"><span><input type="checkbox" name="minifeed"  id="minifeed"/>minifeed</span></label>
	    <label for="feed"><span><input type="checkbox" name="feed"  id="feed"/>feed</span></label>
	    <label for="friend"><span><input type="checkbox" name="friend"  id="friend"/>friend</span></label>
	    <br />
	    <label for="doing"><span><input type="checkbox" name="doing"  id="doing"/>doing</span></label>
	    <br />
	    <span><input size="10" type="input" name="opened"  value=""/> <label for="award"><span><input type="checkbox" name="award"  id="award"/>award</span></label> </span>
	    <br />
	    <label for="pageon"><span><input type="checkbox" name="pageon"  id="pageon"/>page</span></label>
	    <%if (chost!=null && isMan(chost.getId())){%>
		    <br />
	        <span><input size="10" type="text" name="gossip_p"  id="gossip_p" value="<%=request.getParameter("gossip_p")==null?"":request.getParameter("gossip_p") %>"/>
	        </span>
        <%}%>
        
	    <label for="gossip"><span><input type="checkbox" name="gossip"  id="gossip"/>gossip</span></label>
	    <%if (chost!=null && isMan(chost.getId())){%>
    	    <br />
    	    <span><input size="10" type="text" name="message_s"  id="message_s" value="<%=request.getParameter("message_s")==null?"":request.getParameter("message_s") %>"/></span>
    	    <label for="message"><span><input type="checkbox" name="message"  id="message"/>message</span></label>
	    <%}%>
	    <%if (chost!=null && isMan(chost.getId())){%>
		    <br />
    	    <span><input size="10" type="text" name="blog_s"  id="blog_s" value="<%=request.getParameter("blog_s")==null?"":request.getParameter("blog_s") %>"/></span>
    	    <span><input size="10" type="text" name="blog_bid"  id="blog_bid" value="<%=request.getParameter("blog_bid")==null?"":request.getParameter("blog_bid") %>"/></span>
    	<%}%>
    	<label for="blog"><span><input type="checkbox" name="blog"  id="blog"/>blog</span></label>
    	<%if (chost!=null && isMan(chost.getId())){%>
            <br />
            <span><input size="10" type="text" name="photo_aid"  id="photo_aid" value="<%=request.getParameter("photo_aid")==null?"":request.getParameter("photo_aid") %>"/></span>
            <span><input size="10" type="text" name="photo_pid"  id="photo_pid" value="<%=request.getParameter("photo_pid")==null?"":request.getParameter("photo_pid") %>"/></span>
        <%}%>
    	<label for="photo"><span><input type="checkbox" name="photo"  id="photo"/>photo</span></label>
    	<br />
        <%if (chost!=null && isMan(chost.getId())){%>
            <span><input size="10" type="text" name="iplog_date"  id="iplog_date" value="<%=request.getParameter("iplog_date")==null?"":request.getParameter("iplog_date") %>"/></span>
            <label for="iplog"><span><input type="checkbox" name="iplog"  id="iplog"/>iplog</span></label>
            <br />
        <%}%>
    	<label for="change"><span><input type="checkbox" name="change"  id="change"/>change</span></label>
    	<br />
	    Check Friend:<input type="text" id="fid" name="fid" value="<%=request.getParameter("fid")==null?"":request.getParameter("fid") %>" />
	    <br />
	    <%if (chost!=null && isMan(chost.getId())){%>
			Blog:(pageid,blogid)<br />
			Photo:(albumid,photoid)<br />
    	<%}%>
	</form>

<!-- {------------ --> 
<%

%>