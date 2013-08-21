<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html"%>
<%@ taglib uri="http://www.renren.com/tags-core" prefix="xiaonei"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
	<meta http-equiv="X-UA-Compatible" content="IE=EmulateIE7" />
	<meta name="Description" content="人人,人人网是一个真实的社交网络，联络你和你周围的朋友。加入人人,人人网你可以:联络朋友，了解他们的最新动态；和朋友分享相片、音乐和电影；找到老同学，结识新朋友；用照片和日志记录生活,展示自我。" />
	<meta name="Keywords" content="Xiaonei,人人,人人,大学,同学,同事,白领,个人主页,博客,相册,群组,社区,交友,聊天,音乐,视频,校园,人人,人人网" />
	<link href="http://s.xnimg.cn/csspro/old/base.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
	<link href="http://s.xnimg.cn/csspro/base/layout.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
	<link href="http://s.xnimg.cn/csspro/apps/guide_flash_canteen.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
	<link href="${applicationScope.urlSmallStatic}/favicon-${domainIdentified["siteNameAbbr"]}.ico?ver=2" rel="shortcut icon" type="image/x-icon" />

	<!--[if lte IE 6]><style type="text/css" media="screen">/* <![CDATA[ */ @import url(http://s.xnimg.cn/csspro/patch/ie6.css?ver=$revxxx$); /* ]]> */</style><![endif]--><!--[if gte IE 7]><style type="text/css" media="screen">/* <![CDATA[ */ @import url(http://s.xnimg.cn/120444/patch/ie7.css?ver=$revxxx$); /* ]]> */</style><![endif]-->
	<script type="text/javascript">
		//XN = {env:{domain:'renren.com',shortSiteName:'人人',siteName:'人人网'}};
	</script>
	
	<script src="http://s.xnimg.cn/jspro/base.js?ver=$revxxx$" type="text/javascript"></script>
   	<script src="http://s.xnimg.cn/jspro/xn.ui.schoolSelectors.js?ver=$revxxx$" type="text/javascript"></script>
    <script src="http://s.xnimg.cn/jspro/xn.ui.currentPlace.js?ver=$revxxx$" type="text/javascript"></script>
    <script src="http://s.xnimg.cn/jspro/guide/swfobject.js?ver=$revxxx$" type="text/javascript"></script>
    <script src="http://s.xnimg.cn/jspro/xn.ui.pageTools.js?ver=$revxxx$" type="text/javascript"></script>
	<script src="http://s.xnimg.cn/jspro/guide/xn.flash.jsproxy.js?ver=$revxxx$" type="text/javascript"></script>
	<!-- 
		 	
	    
	 -->
	
	<script type="text/javascript">
	XN.getFileVersion(
	[
	'http://s.xnimg.cn/csspro/apps/profile.css?ver=$revxxx$',
	'http://s.xnimg.cn/jspro/xn.app.recommendFriend.js?ver=$revxxx$',
	'http://s.xnimg.cn/jspro/xn.ui.pager.js?ver=$revxxx$',
	'http://s.xnimg.cn/jspro/xn.ui.multiFriendSelectorBox.js?ver=$revxxx$',
	'http://s.xnimg.cn/csspro/module/friendSelector.css?ver=$revxxx$'
	]);
	</script>
 
	<title>${domainIdentified["siteName"]}网 </title>
</head>

<body id="conteen">
<div id="container">
	<div id="header">
		<h1 id ="logo"><a href="http://www.renren.com">人人网</a></h1>
		<ul id="gift_menu" class="clearfix">
			<li id="field"><a href="#"></a></li>
			<li id="hand"><a href="#"></a></li>
			<li id="bottle"><a href="#"></a></li>
			<li id="final"><a href="#"></a></li>
		</ul>
	</div>
	<div>
    	<p id="work_cur_p" style="display:none;">
            <label>
               	所在地 : 
            </label>
            <select name="homeprovince" id="work_cur" class="select-short cur-plc">
                <option value="">省份</option>
            </select>
            <select id="work_cur_city" class="select-short">
                <option value="">城市</option>
            </select>
            <input type="hidden" name="homecitycode" value="0" id="work_cur_city_code"><input type="hidden" name="homecityname" value="" id="work_cur_city_name">
        </p>
        <p id="none_cur_p" style="display:none;">
            <label>
                	所在地 : 
            </label>
            <select name="homeprovince" id="none_cur" class="select-short cur-plc">
                <option value="">省份</option>
            </select>
            <select id="none_cur_city" class="select-short" >
                <option value="">城市</option>
            </select>
            <input type="hidden" name="homecitycode" value="0" id="none_cur_city_code" /><input type="hidden" name="homecityname" value="" id="none_cur_city_name" />
        </p>
        <div id="flashDiv">
        </div>
        <div style="display:none">
            <div id="cameraDialog">
                <div id="cameraError" class="errors_div ajax_msgerror" style="display: none;">
                </div>
                <div class="large" id="ttt">
                </div>
            </div>
        </div>
        
        <script type="text/javascript">
		function getFlashType(){
			return 1;
		}
        
        function insertAllSWF(){
			//插入上传头像的flash
			var flashvars = {};
			//flashvars.tsc = "9ade147a1c4da5c789b852fc8a0895e7";
			flashvars.societyguester = "<%=request.getAttribute("f_ck_societyguester") %>";
			flashvars.hostid = "<%=request.getAttribute("f_identity") %>";
			flashvars.profileUrl = "javascript:afterImageSave()";
			
			flashvars.uploadUrl = "http://head2.upload.renren.com/flash/CameraUpload.f";
			var params = {};
			params.menu = "false";
			params.quality = "autohigh";
			//params.wmode = "opaque";
			params.allowfullscreen = "true";
			params.allowscriptaccess = "always";
			params.allownetworking = "all";
			var attributes = {};
			attributes.id = "cameraUploadContent";
			swfobject.embedSWF("http://head2.upload.renren.com/pages/profile/newCameraEditer.swf", "ttt", "440", "380", "9.0.0", false, flashvars, params, attributes);
			
			//插入人人餐厅注册引导flash
			function insertSWF(){
				var flashvars = {};
				flashvars.societyguester = "<%=request.getAttribute("f_ck_societyguester") %>";
				//flashvars.societyguester = "test";
				flashvars.hostid = "<%=request.getAttribute("f_identity") %>";
				flashvars.t = "<%=request.getAttribute("f_ck_ticket") %>";
				flashvars.domainName = document.domain;
				flashvars.ver = "4";
				var params = {
					allowScriptAccess: "always",
					allowNetWorking: "all"
				};
				
				if( !isFlashBroken() ){// 如果不是flash broken, 就让flash transparent,isFlashBroken这个函数再n.flash.jsproxy.js这个文件里面
					XN.debug.log( 'not Gecko...set transparent' );
					params.wmode = "transparent";
				} 
				
				
				var attributes = {
					id: "scoreFlash",
					name: "scoreFlash"
				};
				swfobject.embedSWF("${applicationScope.urlStatic}/swf/canteen/RegisterGuideLoader.swf", "flashDiv", "844", "584", "9.0.0", "expressInstall.swf", flashvars, params, attributes);
			}
			insertSWF();
		}
		insertAllSWF();
		
        </script>
		<input type="hidden" id="f_gender" value="<%=request.getAttribute("f_gender") %>" />
		<input type="hidden" id="f_birth" value="<%=request.getAttribute("f_birth") %>" />
		<input type="hidden" id="f_name" value="<%=request.getAttribute("f_name") %>" />
		<input type="hidden" id="f_identity" value="<%=request.getAttribute("f_identity") %>" />
		<input id="f_ck_societyguester" type="hidden" value="<%=request.getAttribute("f_ck_societyguester") %>"/>
		<input id="f_ck_host" type="hidden" value="<%=request.getAttribute("f_ck_host") %>"/>
		<input id="f_ck_ticket" type="hidden" value="<%=request.getAttribute("f_ck_ticket") %>"/>
	</div>
</div>
<input type="hidden" id="f_ck_societyguester" value="<%=request.getAttribute("f_ck_societyguester") %>" />
<input type="hidden" id="f_ck_host" value="<%=request.getAttribute("f_ck_host") %>" />


</body>
</html>