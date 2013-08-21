<%@ page contentType="text/html;charset=UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<!-- _po_dark_ -->
<head>
    <%@ include file="/inc/head-guide-flow.inc" %>
	<link href="${applicationScope.urlSmallStatic}/csspro/apps/reg-guide.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
	<link href="${applicationScope.urlSmallStatic}/csspro/apps/guide-new.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all">
    <script type="text/javascript" src="${applicationScope.urlSmallStatic}/js/app/carpark/swfobject.js?ver=$revxxx$"></script>

<script>
var errorDiv = 'uploadError';

function uploadHeadShowError(msg) {
	var e = $(errorDiv);
	e.innerHTML = msg;
	e.show();
	setTimeout('$(errorDiv).hide();', 1000 * 8);
}

function toUseUpload() {
	var e = $('cameraError');
    e.innerHTML = '没有找到可用的摄像头！';
    e.show();
    $('cameraUploadContent').hide();
}

function uploadHeadSwfChkFlash() {
	var version = swfobject.getFlashPlayerVersion();
	if (document.getElementById && version && version.major >= 9) {
		// okay
		return true;
	} else {
		return false;
	}
}

function saveImage(){
	var rq = new XN.NET.xmlhttp({
		url: 'http://guide.${applicationScope.domain}/xtrs?act_id=20203010',
		method: 'get',
		onSuccess: function(r){
			location.href = 'portraits';//'${param.rurl}';
		},
		onError: function(r){
			location.href = 'portraits';//'${param.rurl}';
		}
	})
}

function  userpic_onUploadSuccess(code, msg, headpicUrl){
	if (code == 1) {
	        $('userImage').src = headpicUrl;
	        $('saveHead').show();
	        $('jumpHead').hide();
	}
	else {
	        uploadHeadShowError(msg);
	}
 };


function addSelectEvent(){
	XN.event.addEvent($('upload_image'), 'change', function(e){
	        var v = $('upload_image').value;
	        if (!/\.(png|jpg|jpeg|gif|bmp)/i.test(v)) { 
	                uploadHeadShowError('请选择一张图片');
	                return;
	        };
	        $('userpic_form').submit();
	})                      
};             

function jump(){
	var rq = new XN.NET.xmlhttp({
		url: 'http://guide.${applicationScope.domain}/xtrs?act_id=20203011',
		method: 'get',
		onSuccess: function(r){
			location.href = 'portraits';//'${param.rurl}';
		},
		onError: function(r){
			location.href = 'portraits';//'${param.rurl}';
		}
	})
}

function report(){
	var rq = new XN.NET.xmlhttp({
		url: 'http://guide.${applicationScope.domain}/xtrs?act_id=20203002',
		method: 'get'
	})
}

function arrive(){
	var rq = new XN.NET.xmlhttp({
		url: 'http://guide.${applicationScope.domain}/xtrs?act_id=20203001',
		method: 'get'
	})
};

XN.dom.ready(function(){
addSelectEvent();
arrive();
}
);
</script>

<title>${domainIdentified["siteName_fillinfo"]} - 上传真实头像</title>
</head>

<body id="home" class="guide">
<%@ include file="/inc/header-guide-flow.inc" %>
<div id="content">
	<div class="user-data">
		<div class="title">
			<h1>有真实头像，朋友才能认出你，还能提升人气哦！</h1>
		</div>
		<div id="uploadError" class="upload-error" style="display: none;"></div>
		<div class="upload-photo clearfix">
			<div class="user-pic">
				<c:choose>
				    <c:when test="${not empty hasHead}">
						<img id="userImage" src="${requestScope.userMainFullUrl}" alt="头像预览" style="width: auto;" />
					</c:when>
					<c:otherwise>
						<img id="userImage" src="http://s.xnimg.cn/imgpro/guide/user-pic.jpg" width="200"/>
					</c:otherwise>
				</c:choose>
			</div>
			<div class="upload-btn">
				<form id="userpic_form" method="POST" target="userpic_quick_upload" enctype="multipart/form-data" action="http://head2.upload.${applicationScope.domain}/uploadse
rvice.fcgi?pagetype=addHeadProfile">
					<a href="javascript:;" class="upload-bg">
						<input type="file" class="addfile" size="1" name="file" id="upload_image" onclick="report();"/>
					</a>
					<input type="hidden" name="t" value="${cookie.t.value}"/>
                    <input type="hidden" name="hostid" value="${user}"/>
					<input type="hidden" name="from" value="registerImage"/>
                </form>
				<span>或 <a href="javascript:void(0);" onclick="showCarama();">摄像头拍摄</a></span>
				<div style="display:none">
						<div id="cameraDialog">
                         	<div id="cameraError" class="errors_div ajax_msgerror" style="display: none;"></div>
                         	<div class="large" id="cameraUploadContent"></div>
                         </div>
					</div>
					<script type="text/javascript">
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
						swfobject.embedSWF("http://head2.upload.${applicationScope.domain}/pages/profile/newCameraEditer.swf", "cameraUploadContent", "440", "380", "9.0.0", false, flashvars, params, attributes);
	
						function showCarama(){
							if(uploadHeadSwfChkFlash()){
								window.dialog = XN.DO.alert(
								{
									title : '摄像头',
									message : $('cameraDialog'),
									yes : '关闭',
									width : 485,
									Y:50
								});
								dialog
							}else{
								var e = $('cameraError');
							    e.innerHTML = '的浏览器未安装Flash 9以上版本插件，无法使用摄像头';
							    e.show();
							}
						}
					</script>
			</div>
		</div>
		<div class="fill-data secondary">
			<div class="btn-box">
				<c:choose>
				    <c:when test="${not empty hasHead}">
						<input id="saveHead" type="button" value="保存头像" class="input-submit" onclick="saveImage();"/>
						<a href="#nogo" id="jumpHead" onclick="jump();" style="display:none">跳过</a>
					</c:when>
					<c:otherwise>
						<input id="saveHead" type="button" value="保存头像" class="input-submit" onclick="saveImage();" style="display:none"/>　
						<a href="#nogo" id="jumpHead" onclick="jump();" style="float:right;padding-right:12px;">跳过</a>
					</c:otherwise>
				</c:choose>
			</div>
		</div>
	</div>
</div>
<iframe name="userpic_quick_upload" src="http://www.${applicationScope.domain}/ajaxProxy.html" style="display:none;"></iframe>
<%@ include file="/inc/footer-guide-flow.inc" %>
<script src="${applicationScope.urlSmallStatic}/utm/urchin.js" type="text/javascript">
</script> 
<script type="text/javascript"> 
   _userv=0; 
   urchinTracker(); 
</script> 
</body>
</html>




