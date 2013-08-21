<%@ page pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>
<%@ taglib uri="http://jakarta.apache.org/taglibs/string-1.1" prefix="str" %>

<%--海外用户不显示广告--%>
<c:if test="${INC_NO_JEBE == true}">
	<c:set var="cleanpage" value="true" />
</c:if>

<c:set var="INC_NO_HEADER_BANNER" value="${INC_NO_HEADER_BANNER || requestScope.visiter.userStateInfo.hideAds == true}" />

<%-- make Struts XHTML-compliant --%>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<meta http-equiv="X-UA-Compatible" content="IE=EmulateIE7" />
<meta name="Description" content="${domainIdentified["siteDescription"]}" />
<meta name="Keywords" content="${domainIdentified["siteKeywords"]}" />
<!--[if IE]><script type="text/javascript" src="${applicationScope.urlSmallStatic}/n/core/expressions.js?ver=$revxxx$"></script><![endif]-->
<link href="${applicationScope.urlSmallStatic}/csspro/base/layout.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
<link href="${applicationScope.urlSmallStatic}/csspro/old/base.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
<link href="${applicationScope.urlSmallStatic}/favicon-${domainIdentified["siteNameAbbr"]}.ico?ver=2" rel="shortcut icon" type="image/x-icon" />
<link rel="apple-touch-icon" href="${applicationScope.urlStatic}/wap/apple_icon_.png" />

<script type="text/javascript">
XN = {env:{domain:'${applicationScope.domain}',shortSiteName:'${domainIdentified["siteName"]}',siteName:'${domainIdentified["siteName"]}网'}};
</script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/js/prototype.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/js/compact.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/a16285/jspro/base.js"></script>
<link href="${applicationScope.urlSmallStatic}/css/dialogpro.css?ver=$revxxx$" rel="stylesheet" type="text/css" />
<%-- 获取动态加载js的版本号 --%>
<script type="text/javascript">
XN.getFileVersion(
[
    '${applicationScope.urlSmallStatic}/csspro/apps/profile.css?ver=$revxxx$',
    '${applicationScope.urlSmallStatic}/jspro/xn.app.recommendFriend.js?ver=$revxxx$',
    '${applicationScope.urlSmallStatic}/jspro/xn.ui.pager.js?ver=$revxxx$',
    '${applicationScope.urlSmallStatic}/jspro/xn.ui.multiFriendSelectorBox.js?ver=$revxxx$',
    '${applicationScope.urlSmallStatic}/csspro/module/friendSelector.css?ver=$revxxx$',
	'${applicationScope.urlSmallStatic}/jspro/xn.app.status.js?ver=$revxxx$',
	'${applicationScope.urlSmallStatic}/jspro/xn.app.webpager.js?ver=$revxxx$',
	'${applicationScope.urlSmallStatic}/jspro/xn.app.ilike.js?ver=$revxxx$',
    '${applicationScope.urlSmallStatic}/jspro/xn.app.addFriend.js?ver=$revxxx$',
    '${applicationScope.urlSmallStatic}/jspro/album/ZeroClipboard.js?ver=$revxxx$',
    '${applicationScope.urlSmallStatic}/jspro/xn.ui.emoticons.js?ver=$revxxx$',
	'${applicationScope.urlSmallStatic}/csspro/module/minieditor.css?ver=$revxxx$',
    '${applicationScope.urlStatic}/swf/album/ZeroClipboard.swf?ver=$revxxx$'
]);
XN.dynamicLoad({
	file : 'http://s.xnimg.cn/jspro/xn.app.share.js?ver=$revxxx$',
	funcs : ['create_share_div','create_share_feed','create_share_edm','create_share_popup','create_share_jebe'] 
});

XN.namespace( 'user' );
XN.user.id = '${visiter.id}';
XN.user.tinyPic = '${visiter.tinyFullUrl}';
XN.user.name = "<str:encodeUrl><c:out value='${visiter.name}'/></str:encodeUrl>";
<c:if test="${!empty requestScope.visiter.userStateInfo.vipMember}">XN.user.isVip = ${requestScope.visiter.userStateInfo.vipMember};</c:if>
<c:if test="${!empty requestScope.visiter.userStateInfo.hideAds}">XN.user.hideAds = ${requestScope.visiter.userStateInfo.hideAds};</c:if>

<c:if test="${INC_NO_JEBE != true}">load_jebe_ads(1);</c:if><%-- load ad --%>
</script>

<c:if test="${visiter!=null}">
	<c:if test="${visiter.id%10==3 && !visiter.userStateInfo.guideDone && visiter.userStateInfo.socialGraphGood}">
		<script>XN.DOM.readyDo(function(){GuidBar.addBar();});</script>
	</c:if>
</c:if>
