<%@ page contentType="text/html;charset=UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/fmt" prefix="fmt" %>
<%@ taglib uri="http://jakarta.apache.org/taglibs/string-1.1" prefix="str" %>

<%--
@param INC_HOME_FRAME home frame 所需要的静态
@param pageTitle 页面title
--%>

<%-- upload如果挂了，切换upload的上传地址用 --%>
<c:set var="cur_domain_upload" value="http://upload.renren.com" />

<c:set var="KAIXIN" value="${applicationScope.domain == 'kaixin.com'}" />
<c:if test="empty PAGE_MODE">
	<c:set var="PAGE_MODE" value="topad, logo, webpager, " />
</c:if>
<%--海外用户不显示广告--%>
<c:if test="${INC_NO_JEBE == true}">
	<c:set var="cleanpage" value="true" />
</c:if>

<c:set var="INC_NO_HEADER_BANNER" value="${INC_NO_HEADER_BANNER || requestScope.visiter.userStateInfo.hideAds == true}" />

<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<meta http-equiv="X-UA-Compatible" content="IE=EmulateIE7" />
<meta name="Description" content="${domainIdentified["siteDescription"]}" />
<meta name="Keywords" content="${domainIdentified["siteKeywords"]}" />
<meta name="msapplication-task" content="name=新鲜事;action-uri=http://www.renren.com/home;icon-uri=http://a.xnimg.cn/n/res/icons/newsfeed.ico" />
<meta name="msapplication-task" content="name=日志;action-uri=http://blog.renren.com;icon-uri=http://a.xnimg.cn/n/res/icons/blog.ico" />
<meta name="msapplication-task" content="name=相册;action-uri=http://photo.renren.com;icon-uri=http://a.xnimg.cn/n/res/icons/photo.ico" />
<meta name="msapplication-task" content="name=音乐;action-uri=http://music.renren.com;icon-uri=http://a.xnimg.cn/n/res/icons/music.ico" />
<meta name="msapplication-task" content="name=分享;action-uri=http://share.renren.com;icon-uri=http://a.xnimg.cn/n/res/icons/share.ico" />
<meta name="msApplication-ID" content="App" />
<meta name="msApplication-PackageFamilyName" content="57722RenRenpreview.RenrenHD_fknrsfzqca1jw" />

<c:if test="${!empty pageTitle}"><title>${pageTitle}</title></c:if>

<link rel="shortcut icon" type="image/x-icon" href="${applicationScope.urlStatic}/favicon-${domainIdentified['siteNameAbbr']}.ico?ver=3" />
<link rel="apple-touch-icon" href="${applicationScope.urlStatic}/wap/apple_icon_.png" />

<!--[if IE]><script type="text/javascript" src="${applicationScope.urlSmallStatic}/n/core/expressions.js?ver=$revxxx$"></script><![endif]-->

<c:if test="${INC_NO_COMMON_CSS != true}">
	<link href="${applicationScope.urlSmallStatic}/n/core/global-all-min.css?ver=$revxxx$" rel="stylesheet" type="text/css" />
</c:if>

<script type="text/javascript">
XN = {get_check:'${requestScope.postFormSession}',get_check_x:'${requestScope._rtk}',env:{domain:'${applicationScope.domain}',shortSiteName:'${domainIdentified["siteName"]}',siteName:'${domainIdentified["siteName"]}网'}};

try {
	window.onerror=function(){
		var a=arguments,e=encodeURIComponent,l=location,d=new Date();
		if(a.length!=3||a[2]==0)return 1;
		new Image().src=['http://s.renren.com/speedstats/jserror/stats.php?message='+e(a[0]),
			'url='+e(a[1]),'lineNo='+a[2],'location='+e(l),'time='+d.toLocaleTimeString()].join('&');
		return 1;
	};
} catch (e){};

