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
    <link href="${applicationScope.urlSmallStatic}/actimg/guide2010/css/style.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
	<script src="${applicationScope.urlSmallStatic}/jspro/guide/xn.guide.getRcbonus.js?ver=$revxxx$" type="text/javascript"></script>    
    <title>${domainIdentified["siteName_fillinfo"]} - 100%有奖，我为人人狂！</title>
</head>
<body id="home" class="guideActivit">
<c:set var="cleanpage" value="ture" />
<%@ include file="/inc/header.inc"%>
<div id="content">
    <div class="activit-content clearfix">
        <div class="activit-header">
            <span><em id="now_rc">${requestScope.rc_user_current}</em> 个</span>
            <a href="#nogo" onclick="showFriendSelect();return false;" class="btn" title="赢取更多奖豆"></a>
        </div>
        <div class="activit-home clearfix">

            <ul>
                <li>
                    <img src="${applicationScope.urlStatic}/actimg/guide2010/images/mac.jpg"/>
                    <strong>Mac 笔记本一台</strong>
                    <span>活动已结束</span>

                </li>
                <li>
                    <img src="${applicationScope.urlStatic}/actimg/guide2010/images/iphone.jpg"/>
                    <strong>iPhone 一台</strong>
                    <span>活动已结束</span>

                </li>
                <li>
                    <img src="${applicationScope.urlStatic}/actimg/guide2010/images/m10.png"/>
                    <strong>移动话费 10 元</strong>
                    <span>活动已结束</span>

                </li>
                
                <li>
                    <img src="${applicationScope.urlStatic}/actimg/guide2010/images/vip-5years.jpg"/>
                    <strong>免费 VIP 5年</strong>
                    <span>活动已结束</span>

                </li>
                <li>
                    <img src="${applicationScope.urlStatic}/actimg/guide2010/images/vip-30month.jpg"/>
                    <strong>3 个月 VIP 体验</strong>
                    <span>活动已结束</span>

                </li>
                <li>
                    <img src="${applicationScope.urlStatic}/actimg/guide2010/images/vip-30day.jpg"/>
                    <strong>30 天 VIP 体验</strong>
                    <span>活动已结束</span>

                </li>
                <li>
                    <img src="${applicationScope.urlStatic}/actimg/guide2010/images/vip-7day.jpg"/>
                    <strong>7 天 VIP 体验</strong>
                    <span>活动已结束</span>

                </li>
                <li>
                    <img src="${applicationScope.urlStatic}/actimg/guide2010/images/gift10.jpg"/>
                    <strong>礼券 10 个</strong>
                    <span>活动已结束</span>

                </li>
                <li>
                    <img src="${applicationScope.urlStatic}/actimg/guide2010/images/gift5.jpg"/>
                    <strong>礼券 5 个</strong>
                    <span>活动已结束</span>

                </li>
                <li>
                    <img src="${applicationScope.urlStatic}/actimg/guide2010/images/gift2.jpg"/>
                    <strong>礼券 2 个</strong>
					<span><em>已被抢兑一空</em></span>
					<span><br />敬请期待后续奖品</span>
                </li>
                <li>
                    <img src="${applicationScope.urlStatic}/actimg/guide2010/images/m30.png"/>
                    <span>本次发放已结束 </span>
                    <span><br />敬请期待后续奖品</span>

                </li>
                <li class="desc">
                    <p>
                        1.中奖概率：每次抽取均有<font color=red><b>50%</b></font>概率得中，每人抽奖当天有一次抽取机会。<br />
                        2.抽奖条件：3月27日当天登录即可抽取且无需花费抽奖豆。<br />
                        3.抽奖时间：3月27日全天，每小时放出100张，先到先得，抽完即止。  <br />
                        4.最终解释权归人人网所有。
                    </p>

                </li>
                
            </ul>
        </div><!--activit-home end-->
        <div class="activit-sidebar clearfix">
		<script type="text/javascript">
		try{
		var rc_user_awarded = ${requestScope.rc_user_awarded }
		}catch(e){
		var rc_user_awarded = {};
		}

		</script>
            <ul id="awardsUserList">
            </ul>
        </div><!--activit-sidebar end-->
        <div class="activit-footer">
            <p>1.从3月26日起每天登录获得150抽奖豆，每天只奖励一次。</p>
            <p>2.成功邀请一个好友来抽奖活动，获得抽奖豆3个。</p>
            <p>3.获奖之后我们会在两个工作日内赠送奖品或联系获奖人，移动话费我们将在五个工作日内赠送奖品或联系获奖人。</p>

            <p>4.请你保持对本活动页面的关注，为倾情回馈广大人人网用户对人人网的支持，更多赠送活动将持续推出。</p>
            <p>5.本活动截止到2010年3月31日，活动的最终解释权归人人网所有。</p>
        </div>
    </div><!--activit-content end-->
</div><!--content end-->
<%@ include file="/inc/footer-guide-flow.inc" %>
</body>
</html>
