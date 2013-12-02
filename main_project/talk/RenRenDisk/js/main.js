var CURRENTPATH = "";
var HOMEURL = "list";
var MKDIRURL = "mkdir";
var MOVEURL = "move";
var DELETEURL ="dels";
var DOWNLOADURL ="download";
var SHARELINKURL="shareLink";
var CREATELINKURL="createLink";
var QUOTAURL="quota";
var SHARELISTURL="shareList";
var CANCELSHAREURL="cancelShare";
var COPYSHAREURL="copyShareList";
var SEARCHURL="search";
var SENDMAIL = "sendMail";
var SHIFTDOWN = false;
var BASEPATH = "/apps/人人网盘/";
var SHAREBASEURL = "http://disk.renren.com/share/";
var ALL;
var USED;
var RANKSIZE = "up";
var RANKTIME = "up";
var T;
var SHARE = null;
var COPYFLAG;
var share_data;

jQuery(document).ready(function() {  //页面加载函数	
	
	if (typeof String.prototype.trim !== 'function') {
		String.prototype.trim = function() {
			return this.replace(/^\s+|\s+$/g, '');
		};
	}	
	if(typeof Array.prototype.have !== 'function'){
		Array.prototype.have = function(e) {
			for (var i = 0; i < this.length; i++) {
				if (this[i] == e)
					return true;
			}
			return false;
		};
	}	
	if(typeof String.prototype.startWith !== 'function'){
		String.prototype.startWith = function(str){
			var reg = new RegExp("^" + str);
			return reg.test(this);
		};
	}
	if(typeof String.prototype.endWith !== 'function'){
		String.prototype.endWith = function(str) {
			var reg = new RegExp(str + "$");
			return reg.test(this);
		};
	}
	
	jQuery(".upload_title_con>a:eq(0)~:not(:last)").hide();
	jQuery(".main_error").hide();
	T = getCookie("a_t");
	importQuota();
	
	jQuery(".main_file_name>a").click(function() {  //全选按钮 事件
		if(jQuery(".upload_list:has(:text)").length != 0){
			jQuery(".upload_list :text").focus().select();
			return;
		}
		jQuery(".sub_menu>.sub_menu_list").css("display", "none");
		jQuery(this).toggleClass("dialog_checkbox_checked");
		var isChecked = jQuery(this).hasClass("dialog_checkbox_checked");
		jQuery(".upload_list>li").each(function() {
			if (isChecked)
				jQuery(this).find("a").eq(0).addClass("dialog_checkbox_checked");
			else
				jQuery(this).find("a").eq(0).removeClass("dialog_checkbox_checked");
		});
		var head = jQuery(".header_nav>.header_nav_btn_act").index();
		var num = jQuery(".upload_list>li:has(.dialog_checkbox_checked)").length;
		jQuery(".upload_title_con>:lt(10):gt(1)").css("display", "none");
		if(head == 0){
			if(num == 1){
				jQuery(".upload_title_con>.sub_menu_box").show();
			}else if(num > 1){
				jQuery(".upload_title_con>.sub_menu_box_3").show();
			}
		}else if(head == 1){
			jQuery(".upload_title_con>.direct_func_btn:lt(2)").show();
		}
	});
	
	//返回上一级  按钮的点击事件
	jQuery(".upto_btn").css("display", "none");
	jQuery(".upto_btn").click(function(){
		if(jQuery(".upload_list:has(:text)").length > 0){
			jQuery(".upload_list :text").focus().select();
			return;
		}
		freshList(getCurrentPath(-1));
	});
	
	jQuery(".header_nav>a:eq(2)").css("display", "none");
	
	jQuery(".btn_myshare").one("click", function(){
		myShare();
		jQuery(".btn_myfile").one("click", function(){
			myDocument();
		});
	});
	
	uploadInit();  //上传flash初始化
	
	jQuery(".direct_func_bar>a").bind({//新建按钮
		"click":function(){		
			var o = jQuery(this).children();
			if (o.text().trim()=="新建") {
				New();
			}
		},
		"mouseenter":function(){
			jQuery(this).css("cursor", "hand");
		}
	});	
	
	jQuery(".upload_title_con>.direct_func_btn").mouseenter(function(){
		jQuery(this).css("cursor" , "hand");
	});
	
	jQuery(".search_bar>:text").keypress(function(e){  //搜索栏输入事件
		if(e.keyCode==13){
			jQuery(".search_btn").trigger("click");
		}
	});	 
	jQuery(".search_btn").click(function(){              //搜索按钮点击事件
		if(jQuery(".upload_list:has(:text)").length != 0){
			jQuery(".upload_list :text").focus().select();
			return;
		}
		if(jQuery(".header_nav_btn:eq(0)").hasClass("header_nav_btn_act")){
			search();
		}else{
			searchShareList(jQuery(".search_input").val());
		}
	});
	
	jQuery(".sub_menu").bind({
		mouseenter:function(){
		    if(jQuery(this).children("a:eq(0)").text()=="更多"){
		        var liObj = jQuery(".upload_list>li:has(.dialog_checkbox_checked)");
		    	    if(liObj.length > 1){
		    	        jQuery(this).find("li:eq(0)").hide();
		    	        jQuery(this).find("li:eq(1)").hide();
		    	    }else{
		    		    if(liObj.has(".upload_icon_file").length == 1){
		    			    jQuery(this).find("li").eq(0).hide();
		    		    }
		    	    }
		    	    var len = liObj.length;
		    	    for(var i = 0; i < len; ++i){
		    		    var id = liObj.eq(i).attr("id");
		    		    if(id != null && id.length !=0){
		    			    jQuery(this).find("li:eq(4)").hide();
		    			    jQuery(this).find("li:eq(1)").hide();
		    		    }
		    	    }
		     }
		     jQuery(this).children("ul").show();
		},
	    mouseleave:function(){
	    	jQuery(this).find("li").show();
	    	jQuery(this).children("ul").hide();
	    }
	});
	  
	jQuery(".sub_menu li").bind({
	    "click" : function(){
	    	jQuery(".sub_menu>ul").hide();
		    switch(jQuery(this).text().trim()){
			    case "复制链接":copyLink();break;
			    case "发邮件给好友":sendMessage();break;
			    case "下载":download();break;
			    case "重命名":rename();break;
			    case "删除":del();break;
			    case "复制到":copyTo();break;
			    default:moveTo();
		    }
	    }
	});
	
	jQuery(".cancleshare_btn").click(function(){
		cancelShare();
	});
	  
    /**
     * 复制链接按钮事件
     */
	jQuery(".upload_title_con>.direct_func_btn:eq(1)").bind({
	    "click" : function(){
	    	var liObj = jQuery(".upload_list>li:has(.dialog_checkbox_checked)"); 
			var arr = new Array();
			var len = liObj.length;
			for(var i = 0; i < len; ++i){
			    arr.push(SHAREBASEURL + liObj.eq(i).attr("id"));
			}
			COPYFLAG = true;
	    	jQuery(".link_descrip").val(arr.join("  "));
			showDialog(jQuery(".dialog:eq(0)"),"创建链接");
	    }
	});
	  
	  /*绑定dialog事件*/
	bindFunction(jQuery(".dialog:eq(0)"));

	jQuery(".right_bottom .min_box").click(function(){
		jQuery(".right_bottom>.dialog_box_min").show();
		jQuery(".right_bottom>.dialog_box_upload").hide();
		jQuery(".right_bottom>.dialog_box_upload").css({"margin-bottom":"-300px"});
	});
	jQuery(".dialog_box_upload .dialog_btns").css("display","none");
	jQuery(".right_bottom .max_box").click(function(){
		jQuery(".right_bottom>.dialog_box_upload").show();
		jQuery(".right_bottom>.dialog_box_min").hide();
		var str = jQuery(".right_bottom>.dialog_box_upload").css("margin-bottom");
		str = str.substring(0, str.length - 2);
		Show(str);
	});
	jQuery(".dialog_box_upload .dialog_btn_short:eq(0)").click(function(){
		jQuery(".right_bottom .close_box").trigger("click");
	});
	jQuery(".right_bottom .close_box").click(function(){
		jQuery(".right_bottom .min_box").trigger("click");
		jQuery(".right_bottom>.dialog_box_min").hide();
		swfu.cancelQueue();
		jQuery(".dialog_upload_table>li").remove();
	});
	jQuery(".right_bottom .min_box").trigger("click");
	jQuery(".right_bottom>.dialog_box_min").hide();	  
	  
	jQuery(".logo").click(function(){
		var f1 = jQuery(".upload_list>li:has(.upload_icon_file)");
		var r1 = f1.sorted("",1);
		jQuery(".upload_list>li:has(.upload_icon_file)").remove();
		var f2 = jQuery(".upload_list>li");
		var r2 = f2.sorted("",1);
		jQuery(".upload_list>li").remove();
		jQuery(".upload_list").append(r1).append(r2);
	});
	  
	jQuery(".main_flie_item>a").click(function(){
		if(jQuery(".upload_list:has(:text)").length != 0){
		    jQuery(".upload_list :text").focus().select();
		    return;
		}
		if(jQuery(this).index() == 0){
			rankBySize();
		}else{
			rankByTime();
		}
	});

       jQuery(".share_new").mouseenter(function(){
           var liObj = jQuery(".upload_list>li:has(.dialog_checkbox_checked)");
           var liId = liObj.attr("id");
           if (liId == null || liId.length == 0) {
               liId = "t" + tempId;
           } else {
               liId = "o" + liId;
           }
           var id = "{name" + ":\"" + liObj.find(".main_icons").text().trim() +"\",id:\"" +liId + "\"}";
           //share_data = "{name" + ":\"" + liObj.attr("name") +"\",id:\"" +liId + "\"}";
           //share_data = {name : liObj.attr("name"), id : liId};
           share_post.name = liObj.attr("name");
           share_post.id = liId;
           share_post.size = liObj.find(".main_file_size").text().trim();
           post_obj = liObj;
           jQuery(".share_new").attr("data-share", data_share + "'" + id + "'}");
       });
});

