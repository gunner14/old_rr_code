<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html"%>
<%@ taglib uri="http://www.renren.com/tags-core" prefix="xiaonei"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<!-- _rcbonus_ -->
<head>
    <c:set var="INC_NO_JEBE" value="true" />
    <%@ include file="/inc/headpro.inc"%>
    <title>${domainIdentified["siteName_fillinfo"]} - 免费领养最贵的狗狗！</title>
    <link href="${applicationScope.urlSmallStatic}/actimg/KX_Dog/css/style.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
    <script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/guide/xn.guide.gougou.js?ver=$revxxx$"></script>
</head>
<body id="home" class="guideActivit">
<c:set var="cleanpage" value="ture" />
<%@ include file="/inc/header.inc"%>
<div id="content">
    <div class="activit-content clearfix">
        <div class="activit-header">
            <img src="${applicationScope.urlSmallStatic}/actimg/KX_Dog/images/1.png" width="958" height="136"/><img src="${applicationScope.urlSmallStatic}/actimg/KX_Dog/images/2.png" width="958" height="137"/><p><a href="http://apps.kaixin.com/kxdog_app?ref=kaixin_dogbonus_button" class="adopt" title="马上领养"></a>
                <a href="#" class="invite" onclick="showFriendSelect();return false;" title="邀请好友"></a>
            </p>
        </div>
        <div class="activit-footer">
            <h1>活动规则 ：</h1>
            <p>1.从即日起，所有此前已经安装过狗狗的开心网用户，第一次登陆狗狗将获得2000骨头。</p>
            <p>2.从即日起，每天登陆狗狗都将获得500骨头，每天只可获得一次。</p>
            <p>3.成功邀请一个好友，获得100骨头，每天每个好友只能邀请一次，邀请好友越多，可能获得的骨头越多。</p>
            <p>4.请保持对本活动页面的关注，为倾情回馈广大开心网用户对开心网的支持，更多赠送活动将持续推出。</p>

            <p>5.本活动最终解释权归开心网所有。</p>
        </div>
    </div><!--activit-content end-->
</div><!--content end-->

<%@ include file="/inc/footer-guide-flow.inc" %>
</body>
</html>
