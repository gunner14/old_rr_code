<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>人人桌面网盘</title>
<link href="http://s.xnimg.cn/a31310/n/core/global-all-min.css"
	rel="stylesheet" type="text/css" />
<link href="http://s.xnimg.cn/actimg/12skydrive/css/style.css"
	rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.js"></script>
<script type="text/javascript" src="swf/swfupload.js"></script>
<script type="text/javascript" src="swf/swfupload.queue.js"></script>
<script type="text/javascript" src="swf/swfupload.speed.js"></script>
<script type="text/javascript" src="swf/handlers.js"></script>
<script type="text/javascript" src="copy/ZeroClipboard.js"></script>
<script type="text/javascript" src="js/main.js"></script>
<script type="text/javascript" src="js/dialog.js"></script>
<script type="text/javascript" src="swf/upload.js"></script>
<script type="text/javascript">
XN = {get_check:'${requestScope.postFormSession}',get_check_x:'${requestScope._rtk}',env:{domain:'renren.com',shortSiteName:'人人',siteName:'人人网'}};
data_share = "{stype:'create_share_feed', owner:${uid}, type:'wpan', id:";
tempId ="${tempId}";
share_post = {uid : ${uid}};
</script>
<script src="http://s.xnimg.cn/a47756/n/core/base-all2.js" type="text/javascript"></script>
<script src="http://s.xnimg.cn/a47758/jspro/xn.app.share.js" type="text/javascript"></script>
<script>
var post_obj;
XN.app.share.addEvent('postSuccess',function(){
    jQuery.post("/shareSuccess",{json : JSON.stringify(share_post)},function(resp){
        if (resp == "success") {
            error_msg("文件已分享");
        } else if (resp == "fail") {
            error_msg("文件分享失败");
		} else {
			var data = eval("[" + resp+ "]")[0];
            var sh = data.share;
			SHARE.push(sh);
            tempId = data.tempId;
			post_obj.attr("id", sh.id);
			post_obj.find(".shared_sign").text("已分享");
		}
    });
});
</script>
</head>
<style type="text/css">
html {
    /*overflow: auto;*/
    _overflow-x: hidden;
    _overflow-y: hidden;
}

body {
	overflow-x: hidden;
	overflow-y: hidden;
	overflow: hidden;
}

.right_bottom {
	position: absolute;
	bottom: 0px;
	right: 0px;
}

.dialog {
	width: 380px;
	position: absolute;
	display: none;
	z-index: 98;
}

.separate {
	height: 100%;
	width: 100%;
	filter: alpha(Opacity = 0);
	position: absolute;
	-moz-opacity: 0.5;
	opacity: 0;
	z-index: 97;
	background-color: #ffffff;
	left: 0px;
	top: 0px;
	display: none;
}

.error{
    z-index: 90;
    text-align: center;
    position:absolute;
    color: red;
    padding-top: 8px;
    margin-top: -8px;
    back-ground: none repeat scroll 0 0 #F8FBFE;
    height: 34px
}

