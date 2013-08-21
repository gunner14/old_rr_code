<%@ page contentType="text/html;charset=UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<%@page import="com.xiaonei.reg.guide.root.interfaces.IGuideUserLeadType"%><html xmlns="http://www.w3.org/1999/xhtml">

<head><!-- pvz -->
<%@ include file="/inc/head-guide-flow.inc" %>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/swfobject.js?ver=$revxxx$"></script>
<link href="${applicationScope.urlSmallStatic}/csspro/apps/guide-register-pvz.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all">
<script type="text/javascript">
var errorDiv = 'uploadError';
    function guide_stats ( tag ) {
         var t = new Image();
         var rand = Math.random(); 
         t.src = 'http://guide.' + XN.env.domain + '/axat?p=p41&a=' + tag + '&t=' + rand ;
    }

    function uploadHeadShowError(msg) {
        var e = $(errorDiv);
        e.innerHTML = msg;
        e.show();
        setTimeout('$(errorDiv).hide();', 1000 * 8);
    }

    function toUseUpload() {
        $('cameraUploadContent').hide();
        var e = $('cameraError');
        e.innerHTML = '没有找到可用的摄像头！';
        e.show();
    }

    function uploadHeadSwfChkFlash() {
        var version = swfobject.getFlashPlayerVersion();
        if (document.getElementById && version && version.major >= 9) {
            return true;
        } else {
            return false;
        }
    }

    function saveImage(){
        guide_stats('rggp3v3a3');
        var rq = new XN.NET.xmlhttp({
            url: 'http://guide.${applicationScope.domain}/xtrs',
            method: 'get',
            onSuccess: function(r){
                location.href = 'portraits';//'${param.rurl}';
            },
            onError: function(r){
                location.href = 'portraits';//'${param.rurl}';
            }
        });
    }

    function  userpic_onUploadSuccess(code, msg, headpicUrl){
        if (code == 1) {
                $('userImage').src = headpicUrl;
                $('saveHead').show();
                $('jumpHead').hide();
        } else {
                uploadHeadShowError(msg);
        }
     }


    function addSelectEvent(){
        XN.event.addEvent($('upload_image'), 'change', function(e){
            var v = $('upload_image').value;
            if (!/\.(png|jpg|jpeg|gif|bmp)/i.test(v)) { 
                    uploadHeadShowError('请选择一张图片');
                    return;
            }
            $('userpic_form').submit();
        });                      
    }             

    function jump(){
        guide_stats('ga2');
        var rq = new XN.NET.xmlhttp({
            url: 'http://guide.${applicationScope.domain}/xtrs',
            method: 'get',
            onSuccess: function(r){
                location.href = 'portraits';//'${param.rurl}';
            },
            onError: function(r){
                location.href = 'portraits';//'${param.rurl}';
            }
        });
    }

    function report(){
        guide_stats('ga1');
    }

    function arrive(){
    }

    function showCarama(){
        guide_stats('ga4');

        var flashvars = {};
        flashvars.societyguester = "${cookie.t.value}";
        flashvars.hostid = "${user}";
        flashvars.t = "${cookie.t.value}";
        flashvars.profileUrl = "${encodeUrl}";
        flashvars.uploadUrl = "http://head2.upload.${applicationScope.domain}/registerguide/CameraUpload.f";
        var params = {};
        params.menu = "false";
        params.quality = "autohigh";
        params.allowfullscreen = "true";
        params.allowscriptaccess = "always";
        params.allownetworking = "all";
        var attributes = {};
        attributes.id = "cameraUploadContent";
        if(uploadHeadSwfChkFlash()){

            if( !window.dialog ) {
                window.dialog = XN.DO.alert({
                    title : '摄像头',
                    message : '<div id="cameraDialog">' +
                              '    <div id="cameraError" class="errors_div ajax_msgerror" style="display: none;"></div>' +
                              '    <div class="large" id="cameraUploadContent"></div>' +
                              ' </div>', 
                    yes : '关闭',
                    width : 485,
                    Y:50
                });
                swfobject.embedSWF("http://head2.upload.${applicationScope.domain}/pages/profile/newCameraEditer.swf", "cameraUploadContent", "440", "380", "9.0.0", false, flashvars, params, attributes);
            } else {
                window.dialog.show();
            }

        } else {
            var e = $('cameraErrorPage');
            e.innerHTML = '的浏览器未安装Flash 9以上版本插件，无法使用摄像头';
            e.show();
        }
    }

    XN.dom.ready(function(){
        addSelectEvent();
        arrive();
    });

    var hasAddFriendList = ${requestScope.lead_more_friend_confirmlist} || {};
    XN.dom.ready(function(){
        if($('jumpHead')){
            $('jumpHead').onclick = function(){
                guide_stats('ga2');
            }
        }
        var list = hasAddFriendList.candidate || [];
        for(var i=0,len = list.length; i < len ; i++){
            var obj = list[i];
            $('user-' + (i+1)).innerHTML = '<img src="'+obj.head+'" width="50" height="50" alt="'+obj.name+'"></img>'
        }

    },true)
    </script>

