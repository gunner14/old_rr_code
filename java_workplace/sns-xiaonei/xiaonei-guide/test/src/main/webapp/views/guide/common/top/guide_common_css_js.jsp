<%@ page pageEncoding="utf-8" %>

<script language="javascript">  
	//<![CDATA[	
    function playAudio(el,t,file){
		el = $(el + '');
		if(t == 'mp3'){
			el.innerHTML = XN.Template.flashPlayer({filename:file});
		}else{
			el.innerHTML = XN.Template.mediaPlayer({filename:file});
		}
	}	
    function playswf(el,file,scale){
		var w,h;
		el = $(el + '');
		w = el.parentNode.offsetWidth - 24;
		if(w > 500){
			w = 500;
		}
		h = parseInt(w/scale);
		el.innerHTML = XN.Template.flash({width:w,height:h,filename:file});
		el.onclick = null;
	}
	XN.namespace( 'user' );
	XN.user.id = '${userBasic.id}';
	XN.user.tinyPic = '${userBasic.tiny_Url}';
	XN.user.name = "<str:encodeUrl><c:out value='${userBasic.name}'/></str:encodeUrl>";
	function ajax_add_frd(from){
		new Ajax.Request("ajaxAddFrd.do?from="+from,{method:'post'});		
	}
	function ajax_set_mem(memKey,memExpiry){
		new Ajax.Request("ajaxGuideSetMemcacheWithExpiry.do?memKey="+memKey+"&memExpiry="+memExpiry,{method:'post'});	
	}
	function ajax_set_mem_with_expiry(memKey,memExpiry){
		new Ajax.Request("ajaxGuideSetMemcacheWithExpiry.do?memKey="+memKey+"&memExpiry="+memExpiry,{method:'post'});		
	}		
	function ajax_note_action(divid,userid){
		new Ajax.Request("ajaxNoteAction.do?divid="+divid+"&userid="+userid,{method:'post'});		
	}	
	function ajax_stat_link(action,ref){
		new Ajax.Request("ajaxStatLink.do?actionid="+action+"&ref="+ref,{method:'post'});		
	}
	function ajax_stat_nur_link(nur){
		new Ajax.Request("ajaxStatLink.do?nur="+nur,{method:'post'});		
	}
	function ajax_stat_page(uid){
		new Ajax.Request("http://page.renren.com/receiveLogRequest/get?ref=guidetab&uid="+uid,{method:'post'});		
	}		
	function ajax_follow_pages(){
		new Ajax.Request("http://page.renren.com/pa/bf?pid=600002283",{method:'post'});		
		new Ajax.Request("http://page.renren.com/pa/bf?pid=600002281",{method:'post'});	
		new Ajax.Request("http://page.renren.com/pa/bf?pid=600002279",{method:'post'});	
		new Ajax.Request("http://page.renren.com/pa/bf?pid=600002282",{method:'post'});	
	}
	function feed_img_delay_load(img){
		if (!img) return;
        if (img._loaded) return;
        img._loaded = true;
        img.setAttribute('src', img.getAttribute('lala'));
        img.removeAttribute('lala');
        img.removeAttribute('onload');
	    if ( img.getAttribute('needclip') ) clipImage(img);
	}
	// ]]>
</script>