jQuery(document).bind({
	keydown:function(e){
		if(e.keyCode==116){
			/*var ee = window.event || e;
			ee.keyCode=0;
			ee.returnValue=false;*/
		}
		if(e.keyCode==16){
			document.body.onselectstart=function(){return false;};
			SHIFTDOWN = true;
		}
	},
	keyup:function(e){
		if(e.keyCode==16){
			document.body.onselectstart=function(){};
			SHIFTDOWN = false;
		}
	}
});

function rankBySize(){
	var rank = RANKSIZE;
	if(RANKSIZE == "up"){
		RANKSIZE = "down";
	}else{
		RANKSIZE = "up";
	}
	var f1 = jQuery(".upload_list>li:has(.upload_icon_file)");
	var f2 = jQuery(".upload_list>li:not(:has(.upload_icon_file))");
	var r1 = f1;
	var r2 = f2.sorted(rank,"size");
	jQuery(".upload_list>li").remove();
	jQuery(".upload_list").append(r1).append(r2); 
	setCheckBox();
}

function rankByName(){
	var rank = "down";
	var f1 = jQuery(".upload_list>li:has(.upload_icon_file)");
	var f2 = jQuery(".upload_list>li:not(:has(.upload_icon_file))");
	var r1 = f1.sorted(rank,"name");
	var r2 = f2.sorted(rank,"name");
	jQuery(".upload_list>li").remove();
	jQuery(".upload_list").append(r1).append(r2); 
	setCheckBox();
}

