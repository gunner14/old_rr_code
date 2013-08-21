<%@ page pageEncoding="utf-8" %>
<%@ page import="com.dodoyo.friends.utils.LogTimeUtils"%>
<%@ page import="com.xiaonei.platform.core.cache.MemCacheManager"%>
<%@ page import="com.xiaonei.platform.core.cache.MemCacheKeys"%>
<link href="${applicationScope.urlSmallStatic}/csspro/apps/home-all.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
<link href="${applicationScope.urlSmallStatic}/csspro/apps/guide.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
<link href="${applicationScope.urlSmallStatic}/csspro/module/news-feeds.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
<link href="${applicationScope.urlSmallStatic}/css/dialogpro.css?ver=$revxxx$" rel="stylesheet" type="text/css" />
<link href="${applicationScope.urlSmallStatic}/css/search-new.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
<link href="${applicationScope.urlSmallStatic}/css/xsuggest.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all">
<link href="${applicationScope.urlSmallStatic}/csspro/apps/searchpro.css" rel="stylesheet" type="text/css" media="all" />
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/xn.app.feed.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/xn.app.share.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/xn.app.status.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/xn.page.home.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/swfupload.v2.2.0.1/swfupload.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/js/provinceCity.js?ver=$revxxx$""></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/js/xn.app.guidebar.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/js/search2.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/js/cityArray.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/js/searchclassfangle.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/js/yui/yahoo-dom-event/yahoo-dom-event.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/js/yui/connection/connection-min.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/js/yui/autocomplete/autocomplete-min.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/js/schoolComponent.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/xn.app.search.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/xn.app.ilike.js?ver=$revxxx$"></script>
<%@ include	file="/views/guide/common/top/guide_common_css_js.jsp"%>
<c:set var="poped" value="yes" />
<%@ include	file="/pages/guide/common/top/guide_ready_do.jsp"%>






