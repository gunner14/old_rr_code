<%@ page contentType="text/html;charset=UTF-8" session="false" %>
<link href="${applicationScope.urlSmallStatic}/csspro/apps/home-all.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/xn.app.status.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/xn.app.ilike.js?ver=$revxxx$"></script>
<script>
	XN.getFileVersion(
    [
        '${applicationScope.urlSmallStatic}/jspro/xn.app.importFriendsFromKaixin.js?ver=$revxxx$'
    ]);    
    XN.dynamicLoad(
    {
        file : '${applicationScope.urlSmallStatic}/jspro/xn.app.share.js?ver=$revxxx$',
        funcs : ['create_share_div','create_share_feed'] 
    });
	function hideInfo() {
		XN.cookie.set("homeNtcInf", "0", 10000, "/", ".${applicationScope.sysdomain}");
		if($('info')) $('info').hide();
	}
	function showInfo() {
		if(XN.cookie.get("homeNtcInf") == null) {
			if($('info')) $('info').show();
		}
	}
	XN.dom.ready(showInfo);
	XN.namespace( 'user' );
	XN.user.id = '${userBasic.id}';
	XN.user.tinyPic = '${userBasic.tiny_Url}';
	XN.user.name = "<str:encodeUrl><c:out value='${userBasic.name}'/></str:encodeUrl>";
	function closewarn(){
		new Ajax.Request("closewarn.do",{method:'get'});							
	}	
	function close_info_complete(from){
		new Ajax.Request("ajaxCloseInfo.do?from="+from,{method:'post'});		
	}
	function ajax_stat_link(action,from){
		new Ajax.Request("ajaxStatLink.do?action="+action+"&from="+from,{method:'post'});		
	}
	function ajax_stat_page(uid){
		new Ajax.Request("http://page.renren.com/receiveLogRequest/get?ref=hometab&uid="+uid,{method:'post'});		
	}

(function()
{
    feed_imgs = [];
    status_imgs = [];
    var img_index = 0;
	var i_am_out = false;
	var timer  = null;
    var is_feed_delay_ing = true;

	if ( !XN.browser.IE && !XN.browser.Gecko ) is_feed_delay_ing = false;


    function feed_img_load(img)
    {
        if (!img) return;
        if (img._loaded) return;
        img._loaded = true;
        img.setAttribute('src', img.getAttribute('lala'));
        img.removeAttribute('lala');
        img.removeAttribute('onload');
	    if ( img.getAttribute('needclip') ) clipImage(img);
    }

    function pop_imgs(imgs)
    {
        if (!imgs) return false;
        if (img_index >= imgs.length) return false;
        var img = imgs[img_index];
        setTimeout(function(){feed_img_load(img);}, 0); 
        return true;
    }
    function stop()
    {
        if (timer){ clearInterval( timer );timer = null;}
        feed_imgs = status_imgs = null;
        is_feed_delay_ing = false; 
    }
    function scroll_show_imgs()
    {
        var f = pop_imgs(feed_imgs);
        var s = pop_imgs(status_imgs);
        if ( !f && !s )
	    {
            stop();     
            return;
        }
        img_index ++;
    }

    feed_delay_load_all = function()
    {
        XN.array.each(feed_imgs, function(i, v)
        {
            feed_img_load(v);
        });
        XN.array.each(status_imgs, function(i, v)
        {
            feed_img_load(v);
        });
        stop();
    }

	if ( XN.browser.IE || XN.browser.Gecko )
	{
        var c_scroll = XN.event.scrollTop();
        timer = setInterval(function()
        {
            var scroll = XN.event.scrollTop();
            if ( c_scroll != scroll )
            {
                var ss = Math.abs(c_scroll - scroll);
                c_scroll = scroll;
                if ( ss > 300 )
                {
                    feed_delay_load_all();
                }
                else
                {
                    scroll_show_imgs();
                    scroll_show_imgs();
                }
            }
        },100);
	}

    feed_img_delay_load = function(img, flag)
    {
        if (!is_feed_delay_ing)
        {
            feed_img_load(img);
            return;
        }

        if ( flag )
        {
            var imgs = status_imgs;
        }
        else
        {
            var imgs = feed_imgs;
        }

        img.removeAttribute('onload');
        imgs.push(img);
	    if ( i_am_out ) return;
        var t = XN.element.realTop(img);
        var h = XN.event.winHeight() + XN.event.scrollTop();
        if ( t <= h )
        {
             feed_img_load(img);
        }
        else
        {
            i_am_out = true;
        }
    }

 })();
</script>

