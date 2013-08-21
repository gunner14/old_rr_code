<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ page import="org.apache.commons.lang.math.NumberUtils"%>
<%@ page import="com.xiaonei.reg.register.pojo.*"%>
<%@ page import="com.xiaonei.reg.register.logic.buildperson.*"%>
<%@ page import="com.xiaonei.platform.core.model.User"%>
<%@ page import="com.xiaonei.platform.core.opt.ice.*"%>
<%@ page import="com.xiaonei.reg.register.logic.additional.*"%>
<%@page import="com.xiaonei.reg.common.constants.Globals"%>
<%@page import="com.xiaonei.reg.guide.logic.StepStatusLogic"%>
<%@page import="com.xiaonei.reg.register.logic.pretreatment.RegFromSourceLogic"%>
<%@page import="com.xiaonei.reg.guide.dao.GuideUserDecisionDAO"%>
<%@page import="com.xiaonei.reg.guide.dao.GuideUserIdentityDAO"%>
<%@page import="java.util.Date"%>
<%@page import="com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory"%>
<%@page import="java.util.List"%>
<%@page import="com.xiaonei.reg.guide.root.logics.StepProcessLogic"%>
<%@page import="com.xiaonei.platform.core.model.UserBasic"%>
<%@page import="com.dodoyo.datum.model.RegionInfo"%>
<%@page import="com.dodoyo.datum.client.DatumInfoClient"%>
<%@page import="com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUseIdentityLogic"%>
<%@page import="com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUserDecisionLogic"%>
<%@page import="com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveUserProfileUtil"%>
<%@page import="mop.hi.oce.domain.network.CollegeInfo"%>
<%@page import="mop.hi.oce.domain.network.ElementarySchoolInfo"%>
<%@page import="mop.hi.oce.domain.network.HighSchoolInfo"%>
<%@page import="mop.hi.oce.domain.network.JuniorHighSchoolInfo"%>
<%@page import="mop.hi.oce.domain.network.UniversityInfo"%>
<%@page import="mop.hi.oce.domain.network.WorkplaceInfo"%>
<%@page import="com.xiaonei.sns.platform.core.opt.ice.impl.WUserTimeAdapter"%>
<%@page import="com.xiaonei.platform.core.model.UserTime"%>
<%@page import="java.util.Map"%>
<%@page import="com.xiaonei.reg.register.model.RegExtraMsg"%>
<%@page import="com.xiaonei.platform.component.friends.home.FriendsHome"%>
<%@page import="com.xiaonei.reg.guide.root.miscs.MyStringUtils"%>
<%@page import="com.xiaonei.platform.core.cache.MemCacheManager"%>
<%@page import="com.xiaonei.platform.core.cache.MemCacheFactory"%>
<%@page import="com.xiaonei.platform.core.cache.MemCacheKeys"%>
<%@page import="com.xiaonei.platform.core.model.WUserCache"%>
<%@page import="java.util.Map.Entry"%>
<%@page import="com.xiaonei.platform.core.opt.base.BaseThreadUtil"%>
<%@page import="com.xiaonei.platform.core.opt.base.PassportManager"%>
<%@page import="com.xiaonei.platform.component.application.facade.AppFacade"%>
<%@page import="com.xiaonei.reg.guide.logic.fillinfo.GuideUserProfileLogic"%>
<%@page import="com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUserInfoLogic"%>
<%@page import="com.xiaonei.reg.guide.root.interfaces.IInfoJuniorhighschool"%>