function rankByTime(){
	var rank = RANKTIME;
	if(RANKTIME == "up"){
		RANKTIME = "down";
	}else{
		RANKTIME = "up";
	}
	var f1 = jQuery(".upload_list>li:has(.upload_icon_file)");
	var f2 = jQuery(".upload_list>li:not(:has(.upload_icon_file))");
	var r1 = f1.sorted(rank,"time");
	var r2 = f2.sorted(rank,"time");
	jQuery(".upload_list>li").remove();
	jQuery(".upload_list").append(r1).append(r2); 
	setCheckBox();
}

function myDocument(){
	jQuery(".upload_list>li").remove();
        jQuery(".upload_title_con>a:eq(0)~:not(:last)").css("display", "none");
	jQuery(".header_nav>a.header_nav_btn_act").removeClass("header_nav_btn_act");
	jQuery(".header_nav>:eq(0)").addClass("header_nav_btn_act");
	jQuery(".direct_bar :not(.search_bar)").show();
	jQuery(".main_error").hide();
	jQuery(".upload_list").show();
	jQuery("h2.direct_info").html("我的人人网盘");
	freshList("","myfile");
}

function myShare(){
	jQuery(".header_nav>a.header_nav_btn_act").removeClass("header_nav_btn_act");
	jQuery(".header_nav>:eq(1)").addClass("header_nav_btn_act");
	jQuery(".upload_title_con>a:eq(0)~:not(:last)").css("display", "none");
	jQuery(".search_input").val("");
	jQuery("h2.direct_info").hide();
	jQuery(".direct_func_bar>a").hide();
	jQuery(".main_error").hide();
	jQuery(".upload_list").show();
	jQuery(".upto_btn").hide();
	jQuery(".main_file_name>a").removeClass("dialog_checkbox_checked");
	shareList();
}

function getCurrentPath(top){  //获取路径，当top为空或为0是表示当前路径，为-1是表示上一级，一次类推
	if(!top)  top=0;
	var arr=jQuery(".main>div:eq(0)>h2>a");
	var len=arr.length;
	var path=BASEPATH;
	for(var i=1;i<len+top;i++)
		path+=arr.eq(i).text()+"/";
	return path;
}

function importQuota(){
	jQuery.post(QUOTAURL,null,function(resp){
		if(resp.charAt(0) == '{'){
		    var data = eval('['+resp+']');
		    ALL = data[0].quota;
		    USED = data[0].used;
		    var temp = parseInt(data[0].used*149/data[0].quota+2);
		    jQuery(".drive_room_progress:eq(0)").css({"width": temp + "px"});
		    jQuery(".drive_room_info:eq(0)").text(Size(data[0].used)+"/"+Size(data[0].quota));
		    if(SHARE == null){
				jQuery.post(SHARELISTURL,null,function(resp){
					if(resp == "fail"){
						resp = "[]";
					}
					SHARE = eval(resp);
					freshList(BASEPATH);
				},"text");
			}
		}else{
			window.location.href = resp;
		}
	},"text");
}

function cancelShare(){
	var obj=jQuery(".upload_list li:has(.dialog_checkbox_checked)");
	var arr = new Array();
	var len=obj.length;
	var paths="";
	for(var i = 0; i < len; ++i)
		arr.push(obj.eq(i).attr("id"));
	paths += arr.join("\r\n");
	jQuery.post(CANCELSHAREURL,{path : paths},function(text){
		if(text=="success"){
			obj.remove();
			var data = SHARE;
	        var len = data.length;
	        var Json = eval("[]");
	        for(var i = 0; i < len; ++i){
	        	if(!arr.have(data[i].id)){
	        		Json.push(SHARE[i]);
	        	}
	        }
	        SHARE = Json;
			jQuery(".upload_title_con:eq(0)").children(":gt(4):lt(2)").hide();
		}else{
		    error_msg("取消失败，请稍后再试");
		}
	},"text");
}

