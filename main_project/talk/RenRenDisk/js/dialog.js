var METAURL="metas";
var LISTURL="list";
var COPYURL="copy";
var MOVEURL="move";
var CUROBJ;
var CURPATH= BASEPATH;
//var clip = null;

function getSubList(obj){
	path=obj.children("span:eq(0)").attr("name");
	jQuery.post(LISTURL,{dir:path},function(data){
		  if(data.error_code){
			  return;
		  }else{
			  var arr=new Array();
			  var dat=data[0].list;
			  var len=dat.length;
			  var div=obj.parents("li").length;
			  var tempObj=null;
			  for(var i=0;i<len;i++)
			  if(dat[i].isdir){
				   if(tempObj==null){
					   tempObj=jQuery("<ul class='file_list_sub'></ul>");
					   obj.append(tempObj);
				   }
				   var spanObj=jQuery('<span class="file_list_li" name="'+dat[i].path+'">'
						        +'<span class="dialog_icons icon_empty"><span class="dialog_icons icon_file file_list_name">'
						        +getName(dat[i].path)+'</span></span></span>');
				   spanObj.css({"padding-left":div*27+"px"});
				   var liObj=jQuery('<li></li>').append(spanObj);				 
				   tempObj.append(liObj);
				   arr.push(dat[i].path);
			  }
			  if(tempObj!=null) setIcon(tempObj,arr);
		  }
	  },"json");
}


function setIcon(obj,arr){
	var temp=getList(arr);
	jQuery.post(METAURL,{param:temp},function(data){
		if(data.error_code){
			return;
		}else{
			var dat=data[0].list;
			var len=dat.length;
			var tempObj=obj.children("li");
			for(var i=0;i<len;i++){
				if(dat[i].ifhassubdir){
					tempObj.eq(i).find("span").eq(1).removeClass("icon_empty").addClass("icon_fold");
				}
				tempObj.eq(i).find("span:eq(0)").click(function(){
					 addClick(jQuery(this));
				});
			}
		}
	},"json"); 	
}