<%
String curip = BizFilterUtil.getClientIP(request);
try{
    //out.println("<p>your ip is:"+curip+"</p>");
    if(curip!=null && curip.trim().startsWith("10.")){
        //good
    }
    else{
        out.println("Inner Required");
        return;
    }
}
catch(Exception e){
    
}
%>
<%
User chost = null;
String cdomain = request.getHeader("Host").trim();
if(!MyStringUtils.contains(cdomain,"renren.com")){
	return;
}
try{
	String xn_sig_app_id = request.getParameter("xn_sig_app_id");
	String xn_sig_added = request.getParameter("xn_sig_added");
	if(MyStringUtils.equals("92436",xn_sig_app_id)){
		if(MyStringUtils.equals("0",xn_sig_added)){
			%>
			<script>top.location.href="http://app.renren.com/apps/tos.do?v=1.0&api_key=22a060195c85440da153cd32c2a3cb46"</script> 
			<%
		}
	}
    Integer chostid = PassportManager.getInstance().getHostIdFromCookie(request, response);
    if(chostid == null || chostid == 0){
    	String xn_sig_user = request.getParameter("xn_sig_user");
    	if(xn_sig_user!=null){
    		chostid = Integer.parseInt(xn_sig_user);
    	}
    }
    if(chostid != null && chostid != 0){
    	chost = SnsAdapterFactory.getUserAdapter().get(chostid);
    }
    
    try{
        //out.println("<p>your ip is:"+curip+"</p>");
        if(curip!=null && curip.trim().startsWith("10.")){
            //good
        }
        else if(isMan(chost.getId())){
        	
        }
        else{
            out.println("Inner Required");
            return;
        }
    }
    catch(Exception e){
        
    }
}
catch(Exception e){
    //e.printStackTrace();
}
if(!Globals.DEBUG_ENV){
	 if(isMan(chost.getId())
		|| isUser(chost)	 
	 ){
	        
	    }
	    else{
	        out.println("");
	        return;
	    }
}
if(chost == null){
	out.println("庭君 请先Login");
    return;
}
%>

<%
    String sid = "";//request.getParameter("id");
    int nid = 0;
    User host = null;
    UserBasic ub = null;
    if(!"true".equals(request.getParameter("run"))){
    }
    else{
        sid = request.getParameter("id");
        nid = 0;
        try{
            nid = Integer.parseInt(sid);
        }
        catch(Exception e){
        }
        if(nid!=0){
        	try{
        	    host = SnsAdapterFactory.getUserAdapter().get(nid);
        	}
        	catch(Exception e){
        		out.println("查无此人");
        	}
        }
        else if(!MyStringUtils.isNullOrEmpty(sid) && chost!=null && hasChi(sid)){ //有汉字
            Map<Integer, WUserCache> fmap = FriendsHome.getInstance().getFriends(chost.getId());
            WUserCache targetf = null;
            for (Entry<Integer, WUserCache> entry : fmap.entrySet()) {
                WUserCache curf = entry.getValue();
                if(curf!=null && curf.getName() != null){
                    if(curf.getName().contains(sid)){
                        targetf = curf;
                        break;
                    }
                }
            }
            if(targetf != null){
            	host = SnsAdapterFactory.getUserAdapter().get(targetf.getId());
            }
            else{
            	out.println("无此好友");
            }
        }
        else if(!MyStringUtils.isNullOrEmpty(sid)){            
            //System.out.println(4+":"+sid);
            try{
	            host = SnsAdapterFactory.getUserAdapter().getByEmail(sid);
            }
            catch(Exception e){
            	
            }
            
            //host = com.xiaonei.platform.core.opt.ice.WUserAdapter.getInstance().getByEmail(sid);
        }
        if(host!=null){
        	try{
	            ub = WUserBasicAdapter.getInstance().get(host.getId());
        	}
        	catch(Exception e){
        		
        	}
        }
    }
%>


<%@page import="com.xiaonei.platform.core.opt.base.chain.impl.BizFilterUtil"%><html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <link href="style.css" rel="stylesheet" type="text/css" media="screen" >
    <script type="text/javascript" src="http://s.xnimg.cn/jspro/base.js?ver=$revxxx$"></script>    
    <script>
    XN.config.jumpOut = false;
    </script>

    <script>

	XN.getFileVersion( [ 'http://s.xnimg.cn/jspro/xn.app.importFriendsFromKaixin.js?ver=$revxxx$' ]);    