function sendMessage(){	
	var id = jQuery(".upload_list>li:has(.dialog_checkbox_checked)").attr("id");
	if (id == null || id.length ==0) {
		jQuery.post(CREATELINKURL,null,function(data){
			if (data != "fail") {
				id = data;
				var text = "我通过人人网盘分享了好东西，你也来看看呗~\n"
						 + "查看地址：" + SHAREBASEURL + id + "\n"
				         + "          还没有用人人桌面？赶紧试试吧！im.renren.com";
				jQuery(".mail_item>textarea").val(text);
				jQuery(".mail_item>textarea").attr("id", id);
				showDialog(jQuery(".dialog:eq(0)"),"发邮件给好友");
			} else {
				error_msg("创建链接失败");
			}
		},"text");
	} else {
		var text = "我通过人人网盘分享了好东西，你也来看看呗~\n"
			     + "查看地址：" + SHAREBASEURL + id + "\n"
	             + "          还没有用人人桌面？赶紧试试吧！im.renren.com";
		jQuery(".mail_item>textarea").val(text);
		showDialog(jQuery(".dialog:eq(0)"),"发邮件给好友");
	}
}

function error_msg(str){
	var lt = jQuery(".main_file_name").offset().left + jQuery(".main_file_name").width();
	var tp = jQuery(".main_file_name").offset().top;
	var wd = jQuery(".main_flie_item").offset().left - lt;
	var divObj = jQuery("<div><font color='red'>" + str + "</font></div>");
	divObj.css({"left" : lt + "px", "top" : tp + 8 + "px", "width" : wd + "px", 
		      "height" : "34px", "position" : "absolute", "z-index" : 90, 
		      "text-align" : "center", "padding-top" : "8px", "margin-top" : "-8px",
		      "background-color" : "#E6F1FB","line-height" : "26px"});
	jQuery("body").append(divObj);
	jQuery("body").one({
		"mousedown":function(){
			divObj.remove();
			jQuery("body").unbind("mousemove");
			jQuery("body").unbind("keydown");
		},
		"mousemove":function(){
			divObj.remove();
			jQuery("body").unbind("mousedown");
			jQuery("body").unbind("keydown");
		},
		"keydown":function(){
			divObj.remove();
			jQuery("body").unbind("mousedown");
			jQuery("body").unbind("mousemove");
		}
	});
}

function copyLink(){
	COPYFLAG = false;
	var id = jQuery(".upload_list>li:has(.dialog_checkbox_checked)").attr("id");
	if(id != null && id.length != 0){
		jQuery(".link_descrip").val(SHAREBASEURL + id);
		showDialog(jQuery(".dialog:eq(0)"),"创建链接");
	}else{
		jQuery.post(CREATELINKURL,null,function(data){
			if(data!="fail"){
				jQuery(".link_descrip").val(SHAREBASEURL + data);
				showDialog(jQuery(".dialog:eq(0)"),"创建链接");
			}
		},"text");
	}
}

function moveTo(){
	showDialog(jQuery(".dialog:eq(0)"),"移动到");
}

function copyTo(){
	showDialog(jQuery(".dialog:eq(0)"),"复制到");
}

function upload(){ // 上传
	jQuery(".right_bottom .max_box").trigger("click");
}

/**
 * 分享列表
 */
function shareList(){
    var data = SHARE;
    jQuery(".upload_list>li").remove();
    for(var i=0;i<data.length;i++)
    if(data[i] != null){
		var obj=data[i];
		var fcss = getFileTypeCss(obj.path);
		var html = "<li class='clear fix' id='" + obj.id + "'> <span class='upload_file_name clear fix'>"
			      +"<a class='dialog_icons dialog_checkbox'></a>"
			      +"<a class='main_icons upload_icon_img " + fcss + "'>" + getName(obj.path) + "</a>"
			      +"<span class='shared_sign'></span></span>"
		          +"<span class='main_file_size'>" + obj.size + "</span>" 
		          +"<span class='main_file_date'>" + obj.time + "</span></li>";
		jQuery(".upload_list").append(html);
	}
	setCheckBox();
}

function searchShareList(rex){
	jQuery(".upload_list>li").remove();
	jQuery(".upload_title_con>.direct_func_btn:lt(2)").hide();
	var data = SHARE;
	var len = data.length;
	for(var i = 0; i < len; ++i){
		if(data[i] == null) continue;
		var obj = data[i];
		var name = getName(obj.path);
		if(rex != null && rex.length != 0 && name.indexOf(rex) == -1){
			continue;
		}
		var fcss = getFileTypeCss(obj.path);
		var html = "<li class='clear fix' id='" + obj.id + "'> <span class='upload_file_name clear fix'>"
			      +"<a class='dialog_icons dialog_checkbox'></a>"
			      +"<a class='main_icons upload_icon_img " + fcss + "'>" + getName(obj.path) + "</a>"
			      +"<span class='shared_sign'></span></span>"
		          +"<span class='main_file_size'>" + obj.size + "</span>" 
		          +"<span class='main_file_date'>" + obj.time + "</span></li>";
		jQuery(".upload_list").append(html);
	}
	setCheckBox();
}

