<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html"%>
<%@ taglib uri="http://www.renren.com/tags-core" prefix="xiaonei"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<%@ include file="/inc/head-guide-flow.inc" %>
	<link href="http://s.xnimg.cn/csspro/old/base.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
	<link href="http://s.xnimg.cn/csspro/apps/guide_flash.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
	
   	<script src="http://s.xnimg.cn/jspro/xn.ui.schoolSelectors.js?ver=$revxxx$" type="text/javascript"></script>
    <script src="http://s.xnimg.cn/jspro/xn.ui.currentPlace.js?ver=$revxxx$" type="text/javascript"></script>
    <script src="http://s.xnimg.cn/jspro/guide/swfobject.js?ver=$revxxx$" type="text/javascript"></script>
    <script src="http://s.xnimg.cn/jspro/xn.ui.pageTools.js?ver=$revxxx$" type="text/javascript"></script>
	<script src="http://s.xnimg.cn/jspro/guide/xn.flash.jsproxy.js?ver=$revxxx$" type="text/javascript"></script>
	
	<style type="text/css">
            .module-schoollist {
                margin-bottom: 0;
            }
            td.pop_content h2 {
               	background:#AE8445;
				border:1px solid #555555;
            }
            
            td.pop_content .dialog_content {
                overflow: hidden;
                background: #FFF5D0 none repeat scroll 0 0;
                border-color: #555555;
                border-style: solid; /*border-width:0 1px 1px;*/
                overflow: hidden;
                padding: 10px;
            }
            
            /*
             td.pop_content .dialog_buttons {  _width:606px; border:1px solid red; background:#E8CF96 none repeat scroll 0 0; padding:10px; text-align:right;}
             
             */
            td.pop_content .dialog_buttons {
                background: #E8CF96;
                padding: 10px;
            }
            
            td.pop_content .dialog_buttons input {
                background-color: #999 !important;
                background-image: none !important;
                border-color: #FFFFFF #124680 #124680 #FFFFFF !important;
                border-style: solid;
                border-width: 1px;
                color: #FFFFFF;
            }
            td.pop_content .dialog_buttons input.farm{
            	background-color: #AE8445 !important;
            }
            
            td.pop_content .dialog_body {
                border-bottom: none;
                font-size: 12px;
                padding-bottom: 0;
                background: #E8CF96;
                overflow: hidden;
            }
            
            td.pop_content .dialog_body a, td.pop_content .dialog_body a:visited {
                color: #AE8445;
            } #popup-country {
                background: white none repeat scroll 0 0;
                margin-bottom: 5px;
            } #popup-country li {
                background: none;
            } #popup-province li.active a, #popup-province-hs li.active a {
                background: #AE8445 none repeat scroll 0 0;
                color: #FFFFFF;
            } #popup-unis li a:hover, #popup-unis-hs li a:hover {
                background: #AE8445 none repeat scroll 0 0;
                color: #FFFFFF;
                text-decoration: none;
            } #popup-unis, #popup-unis-hs {
                margin: 5px 0 0;
            }
        </style>
	<!-- view -->
	<title>${domainIdentified["siteName"]}网</title>
</head>

<body>
<div id="container">
	<div id="header">
		<h1 id ="logo"><a href="http://www.renren.com">人人网</a></h1>
		<ul id="gift_menu" class="clearfix">
			
			<li id="field"><a href="#">6块地</a></li>
			<li id="hand"><a href="#">神偷手</a></li>
			<li id="bottle"><a href="#">喷水壶</a></li>
			<li id="final"><a href="#">终极神秘大礼包</a></li>
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
        <%
        Object hostid_obj = request.getAttribute("f_ck_host");
        Object t_obj = request.getAttribute("f_ck_ticket");
        int hostid = 0;
        String tstr = "";
        String newt="";
        if(hostid_obj!=null && t_obj!=null){
        	hostid = Integer.parseInt((String)hostid_obj);
        	tstr = (String)t_obj;
        	newt = com.xiaonei.platform.core.opt.base.PassportManager.getInstance().encryptTicket(hostid, tstr);//不需要调用服务
        }
        
        %>
        <script type="text/javascript">
        function insertAllSWF(){
			//插入上传头像的flash
			var flashvars = {};
			flashvars.societyguester = "<%=newt %>";
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
			
			//插入开心农场注册引导flash
			function insertSWF(){
				var flashvars = {};
				flashvars.societyguester = "<%=newt %>";
				//flashvars.societyguester = "test";
				flashvars.hostid = "<%=request.getAttribute("f_identity") %>";
				flashvars.t = "<%=newt %>";
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
				swfobject.embedSWF("${applicationScope.urlStatic}/swf/RegisterGuideLoader.swf", "flashDiv", "844", "627", "9.0.0", "expressInstall.swf", flashvars, params, attributes);
			}
			insertSWF();
		}
		insertAllSWF();
		
        </script>
		<input type="hidden" id="f_gender" value="<%=request.getAttribute("f_gender") %>" />
		<input type="hidden" id="f_birth" value="<%=request.getAttribute("f_birth") %>" />
		<input type="hidden" id="f_name" value="<%=request.getAttribute("f_name") %>" />
		<input type="hidden" id="f_identity" value="<%=request.getAttribute("f_identity") %>" />
		<input id="f_ck_societyguester" type="hidden" value="<%=newt %>"/>
		<input id="f_ck_host" type="hidden" value="<%=request.getAttribute("f_ck_host") %>"/>
		<input id="f_ck_ticket" type="hidden" value="<%=newt %>"/>
	</div>
</div>
<input type="hidden" id="f_ck_societyguester" value="<%=newt %>" />
<input type="hidden" id="f_ck_host" value="<%=request.getAttribute("f_ck_host") %>" />


</body>
</html>