XN.dynamicLoad(
{
		file :'http://s.xnimg.cn/jspro/xn.app.share.js?ver=$revxxx$',
		funcs : [ 'create_share_div', 'create_share_feed', 'create_share_edm',
				'create_share_popup' ]
	});

    function  getReplyOfDoingFromJSON (a , b ,c , d,e,f,g,h) {
    }
	function hideInfo() {
		XN.cookie.set("homeNtcInf", "0", 10000, "/", ".renren.com");
		if ($('info'))
			$('info').hide();
	}
	function showInfo() {
		if (XN.cookie.get("homeNtcInf") == null) {
			if ($('info'))
				$('info').show();
		}
	}
	XN.dom.ready(showInfo);
	XN.namespace('user');
	XN.user.id = '81120';
	XN.user.tinyPic = 'http://hdn.xnimg.cn/photos/hdn311/20090517/0430/tiny_7ddY_77325p204234.jpg';
	XN.user.name = "%E9%93%B6%E9%91%AB";
	function closewarn() {
		new Ajax.Request("closewarn.do", {
			method :'get'
		});
	}
	function close_info_complete(from) {
		new Ajax.Request("ajaxCloseInfo.do?from=" + from, {
			method :'post'
		});
	}
	function ajax_stat_link(action, from) {
		new Ajax.Request("ajaxStatLink.do?action=" + action + "&from=" + from,
				{
					method :'post'
				});
	}
	function ajax_stat_page(uid) {
		new Ajax.Request(
				"http://page.renren.com/receiveLogRequest/get?ref=hometab&uid="
						+ uid, {
					method :'post'
				});
	}
	( function() {
		feed_imgs = [];
		status_imgs = [];
		var img_index = 0;
		var i_am_out = false;
		var timer = null;
		var is_feed_delay_ing = true;
		if (!XN.browser.IE && !XN.browser.Gecko)
			is_feed_delay_ing = false;
		function feed_img_load(img) {
			if (!img)
				return;
			if (img._loaded)
				return;
			img._loaded = true;
			img.setAttribute('src', img.getAttribute('lala'));
			img.removeAttribute('lala');
			img.removeAttribute('onload');
			if (img.getAttribute('needclip'))
				clipImage(img);
		}
		function pop_imgs(imgs) {
			if (!imgs)
				return false;
			if (img_index >= imgs.length)
				return false;
			var img = imgs[img_index];
			setTimeout( function() {
				feed_img_load(img);
			}, 0);
			return true;
		}
		function stop() {
			if (timer) {
				clearInterval(timer);
				timer = null;
			}
			feed_imgs = status_imgs = null;
			is_feed_delay_ing = false;
		}
		function scroll_show_imgs() {
			var f = pop_imgs(feed_imgs);
			var s = pop_imgs(status_imgs);
			if (!f && !s) {
				stop();
				return;
			}
			img_index++;
		}
		feed_delay_load_all = function() {
			XN.array.each(feed_imgs, function(i, v) {
				feed_img_load(v);
			});
			XN.array.each(status_imgs, function(i, v) {
				feed_img_load(v);
			});
			stop();
		}
		if (XN.browser.IE || XN.browser.Gecko) {
			var c_scroll = XN.event.scrollTop();
			timer = setInterval( function() {
				var scroll = XN.event.scrollTop();
				if (c_scroll != scroll) {
					var ss = Math.abs(c_scroll - scroll);
					c_scroll = scroll;
					if (ss > 300) {
						feed_delay_load_all();
					} else {
						scroll_show_imgs();
						scroll_show_imgs();
					}
				}
			}, 100);
		}
		feed_img_delay_load = function(img, flag) {
			if (!is_feed_delay_ing) {
				feed_img_load(img);
				return;
			}
			if (flag) {
				var imgs = status_imgs;
			} else {
				var imgs = feed_imgs;
			}
			img.removeAttribute('onload');
			imgs.push(img);
			if (i_am_out)
				return;
			var t = XN.element.realTop(img);
			var h = XN.event.winHeight() + XN.event.scrollTop();
			if (t <= h) {
				feed_img_load(img);
			} else {
				i_am_out = true;
			}
		}
	})();

	function renderStatusFeed(a,b){
	}