function recycleBin(){
	jQuery(".direct_bar>h2").hide();
    jQuery(".direct_bar>div>:not(:last)").hide();
	jQuery(".upload_list>li").remove();
	jQuery(".upload_list").hide();
	jQuery(".main_error").show();
	jQuery(".upto_btn").hide();
	jQuery(".main_file_name>a").removeClass("dialog_checkbox_checked");
}

function download(){
	var filename = jQuery(".upload_list>li:has(.dialog_checkbox_checked)").attr("name");
	jQuery.post(DOWNLOADURL,{path:filename},function(data){
		 if(data[0].error_code){
			 error_msg(data[0].error_msg);
		 }else{
			 path=data[0].list;
			 window.location.href=path;
		 }
	},"json");	
}

/*删除文件*/
function del(){  
	var liObj=jQuery(".upload_list>li:has(.dialog_checkbox_checked)");
	var ids = new Array();
	var arr = new Array();
	var dircs = liObj.has(".upload_icon_file");
	for (var i = dircs.length - 1; i >= 0 ; --i){
		var name = liObj.eq(i).attr("name");
		arr.push(name);
		var obj = SHARE;
		for(var j = 0; j < obj.length; ++j){
			if(obj[j].path.startWith(name + "/")){
				ids.push(obj[j].id);
			}
		}
	}
	var files = liObj.not(":has(.upload_icon_file)");
	for (var i = files.length - 1; i >= 0; --i){
		arr.push(liObj.eq(i).attr("name"));
		var id = liObj.eq(i).attr("id");
		if(id != null && id.length != 0){
			ids.push(id);
		}
	}	
	jQuery.post(DELETEURL, {param : getList(arr), id : ids.join("\r\n")}, function(resp) {
		data = eval("[" + resp + "]");
		if (data[0].error_code) {
			error_msg(data[0].error_msg);
		} else {
			ALL = data[0].quota;
		    USED = data[0].used;
			jQuery(".upload_title_con>a:eq(0)~:not(:last)").hide();
		    var temp = parseInt(data[0].used*149/data[0].quota+2);
		    jQuery(".drive_room_progress:eq(0)").css({"width": temp + "px"});
		    jQuery(".drive_room_info:eq(0)").text(Size(data[0].used)+"/"+Size(data[0].quota));
		    /*var rest =  jQuery(".upload_list>li:not(:has(.dialog_checkbox_checked))");
			jQuery(".upload_list>li").remove();
			jQuery(".upload_list").append(rest);
			setCheckBox();*/
		    jQuery(".upload_list>li:has(.dialog_checkbox_checked)").remove();
		}
	}, "text");
	var len = SHARE.length;
	var Json = eval("[]");
	for(var i = 0; i < len; i++){
		if(!ids.have(SHARE[i].id)){
			Json.push(SHARE[i]);
		}
	}
	SHARE = Json;
}

function rename(){  //重命名
	var showObj = jQuery(".upload_title_con>.sub_menu_box:visible");
	showObj.css("display" , "none");
	var spanObj = jQuery(".upload_list>li:has(.dialog_checkbox_checked)>span:eq(0)");
	var old = spanObj.children("a:eq(1)").clone(true);
	var str = old.text().trim();	
	var tempObj = jQuery('<span>'
	              +'<input class="upload_input" type="text"/>'
				  +'<em class="main_icons icon_right2"></em>'
				  +'<em class="main_icons icon_wrong2"></em>'
				  +'</span>');
	tempObj.attr("class",old.attr("class"));
	spanObj.children("a:eq(1)").replaceWith(tempObj);
	inputObj = tempObj.find(":text");
	inputObj.val(str).keypress(function(e){
		if(e.keyCode == 47)  return;
		if(e.keyCode == 13)
			tempObj.find("em").eq(0).trigger("click");
	});
	textSelect(inputObj);
	var oldname = inputObj.val();
	tempObj.find("em").click(function(){
		if(jQuery(this).index() == 1){
			var newname = inputObj.val();
			if(newname.length != 0 && newname != oldname){  
				var liObj = jQuery(this).closest("li");
				oldname = liObj.attr("name");
				var cur_path = oldname.substr(0, oldname.lastIndexOf("/")) + "/"; 
				newname = cur_path + newname;
				jQuery.post("rename",{from: oldname, to: newname},function(resp){
					if(resp.error_code){
						textSelect(inputObj);
					}else{
						data = resp[0].list[0];
						old.text(getName(data.path));
						tempObj.replaceWith(old);
						spanObj.nextAll(".main_file_date").text(getFileTime(data.mtime));
						liObj.attr("name",data.path);
						showObj.show();
					}
			    },"json");
			} else {
				//textSelect(inputObj);
				if (newname.length != 0) {
					tempObj.replaceWith(old);
					showObj.show();
				} else {
					inputObj.focus();
				} 
			}
		}else{
			tempObj.replaceWith(old);
			showObj.show();
		}
	});
}

