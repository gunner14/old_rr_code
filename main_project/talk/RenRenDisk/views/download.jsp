<!DOCTYPE html>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn"%>
<%@ page isELIgnored="false" %> 
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>文件下载</title>
<script type="text/javascript">
XN = {get_check:'${requestScope.postFormSession}',get_check_x:'${requestScope._rtk}',env:{domain:'renren.com',shortSiteName:'人人',siteName:'人人网'}};
</script>
<script src="http://s.xnimg.cn/a47756/n/core/base-all2.js" type="text/javascript"></script>
<script type="text/javascript">
XN.getFileVersion([
'http://s.xnimg.cn/a47758/jspro/xn.app.share.js'
]);
</script>
<link href="http://s.xnimg.cn/a31310/n/core/global-all-min.css"
	rel="stylesheet" type="text/css" />
<link href="http://s.xnimg.cn/actimg/12skydrive/css/style.css"
	rel="stylesheet" type="text/css" />
<script type="text/javascript" src="/js/jquery.js"></script>
<script>
jQuery(document).ready(function(){	
	var down = 0;
	var url = window.location.href;
	var id = url.substr(url.lastIndexOf("/") + 1);
	$.post("/get/" + id, null ,function(resp){
		if(resp.charAt(0) == "h"){
			window.location.href = resp;
		}else if(resp.charAt(0) == "{"){
			var data = eval("[" + resp + "]")[0];
			var obj = $(".down_file_info").children();
			obj.eq(0).text(getName(data.path));
			obj.eq(1).text("文件大小：" + data.size);
			obj.eq(2).text("下载次数：" + data.downloads);
			obj.eq(3).text("发布时间：" + data.time);
			$(".download_btn_click").attr("path",data.path);
			down = parseInt(data.downloads);
		}else{
			alert(resp);
		}
	},"text");
	jQuery(".dialog_checkbox").click(function(){
		if(jQuery(this).hasClass("dialog_checkbox_checked")){
			jQuery(this).removeClass("dialog_checkbox_checked");
		}else{
			jQuery(this).addClass("dialog_checkbox_checked");
		}
	});
	
	jQuery(".download_btn_click").click(function(){
		jQuery.post("/down",{path : $(this).attr("path")},function(resp){
			if(resp.charAt(0) == "{"){
		    	 data = eval("[" + resp + "]");
		    	 window.location.href = data[0].list;
		    	 var obj = $(".down_file_info").children(":eq(2)");
		    	 obj.text("下载次数：" + (down + 1));
		    	 down += 1;
		     }else{
		    	 alert(resp);
		     }
		},"text");
	})
})
function getName(path){
	return path.substr(path.lastIndexOf("/") + 1);
}
</script>
</head>
<body>
<div class="down_container">
	<div class="renren_header">
		<a class="renren_logo" href=""></a>
	</div>
	<div class="down_main">
		<div class="login">
			<!-- span><label for="username">ç¨æ·å</label><input id="username" type="text"/></span>
			<span><label for="password">å¯ç </label><input id="password" type="text"/></span>
			<a href="#" class="register_btn">æ³¨å</a -->
		</div>
		<div class="down_main_sub">
			<div class="down_left">
			<div class="down_file_info">
					<h2 class="down_file_name">文件名</h2>
					<p>文件大小：</p>
					<p>下载次数：</p>
					<p>发布时间：</p>
				</div>
				<div class="download_btns clearfix">
					<a class="download_btn download_btn_click"></a>
					<!-- a href="#" class="download_btn download_btn_save"></a-->
				</div>
				<p class="publish_to clearfix">
					<a class="dialog_icons dialog_checkbox"><a>同时发布一条新鲜事到人人网</a></a>
				</p>
				<p class="warning">严禁上传包括反动、暴力、色情、等违法内容，或侵犯版权的文件。<a href="#">举报</a></p>
			</div>
			<div class="down_right"></div>
		</div>
		
	</div>
</div>
</body>
</html>