function addClick(obj){
	if(obj.find(".icon_fold").length){
		if(obj.next("ul").length){
			obj.next("ul").show();
		}else{
			path=obj.attr("name");
			getSubList(obj.parent("li"),path);
		}
		obj.children("span").removeClass("icon_fold").addClass("icon_unfold");
	}else if(obj.find(".icon_unfold").length){
		obj.children("span").removeClass("icon_unfold").addClass("icon_fold");
		obj.next("ul").hide();
		obj.next("ul").find(":text:eq(0)").closest("li").remove();
	}
	jQuery(".file_list_active:eq(0)").removeClass("file_list_active");
	obj.addClass("file_list_active");
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

function dialogMove(obj){
	jQuery(".dialog_box_title").mousedown(function(e){
		var curX = obj.offset().left;
		var curY = obj.offset().top;
		var mouseX = e.pageX ;
		var mouseY = e.pageY;
		jQuery(this).mousemove(function(e){
			curX += e.pageX-mouseX;
			curY += e.pageY-mouseY;
			mouseX = e.pageX ;
			mouseY = e.pageY;
			obj.css({
				"left":curX,
				"top":curY
			});
		});
		jQuery(this).mouseup(function(){
			jQuery(this).unbind("mousemove");
		});
		jQuery(this).mouseleave(function(){
			jQuery(this).unbind("mousemove");
		});
	});
}

function newDir(dialogObj){
	var textObj=dialogObj.find("ul:eq(0) :text");
	if(textObj.length!=0){
		textObj.trigger("focus").trigger("select");
		return;
	}
	var liObj=jQuery(".file_list_active:eq(0)").closest("li");
	if(liObj.children("ul").length==0){
		liObj.append('<ul class="file_list_sub"></ul>');
	}
	var ulObj=liObj.children("ul:eq(0)");
	ulObj.show();
	if(!liObj.has(".file_list_root")){
		liObj.find("span").eq(1).removeClass("icon_empty").addClass("icon_unfold");
	}
	addLi(ulObj);
}

function addLi(obj){
	var div=obj.parents("ul").length-1;
	var spanObj=jQuery('<span class="file_list_li"><span class="dialog_icons icon_empty"><span class="dialog_icons icon_file file_list_name clearfix">'
	             +'<input type="text" value="新建文件夹" style="height:15px;margin-top:3px;line-height:15px;margin-left:-7px; float:left"/>'
	             +'</span></span></span>');
    spanObj.css({"padding-left":div*27+"px"});
    var liObj=jQuery('<li></li>').append(spanObj);
    obj.append(liObj);
    var inputObj=spanObj.find(":text");
    var sureObj = jQuery('<a class="main_icons icon_right2" href="#"></a>');
    /*var sureObj=jQuery('<input type="button"/>');
	/*sureObj.css({"background":"url(http://s.xnimg.cn/actimg/12skydrive/img/dialog_icons.png) no-repeat",
		         "background-position-y":"-145px",
		         "width":"16px",
		         "height":"16px",
		         "margin-left":"10px",
		         "border":"0px"});*/
    var cancelObj = jQuery('<a class="main_icons icon_wrong2" href="#"></a>');
	/*var cancelObj=jQuery('<input type="button"/>');
	/*cancelObj.css({"background":"url(http://s.xnimg.cn/actimg/12skydrive/img/dialog_icons.png) no-repeat",
		           "background-position-y":"-161px",
		           "width":"16px",
		           "height":"16px",
		           "margin-left":"10px",
		           "border":"0px"});*/
	inputObj.after(cancelObj).after(sureObj);
	inputObj.focus(function(){
		jQuery(".file_list_active:eq(0)").removeClass("file_list_active");
		inputObj.parents("li:eq(1)").children("span:eq(0)").addClass("file_list_active");
	});
	inputObj.trigger("select");
	inputObj.keypress(function(e) {
		if (e.keyCode == 47)
			return;
		if (e.keyCode == 13)
			sureObj.trigger("click");
	});
	sureObj.click(function(){
		var dirname=inputObj.val();
		if(dirname.length==0)  return;
		var directory=jQuery(".file_list_active:eq(0)").attr("name");
		if (!directory.endWith("/")){
			directory += "/";
		}
		jQuery.post(MKDIRURL,{dir : directory + dirname},function(data){
			if(data.error_code){
				textObj.trigger("focus").trigger("select");
			}else{
				inputObj.nextAll().remove();
				inputObj.remove();
			    spanObj.find("span").eq(1).html(dirname);
			    jQuery(".file_list_active:eq(0)").removeClass("file_list_active");
			    spanObj.addClass("file_list_active");
			    spanObj.attr("name", directory + dirname);
			    spanObj.click(function(){
			    	addClick(spanObj);
			    });
			}
		},"json");
	});
	cancelObj.click(function(){
		var ul_temp=jQuery(this).closest("ul");
		jQuery(this).closest("li").remove();
		if(ul_temp.children("li").length==0&&ul_temp.children(".file_list_root").length == 0){
				ul_temp.closest("li").find("span:eq(1)").removeClass("icon_unfold").addClass("icon_empty");
				ul_temp.remove();
		}
	});
}

function sure(dialogObj){
	var textObj=dialogObj.find("ul:eq(0) :text");
	if(textObj.length!=0){
		textObj.trigger("focus").trigger("select");
		return;
	}
	var operateName=dialogObj.find("h2>span:eq(0)").text();
	var op=MOVEURL;
	var directory=jQuery(".file_list_active:eq(0)").attr("name");
	if(operateName=="复制到")  op=COPYURL;
	var liObj=jQuery(".upload_list>li:has(.dialog_checkbox_checked)");
	var len=liObj.length;
	if(len==1){
        var filename=liObj.find(".upload_icon_img").text().trim();
        var file = liObj.eq(0).attr("name");
		jQuery.post(op,{from:file,to:directory+"/"+filename},function(data){
			dialogObj.find(".close_box").trigger("click"); 
			if(!data.error_code){
				 freshList(directory);
			 }else{
				 error_msg(operateName+"失败!");
			 }
			 if(op == COPYURL){
				 importQuota();
			 }
		},"json");
	}else{
			var arr = new Array();
			var filesname = new Array();
			for ( var i = 0; i < len; i += 1) {
				arr.push(liObj.eq(i).find(".upload_icon_img").text().trim());
				filesname.push(liObj.eq(i).attr("name"));
			}
			var jsonString = getParam(filesname, directory + "/" , arr);
			jQuery.post(op+"s", {param : jsonString}, function(data) {
				dialogObj.find(".close_box").trigger("click");
				if (!data.error_code) {
					freshList(directory);
				} else {
					error_msg(operateName + "失败!");
				}
				if(op == COPYURL){
					 importQuota();
				}
				dialogObj.find(".close_box").trigger("click");
			}, "json");
		}	   
}

function bindFunction(dialogObj){
	dialogObj.find(".close_box").click(function(){
		 dialogObj.children("div:visible").css({"display":"none"});
		 dialogObj.css({"display": "none"});
		 dialogObj.find(".file_list ul").remove();
		 jQuery(".separate").css({"display": "none"});
	});
	dialogMove(dialogObj);
	dialogObj.find(".file_list>li>span:eq(0)").click(function(){
		jQuery(".file_list_active:eq(0)").removeClass("file_list_active");
		jQuery(this).addClass("file_list_active");
	});
	dialogObj.find(".dialog_box_filelist .dialog_btn_bg").click(function(){
		var text = jQuery(this).text();
		switch(text){
		case "新建目录":newDir(dialogObj);break;
		case "确定":sure(dialogObj);break;
		default:dialogObj.find(".close_box").trigger("click");
		}
	});
	
    jQuery(".dialog_box_mail:eq(0) .dialog_btn_short").click(function(){
    	var liObj = jQuery(".upload_list>li:has(.dialog_checkbox_checked):eq(0)");
    	var filepath = liObj.attr("name");
    	var text = jQuery(".mail_item>:text").val();
    	var ct = jQuery(".mail_item>textarea").val();
    	var rex = /^([a-zA-Z0-9_\.\-])+\@(([a-zA-Z0-9\-])+\.)+([a-zA-Z0-9]{2,4})+(\;([a-zA-Z0-9_\.\-])+\@(([a-zA-Z0-9\-])+\.)+([a-zA-Z0-9]{2,4})){0,9}$/;
    	if(! rex.test(text)){
    		jQuery(".mail_item>:text").trigger("focus").trigger("select");
    		return;
    	}
    	//var shareId = jQuery(".upload_list>li:has(.dialog_checkbox_checked):eq(0)").attr("id");
    	var shareId = jQuery(".mail_item>textarea").attr("id");
    	var sz = liObj.children(".main_file_size").text();
    	jQuery.post(SENDMAIL,{receivers : text, content : ct, id : shareId, filename : filepath, size: sz}, function(resp){
    		if(resp == "fail"){
				error_msg("邮件发送失败");
			}else if(resp != "success"){
				dialogObj.find(".close_box").trigger("click");
				if(liObj.attr("id") == null || liObj.attr("id").length ==0){
					liObj.attr("id", shareId);
					liObj.find(".shared_sign").text("已分享");
					var data = {id : shareId, time : resp, path : filepath, size : sz};
					SHARE.push(data);
				}
			}
			dialogObj.find(".close_box").trigger("click");
			jQuery(".mail_item>:text").val("");
			jQuery(".mail_item>textarea").val("");
			jQuery(".mail_item>textarea").attr("id", "");
		},"text");
    });
    
    jQuery(".dialog_box_edit .dialog_btns>a:eq(0)").click(function(){
    	dialogObj.find(".close_box").trigger("click");
    });
    
    var This = this;
    jQuery(".dialog_box_edit .dialog_btns>a:eq(0)").bind({
    	"mouseover":function(){
    		if(!This.clip){
    			This.clip = new ZeroClipboard.Client();
    			This.clip.setHandCursor(true);
    			This.clip.glue("copy_sure");
    			This.clip.addEventListener("complete", function(){
    				if(COPYFLAG){
    					This.clip.hide();
	        			dialogObj.find(".close_box").trigger("click");
	        			return;
    				}
    				var id = jQuery(".upload_list>li:has(.dialog_checkbox_checked):eq(0)").attr("id");
    				if(id == null || id.length == 0){
    					var liObj = jQuery(".upload_list>li:has(.dialog_checkbox_checked):eq(0)");
        				var filename = liObj.attr("name");
        				var sz = liObj.children(".main_file_size:eq(0)").text();
        				var ti = liObj.children(".main_file_date:eq(0)").text();
        				var url = jQuery(".link_descrip").val();
        				var Id = url.substr(url.lastIndexOf("/") + 1);
        				jQuery.post(SHARELINKURL,{path : filename, size : sz, id : Id, time : ti},function(resp){
        					if(resp != "fail"){
        						This.clip.hide();
        	        			dialogObj.find(".close_box").trigger("click");
        	        			var dat = resp.split("&");
        	        			var data = {id : dat[0], time : dat[1], path : filename, size : sz};
        	        			SHARE.push(data);
        	        			liObj.attr("id",Id);
        	        			liObj.find(".shared_sign").text("已分享");
        					}
        				},"text");
    				}else{
    					This.clip.hide();
	        			dialogObj.find(".close_box").trigger("click");
    				}
        		});
    		}else{
    			This.clip.reposition("copy_sure");
    		}
    		This.clip.show();
    		This.clip.setText(jQuery(".link_descrip").val());
    		This.clip.receiveEvent("mouseover",null);
    	},
    	"mouseout":function(){
    	}
    });
    
    var black = false;
    jQuery(".mail_item>:text").bind({
    	"focus" : function() {
    		black = false;
    	},
        "keyup" : function() {
        	if(black == false && jQuery(this).val().length != 0) {
        		black = true;
        		jQuery(this).css("color", "#333");
        	}
        	if(black == true && jQuery(this).val().length == 0) {
        		black = false;
        		jQuery(this).css("color", "#C8C8C8");
        	}
        },
        "blur" : function() {
        	jQuery(this).css("color", "#C8C8C8");
        	black = false;
        }
    });
    
    jQuery(".mail_item>textarea").bind({
    	"focus" : function() {
    		black = false;
    	},
        "keyup" : function() {
        	if(black == false && jQuery(this).val().length != 0) {
        		black = true;
        		jQuery(this).css("color", "#333");
        	}
        	if(black == true && jQuery(this).val().length == 0) {
        		black = false;
        		jQuery(this).css("color", "#C8C8C8");
        	}
        },
        "blur" : function() {
        	jQuery(this).css("color", "#C8C8C8");
        	black = false;
        }
    });
}

function Show(str){
	var num = parseInt(str);
	if(num >= 0)  return;
	num += 15;
	jQuery(".right_bottom>.dialog_box_upload").css("margin-bottom", num + "px");
	setTimeout("Show('" + num + "')", 2);
}
/**
 * 弹出窗口
 * @return
 */
function showDialog(dialogObj,dialogName) {
	dialogObj.children("div").css({"display":"none"});
	if(dialogName == "创建链接"){
		dialogObj.children("div:eq(1)").show();
	}else if(dialogName == "发邮件给好友"){
		dialogObj.children("div:eq(2)").show();
	}else{
		dialogObj.children("div:eq(0)").show();	
		dialogObj.find("h2>span:eq(0)").html(dialogName);
		dialogObj.find(".file_list .file_list_root:eq(0)").addClass("file_list_active");
		getSubList(dialogObj.find(".file_list>li:eq(0)"));
	}
	var objW = jQuery(window);
	var objC = dialogObj;
	var brsw = objW.width();
	var brsh = objW.height();
	var sclL = objW.scrollLeft();
	var sclT = objW.scrollTop();
	var curW = objC.width();
	var curH = objC.height();
	var left = sclL + (brsw - curW + 1) / 2;
	var top = sclT + (brsh - curH + 1) / 2;
	objC.css({
		"left" : left,
		"top" : top
	});
	jQuery(".separate").css({"height": brsh + "px", "width": brsw + "px"}).show();
	dialogObj.show();
};

/*function getParam(cur_path,directory,arr){
    var len=arr.length;
    var param='{"list":[';
    for(var i=0;i<len-1;i++)
  	  param+='{"from":"'+cur_path+arr[i]+'","to":"'+directory+arr[i]+'"},';
    param+='{"from":"'+cur_path+arr[len-1]+'","to":"'+directory+arr[len-1]+'"}';
    return param+']}';
}*/

function getParam(filesname,directory,arr){
    var len = arr.length;
    var param = '{"list":[';
    for(var i = 0; i < len - 1; ++i)
  	  param += '{"from":"' + filesname[i] + '","to":"' + directory + arr[i] + '"},';
    param += '{"from":"' + filesname[len - 1] + '","to":"' + directory + arr[len-1] + '"}';
    return param+']}';
}