function New(){ // 新建文件夹
	if(jQuery(".upload_list>li:has(:text)").length != 0){
		jQuery(".upload_list>li :text").focus().select();
		return;
	}
	
	var showObj = jQuery(".upload_title_con>.sub_menu_box:visible");
	showObj.css("display" , "none");
	var liObj = jQuery('<li class="clearfix"><span class="upload_file_name clearfix">'
	             +'<a class="dialog_icons dialog_checkbox" href="#"></a>'
	             +'<span class="main_icons upload_icon_img upload_icon_file clearfix" href="#">'
	             +'<input class="upload_input" type="text" value="">'
	             +'<a class="main_icons icon_right2" href="#"></a>'
	             +'<a class="main_icons icon_wrong2" href="#"></a>'
	             +'</span><span class="shared_sign"></span></span>'
	             +'<span class="main_file_size">-</span>'
	             +'<span class="main_file_date">xxxx-xx-xx xx:xx</span></li>');
	jQuery(".upload_list").prepend(liObj);
	var inputObj = liObj.find(":text");
	inputObj.focus();
	liObj.find(".icon_right2").click( function(){
		var input_value = inputObj.val();
		if(input_value == null || input_value.trim().length == 0){
			error_msg("文件名不能为空");
			return;
		}
		var name_set = jQuery(".upload_list>li:not(:last)").find(".upload_icon_img");
		var name_check = true;
		name_set.each(function(){
			var file_name = jQuery(this).text();
			if(input_value == file_name){
				name_check = false;
				return;
			}
		});
		if(!name_check){
			error_msg("文件名已存在");
			return;
		}		
		var cur_path = getCurrentPath();
		jQuery.post(MKDIRURL, {dir : cur_path + input_value}, function(resp){
			data = eval("[" + resp + "]")[0];
			if(data.error_code){
				inputObj.focus().select();
			}else{
				liObj.attr("name", data.path);
				var aObj = jQuery("<a class='main_icons upload_icon_img upload_icon_file clearfix'>" + input_value + "</a>");
				liObj.find(".upload_icon_img").replaceWith(aObj);
				liObj.children(".main_file_date").text(getFileTime(data.mtime));
				//rankByName();
				setCheckBox(liObj);
			}
		},"text");		
	});
	liObj.find(".icon_wrong2").click( function(){
		liObj.remove();
		showObj.show();
	});
	inputObj.keypress(function(e){
		if(e.keyCode == 13){
			liObj.find(".icon_right2").trigger("click");
		}
	});
}

var picType=["jpg","jpeg","bmp","gif","png","ico","tif","pcx","tga"];
var zipType=["zip","rar","jar"];
var musicType=["mp3","wma","flac","aac","mmf","amr","ogg","mp2","wav","wavpack"];
var movieType=["mp4","avi","3gp","rmvb","gif","wmv","mkv","mpg","vob","mov","flv","swf"];
var textType=["doc","pdf","txt","docx"];
function getFileTypeCss(fname){ //根据文件扩展名选择css
	var ftype = "";
	var ok = false;
	for(var i = fname.length - 1; i >= 0; --i){
		var c = fname.charAt(i);
		if(c == '.'){
			ok = true;
			break;
		}
		ftype = c + ftype;
	}
	if(ok){
		ftype = ftype.toLowerCase();
		if(picType.have(ftype)){
			return "";
		}else if(textType.have(ftype)){
			return "upload_icon_dom";
		}else if(musicType.have(ftype)){
			return "upload_icon_music";
		}else if(movieType.have(ftype)){
			return "upload_icon_mov";
		}else if(zipType.have(ftype)){
			return "upload_icon_zip";
		}else if("exe" == ftype){
			return "upload_icon_app";
		}
	}
	return "upload_icon_unknow";
}

function getFileTime(ctime){ //将 ctime 转化为  字符串时间
	ctime *= 1000;
	var t = new Date(ctime);
	var mon = t.getMonth() + 1;
	mon = mon>9 ? mon : "0" + mon;
	var day = t.getDate() > 9 ? t.getDate() : "0"+t.getDate();
	var hours = t.getHours() > 9 ? t.getHours() : "0"+t.getHours();
	var min = t.getMinutes() > 9 ? t.getMinutes() : "0" + t.getMinutes();
	return t.getFullYear() + "-" + mon + "-" + day + " " + hours + ":" + min; 
}

function Size(data) { // 计算文件大小
	if (data < 1024)
		return data + 'B';
	data /= 1024;
	if (data < 1024)
		return Math.round(data) + 'KB';
	data /= 1024;
	if (data < 1024)
		return data.toFixed(2) + 'MB';
	data /= 1024;
	return data.toFixed(2) + 'GB';
}
 
/**
 * 复选框class切换
 */