.sure {
	background:
		url(http://s.xnimg.cn/actimg/12skydrive/img/dialog_icons.png)
		no-repeat;
	background-position: 0px -145px;
	width: 16px;
	height: 16px;
	margin-left: 10px;
	border: 0px;
}

.cancel {
	background:
		url(http://s.xnimg.cn/actimg/12skydrive/img/dialog_icons.png)
		no-repeat;
	background-position: 0px -161px;
	width: 16px;
	height: 16px;
	margin-left: 10px;
	border: 0px;
}
</style>
<body>
	<div class="container">
		<div class="header clearfix">
			<div class="logo"></div>
			<div class="header_nav clearfix">
				<a href="#" class="header_nav_btn header_nav_btn_act"><span
					class="header_nav_btn_bg btn_myfile"></span></a> <a href="#"
					class="header_nav_btn"><span
					class="header_nav_btn_bg btn_myshare"></span></a> <a href="#"
					class="header_nav_btn"><span
					class="header_nav_btn_bg btn_recycle"></span></a>
			</div>
			<div class="drive_room clearfix">
				<div class="drive_room_bg">
					<!-- div class="drive_room_progress">
						<span class="drive_room_info">50.00GB/50.00GB</span>
					</div-->
					<div class="drive_room_progress"></div>
					<span class="drive_room_info">0GB/0GB</span>
				</div>
				<p class="header_more">
					<a href="#">获得更多空间</a>
				</p>
			</div>
		</div>
		<div class="main">
			<div class="direct_bar clearfix">
				<h2 class="direct_info">我的人人网盘</h2>
				<div class="direct_func_bar clearfix">
					<!--  a class="direct_func_btn"><span id="spanButtonPlaceholder"></span></a-->
					<a class="direct_func_btn direct_upload_btn2"><span
						id="spanButtonPlaceholder"></span></a> <a class="direct_func_btn"><span
						class="direct_new_btn" style="color:#336699">新建</span></a>
					<div class="search_bar clearfix">
						<input type="text" class="search_input" value="" /> <a href="#"
							class="search_btn"></a>
					</div>
				</div>
			</div>
			<div class="main_upload" style="height: auto;">
				<div class="upload_title_con clearfix">
					<span class="main_file_name"><a href="#"
						class="dialog_icons dialog_checkbox"></a>文件名</span> <a href="#"
						class="main_icons upto_btn">返回上一级</a>
						
					<div class="sub_menu_box sub_menu_box_1">
					<div class="sub_menu">
					    <a href="#nogo" class=" direct_func_btn"><span data-share="{stype:'create_share_feed', id:'XXXXX', owner:${uid}, type:'wpan'}" class="main_icons pubshare_btn share_new">公开分享</span></a>
						<!-- a href="#" class="direct_func_btn"><span class="main_icons pubshare_btn">公开分享</span></a-->
					</div>
				</div>
				<div class="sub_menu_box sub_menu_box_2">
					<div class="sub_menu">
						<a href="#" class="direct_func_btn direct_func_btn_act"><span class="main_icons secshare_btn">私密分享</span><em class="main_icons btn_sub_menu"></em></a>
						<ul class="sub_menu_list sub_menu_list1">
							<li><a href="#" class="direct_func_btn"><span class="main_icons copylink_btn">复制链接</span></a></li>
							<li><a href="#" class="direct_func_btn"><span class="main_icons mailfr_btn">发邮件给好友</span></a></li>
						</ul>
					</div>
				</div>
				<div class="sub_menu_box sub_menu_box_3">
					<div class="sub_menu">
						<a href="#" class="direct_func_btn direct_func_btn_act"><span class="getmore_btn">更多</span><em class="main_icons btn_sub_menu"></em></a>
						<ul class="sub_menu_list sub_menu_list2">
							<li><a href="#" class="sub_menu_list_btn"><span>下载</span></a></li>
							<li><a href="#" class="sub_menu_list_btn"><span>重命名</span></a></li>
							<li><a href="#" class="sub_menu_list_btn"><span>删除</span></a></li>
							<li><a href="#" class="sub_menu_list_btn"><span>复制到</span></a></li>
							<li><a href="#" class="sub_menu_list_btn"><span>移动到</span></a></li>
						</ul>
					</div>
				</div>

					<!-- 我的分享 -->
					<a class="direct_func_btn direct_func_btn_adjust1"><span
						class="main_icons cancleshare_btn">取消分享</span></a>
					<a class="direct_func_btn"><span
						class="main_icons copylink_btn">复制链接</span></a>

					<!-- 回收站 -->
					<a href="#" class="direct_func_btn direct_func_btn_adjust2"><span
						class="main_icons redo_btn">还原</span></a> <a href="#"
						class="direct_func_btn"><span class="main_icons delete_btn">彻底删除</span></a>
					<a href="#" class="direct_func_btn"><span
						class="main_icons cleanr_btn">清空回收站</span></a>

					<div class="main_flie_item">
						<a class="main_file_size" href="#">大小</a> 
						<a class="main_file_date" href="#">修改日期</a>
					</div>
				</div>
				<ul class="upload_list" style="height: 400px;"></ul>
				<div class="main_error"></div>
			</div>
		</div>
	</div>
	<div class="separate"></div>
	<div class="dialog">
		<div class="skydrive_dialog_box dialog_box_filelist">
			<div class="dialog_box_bg clearfix">
				<h2 class="dialog_box_title">
					<span class="dialog_icons icon_renren_logo"></span><a
						class="dialog_icons close_box" href="#"></a>
				</h2>
				<div class="dialog_main">
					<ul class="file_list">
						<li><span class="file_list_root" name="/apps/人人网盘/"><span
								class="dialog_icons icon_file file_list_name">我的文件</span></span></li>
					</ul>
					<div class="dialog_btns clearfix">
						<span class="new_direct dialog_btn_bg"><a
							class=" icon_new_direct" href="#"><span class="dialog_icons">新建目录</span></a></span>
						<a class="dialog_btn_bg dialog_btn_short" href="#">取消</a><a
							class="dialog_btn_bg dialog_btn_short" href="#">确定</a>
					</div>
				</div>
			</div>
		</div>
		<div class="skydrive_dialog_box dialog_box_edit">
			<div class="dialog_box_bg clearfix">
				<h2 class="dialog_box_title">
					<span class="dialog_icons icon_renren_logo">创建链接</span><a
						class="dialog_icons close_box" href="#"></a>
				</h2>
				<div class="dialog_main">
					<div class="dialog_main_sub">
						<p class="file_list_remind">您和您的朋友可以通过以下链接访问文件</p>
						<div class="link_borderline">
							<input class="link_descrip" type="text"
								placeholder="http://renren.com/u/1252791765" />
						</div>
						<span class="link_remind">提示：被分享的文件不能移动或重命名。</span>
					</div>
					<div class="dialog_btns clearfix"><a
							class="dialog_btn_bg dialog_btn_short" id="copy_sure">复制</a>
					</div>
				</div>
			</div>
		</div>
	<div class="skydrive_dialog_box dialog_box_mail">
	<div class="dialog_box_bg clearfix">
		<h2 class="dialog_box_title"><span class="dialog_icons icon_renren_logo">发邮件给好友</span><a class="dialog_icons close_box" href="#"></a></h2>
		<div class="dialog_main">
			<div class="dialog_main_sub clearfix">
				<p>您可以通过邮件分享给好友</p>
				<p class="mail_item"><span>收件人</span><input type="text" placeholder="最多可输入10个邮箱，以分号隔开" /> </p>
				<p class="mail_item link_borderline"><span>正文</span><textarea placeholder="请添加描述…"></textarea></p>
				<span class="link_remind">提示：被分享的文件不能移动或重命名。</span>
			</div>
			<div class="dialog_btns clearfix">
				<a class="dialog_btn_bg dialog_btn_short" href="#">发送</a>
			</div>
		</div>
	</div>
</div>
	</div>
	<div class="right_bottom">
		<div class="skydrive_dialog_box dialog_box_upload">
			<div class="dialog_box_bg clearfix">
				<h2 class="dialog_box_title">
					<span class="dialog_icons icon_renren_logo"><label>正在上传：0/0</label><span
						class="title_upload_speed">速度：0KB/S</span></span><a
						class="dialog_icons min_box"></a><a class="dialog_icons close_box"></a>
				</h2>
				<div class="dialog_main">
					<p class="upload_table_line">
						<span class="upload_title">标题</span> <span class="upload_size">大小</span>
						<span class="upload_root">上传目录</span> <span class="upload_state">状态</span>
						<span class="upload_operate">操作</span>
					</p>
					<ul class="dialog_upload_table"></ul>
					<div class="dialog_btns clearfix">
				<a class="dialog_btn_bg dialog_btn_short" href="#">完成</a>
			</div>
				</div>
			</div>
		</div>
		<div class="skydrive_dialog_box dialog_box_min">
			<div class="dialog_box_bg clearfix">
				<h2 class="dialog_box_title_min">
					<span class="dialog_icons icon_renren_logo"><label>正在上传：0/0</label><span
						class="title_upload_speed">速度：0KB/S</span></span><a
						class="dialog_icons max_box" href="#"></a><a
						class="dialog_icons close_box" href="#"></a>
				</h2>
			</div>
		</div>
	</div>
</body>
</html>
