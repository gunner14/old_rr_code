var upload_dir="";
var upload_path='https://pcs.baidu.com/rest/2.0/pcs/file?method=upload&path=';

function fileDialogComplete(numFilesSelected, numFilesQueued) {
	try {
		if (numFilesSelected > 0) {
			if(jQuery(".right_bottom>.dialog_box_upload:hidden").length!=0){
			    jQuery(".right_bottom .max_box").trigger("click");
			}
			jQuery(".dialog_box_upload .dialog_btns").css("display","none");
			jQuery(".dialog_box_upload ul").removeClass("dialog_upload_table_short");
			this.startUpload();
		}
	} catch (ex)  {
        this.debug(ex);
	}
}

function dialogStart(){
	try {
		upload_dir=getCurrentPath();
	} catch (ex)  {
        this.debug(ex);
	}
}

function fileQueued(file) {
	try {
		var obj=jQuery('<li class="upload_table_line" id= "'+file.id+'" name="'+upload_dir+'">'
				+'<span class="upload_title">'+file.name+'</span>'
				+'<span class="upload_size">'+Size(file.size)+'</span>'
				+'<span class="upload_root">我的文件</span>'
				+'<span class="upload_state">pending</span>' 
				+'<span class="upload_operate"><em class="dialog_icons icon_wrong"></em></span>'
				+'</li>');
		jQuery(".dialog_upload_table").append(obj);
		obj.children(".upload_operate:eq(0)").click(function(){
			swfu.cancelUpload(file.id);
			obj.remove();
			var queue_num = swfu.getStats().files_queued;
			var all = jQuery(".dialog_upload_table>li").length;
			jQuery(".right_bottom .icon_renren_logo>label").text("正在上传：" + (all - queue_num + 1) + "/" + all);
		});
	} catch (ex) {
		this.debug(ex);
	}

}

function fileQueueError(file, errorCode, message) {
	try {
	} catch (ex) {
        this.debug(ex);
    }
}

function uploadStart(file) {
	try {
		swfu.setUploadURL(upload_path+encodeURI(jQuery("#"+file.id).attr("name")+file.name,"utf-8")+ "&access_token=" + T);
		var queue_num = swfu.getStats().files_queued;
		var all = jQuery(".dialog_upload_table>li").length;
		jQuery(".right_bottom .icon_renren_logo>label").text("正在上传：" + (all - queue_num + 1) + "/" + all);
	}
	catch (ex) {}	
	return true;
}

var flag=0;
function uploadProgress(file, bytesLoaded, bytesTotal) {
	try {
		jQuery("#"+file.id).children(".upload_state:eq(0)").html(SWFUpload.speed.formatPercent(file.percentUploaded));
		if(flag==0)  jQuery(".title_upload_speed").html(SWFUpload.speed.formatBPS(file.currentSpeed));
		if(++flag==5)  flag=0;		
	} catch (ex) {
		this.debug(ex);
	}
}

function uploadSuccess(file, serverData) {
	try {
		var data = eval("[" + serverData + "]");
		if(data[0].error_code){
			jQuery("#"+file.id).children(".upload_state:eq(0)").html('<font color = red>文件上传失败</font>');
			jQuery("#"+file.id).children(".upload_operate:eq(0)").html('');
			return;
		}
		
		var obj = data[0];
	    USED += obj.size;
	    var temp = parseInt(USED*149/ALL+2);
	    jQuery(".drive_room_progress:eq(0)").css({"width": temp + "px"});
	    jQuery(".drive_room_info:eq(0)").text(Size(USED)+"/"+Size(ALL));

		jQuery("#"+file.id).children(".upload_state:eq(0)").html('<em class="dialog_icons icon_right"></em>');
		jQuery("#"+file.id).children(".upload_operate:eq(0)").html('');
		if(jQuery("#"+file.id).attr("name")== getCurrentPath()){
			var name = getName(obj.path);
			var html = jQuery("<li class='clear fix' name='"+ obj.path +"'> <span class='upload_file_name clear fix'>"
				        +"<a class='dialog_icons dialog_checkbox'></a>"
				        +"<a class='main_icons upload_icon_img " + getFileTypeCss(name) + "'>" + name + "</a>" 
				        +"<span class='shared_sign'></span></span>"
			            +"<span class='main_file_size'>" + Size(obj.size) + "</span>" 
			            +"<span class='main_file_date'>" + getFileTime(obj.mtime) + "</span></li>");
			jQuery(".upload_list").append(html);
			//rankByName();
			setCheckBox(html);
		}
	} catch (ex) {
		this.debug(ex);
	}
}

function uploadError(file, errorCode, message) {
	try {
		jQuery("#"+file.id).children(".upload_state:eq(0)").html('<font color = red>文件上传失败</font>');
		jQuery("#"+file.id).children(".upload_operate:eq(0)").html('');
	} catch (ex) {
        this.debug(ex);
    }
}

function uploadComplete(file) {
}

function queueComplete(numFilesUploaded) {
	jQuery(".title_upload_speed").html("");
	jQuery(".right_bottom .icon_renren_logo>label").text("上传完成");
	jQuery(".dialog_box_upload ul").addClass("dialog_upload_table_short");
	jQuery(".dialog_box_upload .dialog_btns").show();
	jQuery(".right_bottom").css("margin-bottom", "0px");
}