function setCheckBox(obj){
	var Obj = null;
	if(obj == null){
		Obj = jQuery(".upload_list>li");
	}else{
		Obj = obj;
	}
	Obj.each(function(){
		jQuery(this).find("a:eq(0)").click(function(){
			var index = jQuery(this).closest("li").index();
			if(jQuery(".upload_list:has(:text)").length != 0){
			    jQuery(".upload_list :text").focus().select();
			    return;
			}
			jQuery(".sub_menu>.sub_menu_list").css("display", "none");
			jQuery(this).toggleClass("dialog_checkbox_checked");			
			if(SHIFTDOWN && jQuery(this).hasClass("dialog_checkbox_checked")){
				var before = jQuery(".upload_list>li:lt(" + index + "):has(.dialog_checkbox_checked):last").index();
				jQuery(".upload_list>li:lt(" + index + "):gt(" + before + ")").each(function(){
					jQuery(this).find("a:eq(0)").addClass("dialog_checkbox_checked");
				});
			}			
			var head = jQuery(".header_nav>.header_nav_btn_act").index();
			var num = jQuery(".upload_list>li:has(.dialog_checkbox_checked)").length;
			jQuery(".upload_title_con>:lt(10):gt(1)").css("display", "none");
			if(head == 0){
				if(num == 1){
					if(jQuery(".upload_list>li:has(.dialog_checkbox_checked)").has(".upload_icon_file").length != 0){
						jQuery(".upload_title_con>.sub_menu_box_3").show();
					}else{
						jQuery(".upload_title_con>.sub_menu_box").show();
					}
				}else if(num > 1){
					jQuery(".upload_title_con>.sub_menu_box_3").show();
				}
			}else if(head == 1){
				if(num >0){
				    jQuery(".upload_title_con>.direct_func_btn:lt(2)").show();
				}else{
					jQuery(".upload_title_con>.direct_func_btn:lt(2)").hide();
				}
			}
		});
		
	    var isdir = jQuery(this).has(".upload_icon_file").length;
		var This = this;
		jQuery(this).find(".upload_icon_img").bind({
			"click":function(){
				if(isdir){
					if(jQuery(".upload_list:has(:text)").length != 0){
					    jQuery(".upload_list :text").focus().select();
					    return;
					}
					freshList(jQuery(This).attr("name") + "/");	
				}else{
					jQuery(This).find("a:eq(0)").trigger("click");
				}
			},
			"mouseenter":function(){
				 jQuery(this).css("cursor","pointer");
			},
			"mouseleave":function(){
				 jQuery(this).css("cursor","default");
			}
	  });	
	});
}

/**
 * 清空搜索框
 */
function clearSearchInput(){
	jQuery(".search_bar>:text").attr("value", "");
}

function updatePathNavigate(url){ //更新路径导航	
	var parray = url.split("/");
	var pathNavigate = new Array();
	pathNavigate.push("我的人人网盘");
	var len = parray.length;
	if(url.endWith("/")){
		len = len - 1;
	}
	for(var i = 3; i < len; ++i){
		pathNavigate.push(parray[i]);
	}	
	var first = true;
	var html = "";
	for(var i = 0; i < pathNavigate.length; ++i){
		if(!first){
			html += ">";
		}
		html += "<a>" + pathNavigate[i] + "</a>";
		first = false;
	}
	jQuery(".direct_info").html(html);
	
	/*判断是否显示返回上一层*/
	if(jQuery(".direct_info>a").length>=2){
		jQuery(".upto_btn").show();
	}else{
		jQuery(".upto_btn").hide();
	}
	jQuery(".upload_title_con>a:eq(0)~:not(:last)").hide();
	jQuery(".main_file_name>a").removeClass("dialog_checkbox_checked");	
	
	//为每个a标签添加点击事件
	var dir="";
	jQuery(".direct_info>a").each(function(){
		jQuery(this).bind({
			"click":function(){
				if(jQuery(".upload_list:has(:text)").length > 0){
					jQuery(".upload_list :text").focus().select();
					return;
				}
				var index = jQuery(this).index();
				jQuery(".direct_info>a").each(function(j){
					if(j==0){
						dir = BASEPATH;
					}else if(j <= index){
						dir += jQuery(this).text() + "/";
					}
				});
				CURRENTPATH = dir;
				freshList(dir);
			},
		    "mouseenter":function(){
				jQuery(this).css({"deraction":"underline","cursor":"pointer"});
		    }
		});
	});
}

function search(){
	var key = jQuery(".search_input:eq(0)").val();
	if(key == null || key.length == 0){
		freshList(getCurrentPath());
	}else{
		jQuery.post(SEARCHURL, {path: getCurrentPath(), wd: key},function(resp){
			if(resp[0].error_code){
				error_msg("查询失败");
			}else{
				jQuery(".upload_list>li").remove();
				jQuery(".upload_title_con>.sub_menu_box").hide();
				var data = resp[0].list;
				var len = data.length;
				for(var i = 0; i < len; ++i){
					var obj = data[i];
					var name = getName(obj.path);
					var fcss = getFileTypeCss(name);
					var id = null;
					var temp = SHARE;
					for(var j = 0; j < temp.length; ++j){
						if(temp[j]!= null && temp[j].path == obj.path){
							id = temp[j].id;
							break;
						}
					}
					var html = "<li " + (id == null ? "":("id='" + id +"'")) + " class='clear fix' name='" + obj.path + "'> <span class='upload_file_name clear fix'>"
						      +"<a class='dialog_icons dialog_checkbox'></a>"
						      +"<a class='main_icons upload_icon_img " + fcss + "'>" + name + "</a>"
						      +"<span class='shared_sign'>" + (id == null? "" : "已分享") + "</span></span>"
					          +"<span class='main_file_size'>" + Size(obj.size) + "</span>" 
					          +"<span class='main_file_date'>" + getFileTime(obj.mtime) + "</span></li>";
				   jQuery(".upload_list").append(html);
				}
				jQuery(".direct_func_bar>.direct_func_btn").hide();
				setCheckBox();
			}
		},"json");
	}
}