<title>${domainIdentified["siteName_fillinfo"]} - 上传真实头像</title>
</head>

<body id="home" class="guide-pvz">

<!--menu-bar begin-->
<%@ include file="/inc/header-guide-flow.inc" %>
<!--menu-bar end--><!--header-information-guide.inc end-->
<div id="content" class="content-third">
    <%
    int brown0 = IGuideUserLeadType.XFiveBrown_0;
    %>
    <c:set var="user_type_brown0" value="<%=brown0%>" />
    <c:choose>
        <c:when test="${requestScope.lead_user_type == user_type_brown0}">
					<h2>1.寻找朋友</h2>
					<h2>2.完善资料，找到熟人</h2>
					<h2>3.上传头像，进入首页</h2>
        </c:when>
        <c:otherwise>
					<h2>1.完善资料</h2>
					<h2>2.找到好友</h2>
					<h2>3.上传头像，进入首页</h2>
        </c:otherwise>
    </c:choose>  
    <div class="user-data-upload">
        <h2>使用真实头像，朋友能够认出你，还能享受星级用户特权！</h2>
        <div id="uploadError" class="upload-error" style="display:none;"></div>
        <!-- <div class="upload-error">请先上传头像</div> -->
        <div class="upload-photo">
            <div class="user-pic">
                <div id="user-1" class="user-1"></div>
                <div id="user-2" class="user-2"></div>
                <div id="user-3" class="user-3"></div>
                <div id="user-4" class="user-4"></div>
                <div id="user-5" class="user-5"></div>
                <div id="user-6" class="user-6"></div>
                <div id="user-7" class="user-7"></div>
                <div id="user-8" class="user-8"></div>
                <p><c:choose>
                    <c:when test="${not empty hasHead}">
                        <img id="userImage" src="${requestScope.userMainFullUrl}" alt="头像预览" style="width: auto;" />
                    </c:when>
                    <c:otherwise>
                        <img id="userImage" src="http://s.xnimg.cn/imgpro/guide/user-pic.jpg" width="200"/>
                    </c:otherwise>
                </c:choose></p>
            </div>
            <div class="upload-btn clearfix">
                <!--  <a href="#nogo" class="upload-bg"><input type="file" class="addfile" size="1" name="file"/></a> -->
                <form id="userpic_form" method="POST" target="userpic_quick_upload" enctype="multipart/form-data" action="http://head2.upload.${applicationScope.domain}/uploadse
rvice.fcgi?pagetype=addHeadProfile">
                    <a href="javascript:;" class="upload-bg">
                        <input type="file" class="addfile" size="1" name="file" id="upload_image" onclick="report();"/>
                    </a>
                    <input type="hidden" name="t" value="${cookie.t.value}"/>
                    <input type="hidden" name="hostid" value="${user}"/>
                    <input type="hidden" name="from" value="registerImage"/>
                </form>
                <p>或 <a href="javascript:void(0);" onclick="showCarama();">摄像头拍摄</a></p>
                <div style="display:none" id="cameraErrorPage"> </div>
            </div>
        </div>
        <div class="btn-box">
        <!-- <input type="button" value="保存头像-new" class="input-submit" />　<a href="#" class="skip">跳过-new</a> -->
            <c:choose>
                 <c:when test="${not empty hasHead}">
                     <input id="saveHead" type="button" value="" class="home-input-submit" onclick="saveImage();"/>
                     <!-- <a href="#nogo" id="jumpHead" onclick="jump();" style="display:none">跳过</a> -->
                     <a href="portraits" id="jumpHead" style="display:none">跳过</a>
                 </c:when>
                 <c:otherwise>
                     <input id="saveHead" type="button" value="" class="home-input-submit" onclick="saveImage();" style="display:none"/>　
                     <!-- <a href="#nogo" id="jumpHead" onclick="jump();" style="float:right;padding-right:12px;">跳过</a> -->
                     <a href="portraits" id="jumpHead" style="float:right;padding-right:12px;">跳过</a>
                 </c:otherwise>
            </c:choose>
        </div>
    </div><!--user-data end-->

</div><!--content end-->

<iframe name="userpic_quick_upload" src="http://www.${applicationScope.domain}/ajaxProxy.html" style="display:none;"></iframe>
<%@ include file="/inc/footer-guide-flow.inc" %>
<script src="${applicationScope.urlSmallStatic}/utm/urchin.js" type="text/javascript">
</script> 
<script type="text/javascript">
	guide_stats('ga0');
   _userv=0; 
   urchinTracker(); 
</script> 
</body>
<%
try{
    com.xiaonei.platform.core.model.User user = com.xiaonei.platform.core.opt.base.BaseThreadUtil.currentHost(request);
    if(user != null && user.getStage() == 90 && user.getId()%2 == 0){
           com.xiaonei.wService.userGuide.adapter.UserGuideAdapter.getInstance().push(user.getId(),com.xiaonei.wService.userGuide.adapter.UserGuideAdapter.NEW_REG_ARRIVAL,null);
    }
}
catch(Exception e){
}
%>

</html>