</script>
<script type="text/javascript">
XN.getFileVersion(
[
'http://s.xnimg.cn/csspro/module/school-selector.css?ver=$revxxx$',
'http://s.xnimg.cn/csspro/apps/guide-register-home.css?ver=$revxxx$',
'http://s.xnimg.cn/jspro/xn.ui.schoolSelectors.js?ver=$revxxx$',
'http://s.xnimg.cn/jspro/xn.ui.currentPlace.js?ver=$revxxx$',
'http://s.xnimg.cn/jspro/xn.page.tempProfile.js?ver=$revxxx$'
]);
XN.dom.ready(function(){
new  XN.net.xmlhttp({
url : 'http://friend.' + XN.env.domain + '/ajax_need_modify_datum',
onSuccess : function( r ){
var j = XN.json.parse( r.responseText );
if( j.needModify )  
XN.loadFile( XN.env.staticRoot + 'jspro/xn.page.tempProfile.js',function(){
window.tempModifyProfile.init( j.datum );
}); 
}
})
},true)
</script>
    <title>人人 profile - by Inshion</title>
</head>

<body style="overflow:scroll;overflow-x:hidden">
<!--header start -->
<div id="feedHome" style="display:nont"></div>
<div id="header">
    <div id="logo">
        <h1><a href="#">Renren Profile</a></h1>
        <p><a href="?app=refresh">Produced</a> by Inshion [<a href="http://byebyestar.ycool.com/"><font color=gray>no name town</font></a>]<font color=white> [<%=request.getParameter("xn_sig_user") %>] [<%=chost==null?"null":""+chost.getId() %>] </font></p>

    </div>
    <!-- end #logo -->
    <div id="menu">
        <ul>
            <li class="first"><a href="#" onclick="top.location.href='http://apps.renren.com/inshion?origin=103'">Home</a></li>
            <!-- 
            <li><a href="#">About Us</a></li>
            <li><a href="#">Products</a></li>
            <li><a href="#">Services</a></li>
            <li><a href="#">Contact Us</a></li>
             -->
        </ul>
    </div>
    <!-- end #menu -->
</div>
<!-- 123:${notice}-->
<!-- end #header -->

<div id="page">
    <div id="content">
        <div class="post">
            <%@ include file="sub_changeinfo.jsp"%>
        </div>
        <div class="post">
            <!-- 
            <div class="entry">
                <p><strong>Black and White</strong> is a free template from <a href="http://www.freecsstemplates.org/">Free CSS Templates</a> released under a <a href="http://creativecommons.org/licenses/by/2.5/">Creative Commons Attribution 2.5 License</a>. You're free to use this template for both commercial or personal use. I only ask that you link back to <a href="http://www.freecsstemplates.org/">my site</a> in some way. Enjoy :)</p>

            </div>
            <div class="meta">
                <p class="links"><a href="#" class="comments">Comments (32)</a> &nbsp;&bull;&nbsp;&nbsp; <a href="#" class="more">Read full post &raquo;</a></p>
            </div>
             -->
            <%@ include file="sub_userinfo.jsp"%>
        </div>
        <div class="post">
            <%@ include file="sub_guideinfo.jsp"%>
        </div>
        <div class="post">
            <%@ include file="sub_awardinfo.jsp"%>
        </div>
        <div class="post">
        </div>
        <div class="post">
            <%@ include file="sub_doinginfo.jsp"%>
        </div>
        <div class="post">
            <%@ include file="sub_feedinfo.jsp"%>
        </div>
        <div class="post">
            <%@ include file="sub_minifeedinfo.jsp"%>
        </div>
        <div class="post">
            <%@ include file="sub_bloginfo.jsp"%>
        </div>
        <div class="post">
            <%@ include file="sub_photoinfo.jsp"%>
        </div>
        <div class="post">
            <%@ include file="sub_gossipinfo.jsp"%>
        </div>
        <div class="post">
            <%@ include file="sub_messageinfo.jsp"%>
        </div>
        <div class="post">
            <%@ include file="sub_pageinfo.jsp"%>
        </div>
        <div class="post">
            <%@ include file="sub_iploginfo.jsp"%>
        </div>
        <div class="post">
            <%@ include file="sub_mayknowninfo.jsp"%>
        </div>
        <div class="post">
            <%@ include file="sub_historyinfo.jsp"%>
        </div>
    </div>
    <!-- end #content -->
    <div id="sidebar">
        <div id="sidebar-bgtop"></div>
        <div id="sidebar-content">
            <ul>
                <li id="search">
                    <%@ include file="sub_searchinfo.jsp"%>
                </li>
                <li>
                    <%@ include file="sub_logininfo.jsp"%>
                </li>
                <li>
                    <%@ include file="sub_datuminfo.jsp"%>
                </li>
                <li>
                    <%@ include file="sub_stateinfo.jsp"%>
                </li>
                <li>
                    <%@ include file="sub_friendinfo.jsp"%>
                </li>
                <li>
                    <%@ include file="sub_requestinfo.jsp"%>
                </li>
                <li>
                    <%@ include file="sub_footinfo.jsp"%>
                </li>
                <li id="search">
                    <%@ include file="sub_otherinfo.jsp"%>
                </li>
                <li id="search">
                    <%@ include file="sub_whatsnewinfo.jsp"%>
                </li>

            </ul>
        </div>
        <div id="sidebar-bgbtm"></div>
    </div>
    <!-- end #sidebar -->