function freshList(freshurl,flag){ //刷新文件列表	
	if(SHARE == null){
		jQuery.ajax({url:SHARELISTURL,async:false,success:function(resp){
			SHARE = eval(resp);
		}});
	}
    if(freshurl == null || freshurl.length ==0){
    	freshurl = BASEPATH;
    }
	jQuery.post(HOMEURL,{dir:freshurl},function(resp){
		jQuery(".upload_list>li").remove();
		if (resp.charAt(0) != "["){
			window.location.href = resp;
		}
		var data=eval(resp)[0].list;     
		for(var i = 0; i < data.length; ++i){
			obj = data[i];			
			var id = null;
			var temp = SHARE;
			var name=getName(obj.path);
			var fcss = "upload_icon_file";
			if(obj.isdir == 0){
				fcss = getFileTypeCss(name);
				for(var j = temp.length - 1; j >= 0; --j){
					if(temp[j].path == obj.path){
						id = temp[j].id;
						break;
					}
				}
			}else{
				for(var j = temp.length -1; j>= 0; --j){
					if(temp[j].path.startWith(obj.path + "/")){
						id = temp[j].id;
						break;
					}
				}
			}
			var share_sign = id == null? "" : (obj.isdir == 0? "已分享" : "含分享文件");
			var html = "<li class='clear fix' name='"+ obj.path + "' " + (id == null ? "":("id='" + id +"'")) + "> <span class='upload_file_name clear fix'>"
				      +"<a class='dialog_icons dialog_checkbox'></a>"
				      +"<a class='main_icons upload_icon_img " + fcss + "'>" + name + "</a>" 
				      +"<span class='shared_sign'>" + share_sign + "</span></span>"
			          +"<span class='main_file_size'>" +( obj.isdir?"-":Size(obj.size)) + "</span>" 
			          +"<span class='main_file_date'>" + getFileTime(obj.mtime) + "</span></li>";
			jQuery(".upload_list").append(html);
		}
		jQuery(".direct_func_bar>.direct_func_btn").show();
		setCheckBox();
		clearSearchInput();
		updatePathNavigate(freshurl);
		
		if (flag == "myfile") {
			jQuery(".btn_myshare").one("click", function(){
				myShare();
				jQuery(".btn_myfile").one("click", function(){
					myDocument();
				});
			});
		}
	},"text");
}


function getList(arr){
	var len=arr.length;
	var path="";
	if(len){
		path='{"list":[';
		for(var i=0;i<len-1;i+=1){
			path+='{"path":"'+arr[i]+'"},';
		}
		path+='{"path":"'+arr[len-1]+'"}]}';
	}
	return path ;
}

/**
 * 根据路径名获取文件名
 * @param path  
 * 路径名
 * @returns
 */
function getName(path){
	var res=path.split("/");
	var len=res.length;
	return res[len-1];
}


/**
 * 重命名时选择文本
 * @param obj
 */
function textSelect(obj){
	var str = obj.val();
	var len = str.length;
	var end = len;
	for ( var i = 0; i < len; i += 1)
		if (str.charAt(i) == '.')
			end = i;
	obj.selectRange(0, end);
}

/**
  *<input type="text"/> 
  *文本部分选择
  *start开始位置
  *end结束位置
 */
jQuery.fn.selectRange = function(start, end) {
    return this.each(function() {
        if (this.setSelectionRange) {
            this.focus();
            this.setSelectionRange(start, end);
        } else if (this.createTextRange) {
            var range = this.createTextRange();
            range.collapse(true);
            range.moveEnd('character', end);
            range.moveStart('character', start);
            range.select();
        }
    });
};

function getSize(str) {
	var len = str.length, i;
	if(str == '-')  return 0;
	for (i = len - 1; i > 0; --i)
		if (str.charAt(i - 1) >= '0' && str.charAt(i - 1) <= '9')
			break;
	var res = parseFloat(str);
	if (str.charAt(i) == 'B')
		return res;
	res *= 1024;
	if (str.charAt(i) == 'K')
		return res;
	res *= 1024;
	if (str.charAt(i) == 'M')
		return res;
	return res * 1024;
}

jQuery.fn.sorted = function(rank, method){
	  var arr = jQuery(this).get();
	  var cls;
	  if(method == 'time'){
		  cls = ".main_file_date";
	  }else if(method == "size"){
		  cls = ".main_file_size";
	  }else{
		  cls = ".upload_file_name";
	  }
	  arr.sort(function(a, b){
		  var t1 = jQuery(a).children("'" + cls + "'").text();
		  var t2 = jQuery(b).children("'" + cls + "'").text();
		  if(method == "size"){
			  var s1 = getSize(t1);
			  var s2 = getSize(t2);
			  return (s1 > s2) ? 1 : (s1 < s2) ? -1 : 0;
		  }else{
			  return (t1 > t2) ? 1 : (t1 < t2) ? -1 : 0; 
		  } 
	  });
	  if(rank == "up") arr.reverse();
	  return jQuery(arr);
};

function getCookie(name){
	var cookie = document.cookie;
	var cks = cookie.split(";");
	for(var i=0; i< cks.length; ++i){
		var s = cks[i].trim();
		var str = s.split("=");
		if(str[0] == name){
			return str[1];
		}
	}
	return null;
}
