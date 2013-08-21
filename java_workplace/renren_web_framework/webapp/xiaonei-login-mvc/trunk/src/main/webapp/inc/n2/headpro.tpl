<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<meta http-equiv="X-UA-Compatible" content="IE=EmulateIE7" />
<meta name="Description" content="{{siteDescription}}" />  {{!domainIndentified}}
<meta name="Keywords" content="{{siteKeywords}}" />        {{!domainIndentified}}
{{#INC_COMMON_CSS}}
<link href="{{url_small_static}}/{{csspro_base_layout_css_version}}/csspro/base/layout.css" rel="stylesheet" type="text/css" media="all"/>
{{/INC_COMMON_CSS}}
<link href="{{url_small_static}}/favicon-{{siteNameAbbr}}.ico?ver=2" rel="shortcut icon" type="image/x-icon" />  {{!domainIndentified}}
<link href="{{url_small_static}}/wap/apple_icon_.png" rel="apple-touch-icon" />
<script type="text/javascript">
XN = {get_check:'{{postFormSession}}',env:{domain:'{{domain}}',shortSiteName:'{{siteName]}}',siteName:'{{siteName}}网'}};
</script>
<script type="text/javascript" src="{{url_small_static}}/{{jspro_base_js_version}}/jspro/base.js"></script>
<script type="text/javascript">
XN.getFileVersion(
		[
		'{{url_small_static}}/csspro/apps/profile.css?ver=revxxx',
		'{{url_small_static}}/jspro/xn.app.recommendFriend.js?ver=revxxx',
		'{{url_small_static}}/jspro/xn.ui.pager.js?ver=revxxx',
		'{{url_small_static}}/jspro/xn.ui.multiFriendSelectorBox.js?ver=revxxx',
		'{{url_small_static}}/csspro/module/friendSelector.css?ver=revxxx',
		'{{url_small_static}}/jspro/xn.app.addFriend.js?ver=revxxx',
		'{{url_small_static}}/jspro/xn.ui.multiFriendSelectorBox.forGuide.js?ver=revxxx',
		'{{url_small_static}}/jspro/xn.app.status.js?ver=revxxx',
		'{{url_small_static}}/jspro/xn.app.webpager.js?ver=revxxx',
		'{{url_small_static}}/jspro/xn.app.ilike.js?ver=revxxx',
		'{{url_small_static}}/jspro/lib/mediaplayer.js?ver=revxxx',
		'{{url_small_static}}/allunivlist.js?ver=revxxx',
		'{{url_small_static}}/jspro/album/ZeroClipboard.js?ver=revxxx',
		'{{url_static}}/swf/album/ZeroClipboard.swf?ver=revxxx',
		'{{url_small_static}}/jspro/music/music.for.feed.js?ver=revxxx',
    '{{url_small_static}}/jspro/xn.ui.emoticons.js?ver=revxxx',
    '{{url_small_static}}/csspro/module/minieditor.css?ver=revxxx',
		'{{url_small_static}}/csspro/module/status-pop.css?ver=revxxx'
		]);
XN.dynamicLoad({
	file : '{{url_small_static}}/{{jspro_xn_app_share_js_version}}/jspro/xn.app.share.js',
	funcs : ['create_share_div','create_share_feed','create_share_edm','create_share_popup','create_share_jebe'] 
});

XN.namespace( 'user' );
XN.user.id = '{{visiter_id}}';
XN.user.tinyPic = '{{visiter_tiny_full_url}}';
XN.user.name = "<str:encodeUrl><c:out value='{{visiter_name}}'/></str:encodeUrl>";
try{
$extend(XN.user,{
    userState : {
        experiment : '{{vistor_userstate_experiment}}' == 'true'
    }
});
}catch(e){

}

{{#INC_NO_JEBE}}load_jebe_ads(1){{/INC_NO_JEBE}}
{{#ADD_GUIDE_BAR}}
XN.DOM.readyDo(function() {GuidBar.addBar();});
{{/ADD_GUIDE_BAR}}
</script>