var FLASH_UPLOADER_URL = '${applicationScope.urlSmallStatic}/n/apps/photo/modules/flashUploader/pic_uploader_v9.swf';
var CUR_DOMAIN_UPLOAD = '${cur_domain_upload}';
<%--photo工程种子文件--%>
var photoSeedSrc= '${applicationScope.urlSmallStatic}/n/apps/photo/modules/seed/photoSeed.js?ver=$revxxx$';

</script>

<script src="${applicationScope.urlSmallStatic}/n/core/base-all.js?ver=$revxxx$" type="text/javascript"></script>

<c:if test="${!visiter.userStateInfo.guideDone}">
	<script type="text/javascript">
		XN.namespace('user');
		XN.user.isGuide = true;
        XN.user.stage = '${visiter.stage}';
	</script>
</c:if>

<%-- home-frame --%>
<c:if test="${INC_HOME_FRAME == true}">
	<link href="${applicationScope.urlSmallStatic}/n/core/home-frame-all-min.css?ver=$revxxx$" rel="stylesheet" type="text/css" />
	<script type="text/javascript" src="${applicationScope.urlSmallStatic}/n/core/js/home-frame.js?ver=$revxxx$"></script>
</c:if>


<%--webpager app menu--%>
<c:if test="${not empty requestScope.host}">
	<script type="text/javascript">
		var wpiMenuInfo = {recentMenu : [<c:forEach var="item" items="${requestScope.userMenu.recentMenu}" varStatus="status" begin="0" end="8">{"id":"${item.appId}", "name":"${item.appName}", "icon":"${item.appIcon}", "href":"${item.sideNavUrl}","target":"${empty item.urlTarget ? '_self' : '_blank'}"}${!status.last ? ',' : ''}</c:forEach>],
			favoriteMenu : [<c:forEach var="item" items="${requestScope.userMenu.favoriteMenu}" varStatus="status">{"id":"${item.appId}", "name":"${item.appName}", "icon":"${item.appIcon}", "href":"${item.sideNavUrl}","target":"${empty item.urlTarget ? '_self' : '_blank'}"}${!status.last ? ',' : ''}</c:forEach>],
			user : {"id" : ${requestScope.host.id}},
			currentApp : [<c:if test="${!empty requestScope.currentApp}"> {"id":"${requestScope.currentApp.appId}","name":"${requestScope.currentApp.appName}", "icon":"${requestScope.currentApp.appIcon}","href":"${requestScope.currentApp.sideNavUrl}","target":"${empty item.urlTarget ? '_self' : '_blank'}"} </c:if>]
		};
	</script>
</c:if>

<%--广告--%>
<c:if test="${INC_NO_JEBE != true}">
<script type="text/javascript">
	if (window.asyncHTMLManager) {
		window.asyncHTMLManager.addEvent('load', function() {
			load_jebe_ads(1, false, true);
		});
	} else {
		load_jebe_ads(1);
	}
</script>
</c:if>

<%--guide bar--%>
<c:if test="${visiter!=null && visiter.id%10==3 && !visiter.userStateInfo.guideDone && visiter.userStateInfo.socialGraphGood}">
<script>XN.DOM.readyDo(function(){GuidBar.addBar();});</script>
</c:if>

