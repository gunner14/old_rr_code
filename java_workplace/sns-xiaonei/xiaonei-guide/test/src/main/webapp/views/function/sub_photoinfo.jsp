<%@page contentType="text/html;charset=UTF-8" session="false"%>
<%@page import="java.util.List"%>
<!-- sub_photoinfo -->

<%@page import="com.xiaonei.reg.guide.root.miscs.MyStringUtils"%>
<%@page import="java.nio.charset.Charset"%>
<%@page import="java.util.Map"%>
<%@page import="com.renren.photo.xoa.api.service.PhotoListService"%>
<%@page import="com.renren.xoa.lite.ServiceFuture"%>
<%@page import="com.renren.xoa.lite.ServiceFactory"%>
<%@page import="com.renren.photo.xoa.api.service.AlbumListService"%>
<%@page import="com.renren.xoa.lite.ServiceFactories"%>
<%@page import="com.renren.xoa.lite.ServiceFutureHelper"%>



<%@page import="com.xiaonei.album.home.AlbumHome"%>
<%@page import="com.xiaonei.platform.core.opt.permission.define.RelationDef"%>
<%@page import="com.renren.ugc.model.album.Album"%>
<%@page import="com.renren.ugc.model.album.Photo"%><div></div>
<%
if(!"true".equals(request.getParameter("run"))){
}
else if(host!=null){
%>

<!-- {------------ --> 

<h2 class="title">Photo Info</h2>
<p class="byline"><small>Posted by inshion</small></p>
<div class="entry">
<ul>
    <%
    String photo = request.getParameter("photo");
    String photo_aid = request.getParameter("photo_aid");
    String photo_pid = request.getParameter("photo_pid");
    int n_aid = (photo_aid == null)?0:(Integer)MyStringUtils.stringToObject(photo_aid,Integer.class);
    int n_pid = (photo_aid == null)?0:(Integer)MyStringUtils.stringToObject(photo_pid,Integer.class);
    if(MyStringUtils.equals("on",photo)){
    	//AlbumHome.getInstance().getAlbumCount4Profile(host.getId(), host.getId(), RelationDef.uuFriend);
    	//List<Album> album_list = AlbumHome.getInstance().getAlbumGeneral(host.getId());
		ServiceFactory fact = ServiceFactories.getFactory();
    	AlbumListService albumListService=fact.getService(AlbumListService.class);
    	ServiceFuture<com.renren.ugc.model.album.Album[]> al = albumListService.getAlbumsPhotosLessCount(host.getId(),host.getId(),Album.LIMIT * 2);   
        final com.renren.ugc.model.album.Album[] albums = ServiceFutureHelper.execute(al, 80);
    	out.println("<font color=white>Album_Count:"+albums.length+"</font>");
    	out.println("<table>");
    	int album_count=0;
    	for(Album a : albums){
    		if(a.getControl() == -1 && !isMan(chost.getId())){
    			continue;
    		}
    		album_count++;
    		out.println("<tr>");
    		out.println("<td>");
    		out.println("<img src="+a.getHeadUrl()+" />");
    		out.println("</td>");
    		out.println("<td>");
    		out.println(album_count+" "+a.getName()+":<font color='gray'>"+"("+a.getId()+")"+a.getDescription()+"("+a.getPhotoCount()+")"+"["+a.getControl()+"]</font>");
    		out.println("</td>");
    		out.println("</tr>");
    		//out.println("<br />");
	   		if(n_aid != 0 && n_aid == a.getId()){
	   		    out.println("<tr border=1><td colspan=2>");
	    		//List<Photo> plist = AlbumHome.getInstance().getPhotoListInAlbum(host.getId(),a.getId(),n_pid,1);
				fact = ServiceFactories.getFactory();
                PhotoListService photoListService = fact.getService(PhotoListService.class);
                final ServiceFuture<Photo[]> serviceFuture = photoListService.getPhotosByPage(host.getId(), host.getId(), a.getId(),
                n_pid, 1, "");
                final Photo[] photos = ServiceFutureHelper.execute(serviceFuture, 80);
                int size=photos.length;
                Photo pt=null;
	    		for(int i=0;i<size;i++){
	    		    pt=photos[i];
	    			out.println("<font color=green>");
	    			out.println("<a href='"+pt.getLargeUrl()+"' target='_blank'>"+"<img src="+pt.getLargeUrl()+" width=100% />"+"</a>");
	    		    out.println("<!-- TinyUrl:"+pt.getTinyUrl()+"<br />");
	    		    out.println("MainUrl:"+pt.getMainUrl()+"<br />");
	    		    out.println("LargeUrl:"+pt.getLargeUrl()+"<br />");
	    		    out.println("LargeUrl:"+pt.getLargeUrl()+"<br />");
	    		    out.println("DisplayTitle:"+pt.getDisplayTitle()+"<br /> -->");
	    		    out.println(""+pt.getFullDisplayTitle()+"<br />");
	    		    out.println("CommentCount:"+pt.getCommentCount()+"<br />");
	    		    out.println("SharedDate:"+pt.getSharedDate()+"<br />");
	    		    out.println("</font>");
	    		    //out.println("LinkUrl:"+pt.getLinkUrl()+":"+pt.getLinkUrl());
	    		}
	    	    out.println("</tr></td>");
	   		}
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