</div>
<!-- end #page -->
<div id="footer">
    <p>&copy; 2010. Inshion. Copy From <a href="http://www.freecsstemplates.org/">Free CSS Templates</a>.</p>
</div>
<!-- end #footer -->
<script type="text/javascript" src="http://s.xnimg.cn/a9703/jspro/xn.app.feed.js"></script>
<script type="text/javascript" src="http://s.xnimg.cn/a10950/jspro/xn.page.home.js"></script>
<%
String value = com.xiaonei.platform.core.opt.base.CookieManager.getInstance().getCookie(request, "userid");

%>
</body>
</html>
<%!public static int appid= 92436;%>
<%!public static String notice = "";%>
<%!public final static String MEM_CACHE_KEY = "profile_page_key_inshion_notice";%>
<%!public static String getNotice(){
	String ret = "";
	try{
        MemCacheManager mem = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_org);
        Object notice_obj = mem.get(MEM_CACHE_KEY);
        if(notice_obj != null){
            ret = (String)notice_obj;
        }
        else{
            ret = AppFacade.getInstance().getAppBase(appid).getAppNotice();
            mem.set(MEM_CACHE_KEY,notice);
        }
        //System.out.println("notice:"+notice);
    }
    catch(Exception e){
        
    }
    return ret;
}
%>
<%!public static void reloadAppInfo(){
	try{
		MemCacheManager mem = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_org);
	    notice = AppFacade.getInstance().getAppBase(appid).getAppNotice();
	    mem.set(MEM_CACHE_KEY,notice);
        //System.out.println("notice:"+notice);
    }
    catch(Exception e){
        
    }
}
%>
<%!static{
	//System.out.println(appid+" load");
	reloadAppInfo();
}
%>
<%
if(MyStringUtils.equals("refresh",request.getParameter("app"))){
	//System.out.println(appid+" reload");
	reloadAppInfo();
}
%>
<%!public boolean isUser(User user){
	if(user == null) return false;
	//IInfoJuniorhighschool info = GuideXFiveUserInfoLogic.getInstance().getInfoJuniorhighschool(user);
	if(user.getId() == 232686677
			|| user.getId() == 81120
			|| user.getId() == 248539544
			|| user.getId() == 221347887
			){
		return true;
	}
	return false;
}
%>
<%!public boolean isMan(int id){
	//System.out.println("notice:"+notice);
	
    if(false 
    	//|| id == 81120
        || id == 290293234
        || getNotice().contains(""+id)
    ){
        return true;
    }
    return false;
}
%>
<%!public boolean hasChi(String str){  
    boolean isGB2312=false;   
    if(!MyStringUtils.isNullOrEmpty(str)){
    	isGB2312 = ((str.getBytes().length == str.length())?false:true);
    }
    return isGB2312;   
}  
%>