<script type="text/javascript">
XN.getFileVersion([
	'${applicationScope.urlSmallStatic}/csspro/apps/profile.css?ver=$revxxx$',
	'${applicationScope.urlSmallStatic}/jspro/xn.app.recommendFriend.js?ver=$revxxx$',
	'${applicationScope.urlSmallStatic}/jspro/xn.app.status.js?ver=$revxxx$',
	'${applicationScope.urlSmallStatic}/jspro/xn.app.share.js?ver=$revxxx$',
	'${applicationScope.urlSmallStatic}/jspro/xn.app.ilike.js?ver=$revxxx$',
	'${applicationScope.urlSmallStatic}/jspro/xn.ui.pager.js?ver=$revxxx$',
	'${applicationScope.urlSmallStatic}/jspro/xn.ui.multiFriendSelectorBox.js?ver=$revxxx$',
	'${applicationScope.urlSmallStatic}/csspro/module/friendSelector.css?ver=$revxxx$',
	'${applicationScope.urlSmallStatic}/jspro/xn.app.addFriend.js?ver=$revxxx$',
	'${applicationScope.urlSmallStatic}/jspro/xn.ui.multiFriendSelectorBox.forGuide.js?ver=$revxxx$',
	'${applicationScope.urlSmallStatic}/jspro/lib/mediaplayer.js?ver=$revxxx$',
	'${applicationScope.urlSmallStatic}/a29219/jspro/xn.app.webpager.js',
	'${applicationScope.urlSmallStatic}/csspro/module/status-pop.css?ver=$revxxx$',
	'${applicationScope.urlSmallStatic}/csspro/module/school-selector.css?ver=$revxxx$',
	'${applicationScope.urlSmallStatic}/jspro/xn.ui.schoolSelectors.js?ver=$revxxx$',
	'${applicationScope.urlSmallStatic}/jspro/album/ZeroClipboard.js?ver=$revxxx$',
	'${applicationScope.urlSmallStatic}/jspro/music/music.for.feed.js?ver=$revxxx$',
	'${applicationScope.urlSmallStatic}/jspro/xn.ui.emoticons.js?ver=$revxxx$',
	'${applicationScope.urlSmallStatic}/csspro/module/minieditor.css?ver=$revxxx$',
	'${applicationScope.urlSmallStatic}/jspro/music/music.for.feed.js?ver=$revxxx$',
	'${applicationScope.urlSmallStatic}/apps/radio/css/radio-home.css?ver=$revxxx$',
	'${applicationScope.urlSmallStatic}/apps/radio/css/radio-home-v6.css?ver=$revxxx$',
	'${applicationScope.urlSmallStatic}/apps/radio/js/radio-home.js?ver=$revxxx$',
	'${applicationScope.urlSmallStatic}/n/apps/home/modules/drag-upload/drag-upload.js?ver=$revxxx$',
	'${applicationScope.urlStatic}/swf/album/ZeroClipboard.swf?ver=$revxxx$',
	'${applicationScope.urlSmallStatic}/apps/wiki/css/wiki-card-all-min.css?ver=$revxxx$',
	'${applicationScope.urlSmallStatic}/apps/wiki/js/wiki_model.js?ver=$revxxx$',
	'${applicationScope.urlSmallStatic}/apps/wiki/js/wikicard-load.js?ver=$revxxx$'
]);

XN.namespace( 'user' );
XN.user.id = '${visiter.id}';
XN.user.tinyPic = '${visiter.tinyFullUrl}';
XN.user.name = "<str:encodeUrl><c:out value='${visiter.name}'/></str:encodeUrl>";
XN.user.pageVersion = 5;
<c:if test="${!empty requestScope.visiter.userStateInfo.vipMember}">XN.user.isVip = ${requestScope.visiter.userStateInfo.vipMember};</c:if>
<c:if test="${!empty requestScope.visiter.userStateInfo.hideAds}">XN.user.hideAds = ${requestScope.visiter.userStateInfo.hideAds};</c:if>
</script>
<script type="text/javascript" data-src="http://s.xnimg.cn/modules/namecard/js/namecard.js?ver=$revxxx$" data-module="xn/namecard"></script>
<script type="text/javascript" data-src="http://s.xnimg.cn/n/core/modules/show-share-friend/showShareFriendsModule.js?ver=$revxxx$" data-module="xn/showShareFriend"></script>
<script type="text/javascript" data-src="http://s.xnimg.cn/n/core/modules/mention/index.js?ver=$revxxx$" data-module="xn/mentionMain"></script>
<script type="text/javascript" data-src="http://s.xnimg.cn/n/core/modules/ilike/index.js?ver=$revxxx$" data-module="xn/ilike"></script>
<script type="text/javascript" data-src="http://s.xnimg.cn/n/core/feed/group/group-vote.js?ver=$revxxx$" data-module="xn/group/vote"></script>
