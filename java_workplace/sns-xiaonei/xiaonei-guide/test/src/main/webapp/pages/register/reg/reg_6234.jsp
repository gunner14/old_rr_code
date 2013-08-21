<%@ page contentType="text/html;charset=UTF-8" session="false" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<meta http-equiv="X-UA-Compatible" content="IE=EmulateIE7" />
<meta name="Description" content="人人网校内是一个真实的社交网络，联络你和你周围的朋友。加入人人网校内你可以:联络朋友，了解他们的最新动态；和朋友分享相片、音乐和电影；找到老同学，结识新朋友；用照片和日志记录生活,展示自我。" />
<meta name="Keywords" content="Xiaonei,Renren,校内,大学,同学,同事,白领,个人主页,博客,相册,群组,社区,交友,聊天,音乐,视频,校园,人人,人人网" />
<link href="http://s.xnimg.cn/csspro/base/layout.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
<link href="http://s.xnimg.cn/favicon-rr.ico?ver=2" rel="shortcut icon" type="image/x-icon" />
<link href="http://s.xnimg.cn/renren_logo_apple.png" rel="apple-touch-icon" />
<!--[if IE]><script type="text/javascript" src="http://s.xnimg.cn/n/core/expressions.js?ver=$revxxx$"></script><![endif]-->
<script type="text/javascript">
XN = {env:{domain:'renren.com',shortSiteName:'人人',siteName:'人人网'}};
</script>
<script type="text/javascript" src="http://s.xnimg.cn/js/prototype.js?ver=$revxxx$"></script>
<script type="text/javascript" src="http://s.xnimg.cn/js/compact.js?ver=$revxxx$"></script>

<script type="text/javascript" src="http://s.xnimg.cn/jspro/base.js?ver=$revxxx$"></script>
<link href="http://s.xnimg.cn/css/dialogpro.css?ver=$revxxx$" rel="stylesheet" type="text/css" />

<script type="text/javascript">
XN.getFileVersion(
[
    'http://s.xnimg.cn/csspro/apps/profile.css?ver=$revxxx$',
    'http://s.xnimg.cn/jspro/xn.app.recommendFriend.js?ver=$revxxx$',
    'http://s.xnimg.cn/jspro/xn.ui.pager.js?ver=$revxxx$',
    'http://s.xnimg.cn/jspro/xn.ui.multiFriendSelectorBox.js?ver=$revxxx$',
    'http://s.xnimg.cn/csspro/module/friendSelector.css?ver=$revxxx$',
    'http://s.xnimg.cn/jspro/xn.app.status.js?ver=$revxxx$',
    'http://s.xnimg.cn/jspro/xn.app.addFriend.js?ver=$revxxx$'
]);
XN.dynamicLoad({
    file : 'http://s.xnimg.cn/jspro/xn.app.share.js?ver=$revxxx$',
    funcs : ['create_share_div','create_share_feed','create_share_edm','create_share_popup'] 
});

XN.namespace( 'user' );
XN.user.id = '230961237';
XN.user.tinyPic = 'http://hdn.xnimg.cn/photos/hdn321/20100727/0140/tiny_b5QC_104730o019118.jpg';
XN.user.name = "%E8%B5%B5%E9%98%B3%E2%98%86%E2%99%82%E2%98%86";

load_jebe_ads(1);
</script>
<!--[if gt IE 7]><!--><link type="text/css" rel="stylesheet" href="http://s.xnimg.cn/n/core/webpager-std-min.css?ver=$revxxx$" /><!--<![endif]-->
<!--[if IE 6]><link type="text/css" rel="stylesheet" href="http://s.xnimg.cn/n/core/webpager-ie6-min.css?ver=$revxxx$" /><style id="CSSID_9f2766a4"></style><![endif]-->
<!--[if IE 7]><link type="text/css" rel="stylesheet" href="http://s.xnimg.cn/n/core/webpager-ie7-min.css?ver=$revxxx$" /><![endif]-->
<script type="text/javascript" src="http://s.xnimg.cn/jspro/xn.app.webpager.js?ver=$revxxx$"></script>
<!--[if gt IE 6]><script>if(window.ActiveXObject&&!window.XMLHttpRequest)XN.webpager=null</script><![endif]-->
<link href="http://s.xnimg.cn/actimg/adSearch/search_ad.css?ver=$revxxx$" rel="stylesheet" type="text/css" />
<link href="http://s.xnimg.cn/csspro/module/school-selector.css?ver=$revxxx$" rel="stylesheet" type="text/css" />
<script src="http://s.xnimg.cn/jspro/xn.ui.schoolSelectors.js?ver=$revxxx$" type="text/javascript"></script>
<script type="text/javascript" src="http://s.xnimg.cn/js/register/register-simple.js?ver=$revxxx$"></script>
<script src="http://s.xnimg.cn/actimg/adSearch/search.js?ver=$revxxx$" type="text/javascript"></script>
<title>${domainIdentified["siteName_fillinfo"]} - 搜索结果</title>
</head>
<body>
<div class="share-ad">
    <a class="logo" title="人人网 renren.com - 人人网校内是一个真实的社交网络，联系朋友，一起玩游戏" href="http://www.renren.com"></a>
    <div class="screen search-main-new">
        <div class="procedure first">搜索好友</div>
        <div class="list">
            <ul id="search-list">

                <li class="open">
                    <h2>填写任意信息查找好友</h2>
                    <div class="group showGroup">
                        <p id="classMatesType">
                            <a class="active" href="javascript:;" id="searchUnivTab" onclick="changeTab(this);">大学同学</a>
                            |
                            <a href="javascript:;" id="searchHighTab" onclick="changeTab(this);">高中同学</a> |
                            <a href="javascript:;" id="searchTechTab" onclick="changeTab(this);">中专技校</a> |
                            <a href="javascript:;" id="searchJuniorTab" onclick="changeTab(this);">初中同学</a> |
                            <a href="javascript:;" id="searchPrimaryTab" onclick="changeTab(this);">小学同学</a>

                        </p>
                        <div class="choose-box clearfix">
                            <fieldset id="searchUnivView">
                                <form id="searchUnivForm" class="jsonForm" action="" method="post" autocomplete="off">
                                    <table>
                                        <tbody>
                                          <tr>
                                              <td class="name">
                                                  姓 名 :
                                              </td>
                                              <td>
                                                  <input type="text" value="输入姓名" class="input-text" id="q1" maxlength="12"/>
                                              </td>
                                          </tr>
                                        
                                            <tr>
                                                <td class="name">

                                                    学 校 :
                                                </td>

                                                <td>
                                                    <input type="text" class="input-text s-select s-univ" name="univ" id="univ" value="选择学校"/>
                                                    <input type="hidden" name="univId" id="univ_id" />
                                                    <span id="univ_dept_warper" style="display:inline-block;">
                                                        <select class="select" name="department" id="univ_dept">
                                                            <option value="">
                                                                院系
                                                            </option>

                                                        </select>
                                                    </span>
                                                    <select class="select" name="year" id="universityYear">
                                                        <%@ include file="inc/all_year_options.inc" %>

                                                    </select>
                                                    <input type="button" value="搜索" onclick="search(1);" class="input-submit" />

                                                </td>
                                            </tr>

                                        </tbody>
                                    </table>
                                    <div class="submit-btn">
                                        <input type="hidden" name="ajax" value="0">
                                        <input type="hidden" name="s" value="0">
                                        <input type="hidden" name="p" value="">
                                    </div>

                                <input type="hidden" name="requestToken" value="-1622959582"></form>

                            </fieldset>
                            <fieldset id="searchHighView" class="none">
                                <form id="searchHighForm" class="jsonForm" action="" method="post" autocomplete="off">
                                    <table>
                                        <tbody>
                                            <tr>
                                                <td class="name">
                                                    姓 名 :
                                                </td>

                                                <td>
                                                    <input type="text" value="输入姓名" class="input-text" name="q" maxlength="12" id="q2">
                                                </td>
                                            </tr>
                                            <tr>
                                                <td class="name">
                                                    高 中 :
                                                </td>
                                                <td>

                                                    <input type="text" class="input-text s-select s-hight" value="选择学校" id="high" />
                                                    <select class="select" name="year" id="highSchoolYear">
                                                        <%@ include file="inc/all_year_options.inc" %>
                                                    </select>
                                                    <input type="button" value="搜索" class="input-submit" onclick="search(2);" />
                                                </td>
                                            </tr>
                                        </tbody>

                                    </table>
                                    <div class="submit-btn">
                                        <input type="hidden" name="ajax" value="0">
                                        <input type="hidden" name="s" value="0">

                                        <input type="hidden" name="p" value="">
                                    </div>
                                <input type="hidden" name="requestToken" value="-1622959582"></form>
                            </fieldset>

                            <fieldset id="searchTechView" class="none">
                                <form id="searchTechForm" class="jsonForm" action="" method="post" autocomplete="off">
                                    <table>
                                        <tbody>
                                            <tr>

                                                <td class="name">
                                                    姓 名 :
                                                </td>
                                                <td>

                                                    <input type="text" value="输入姓名" class="input-text" name="q" maxlength="12" id="q3">
                                                </td>
                                            </tr>
                                            <tr>
                                                <td class="name">

                                                    中专技校 :
                                                </td>
                                                <td>
                                                
                                                    <input type="text" value="选择学校" class="input-text s-select s-tech" id="sect"> 
                                                    <select class="select" name="year" id="sectyear">
                                                         <%@ include file="inc/all_year_options.inc" %>
                                                    </select>
                                                    <input type="button" value="搜索" class="input-submit" onclick="search(3);"/>
                                                </td>
                                            </tr>
                                        </tbody>
                                    </table>
                                    <div class="submit-btn">

                                        <input type="hidden" name="ajax" value="0">
                                        <input type="hidden" name="s" value="0">
                                        <input type="hidden" name="p" value="">
                                    </div>
                                <input type="hidden" name="requestToken" value="-1622959582"></form>
                            </fieldset>
                            <fieldset id="searchJuniorView" class="none">
                                <form id="searchJuniorForm" class="jsonForm" action="" method="post" autocomplete="off">

                                    <table>

                                        <tbody>
                                            <tr>
                                                <td class="name">
                                                    姓 名 :
                                                </td>
                                                <td>
                                                    <input type="text" value="输入姓名" class="input-text" maxlength="12" name="q"  id="q4">
                                                </td>

                                            </tr>

                                            <tr>
                                                <td class="name">
                                                    初 中 :
                                                </td>
                                                <td>
                                                    <input type="text" value="选择学校" class="input-text s-select s-junior" id="juni">
                                                    <select class="select" name="year" id="juniorSchoolYear">
                                                         <%@ include file="inc/all_year_options.inc" %>
                                                    </select>
                                                    <input type="button" value="搜索" class="input-submit" onclick="search(4);"/>
                                                </td>
                                            </tr>

                                        </tbody>
                                    </table>
                                    <div class="submit-btn">
                                        <input type="hidden" name="ajax" value="0">

                                        <input type="hidden" name="s" value="0">
                                        <input type="hidden" name="p" value="">
                                    </div>
                                <input type="hidden" name="requestToken" value="-1622959582"></form>
                            </fieldset>

                            <fieldset id="searchPrimaryView" class="none">
                                <form id="searchPrimaryForm" class="jsonForm" action="" method="post" autocomplete="off">
                                    <table>

                                        <tbody>
                                            <tr>
                                                <td class="name">
                                                    姓 名 :
                                                </td>
                                                <td>

                                                    <input type="text" value="输入姓名" class="input-text" maxlength="12" name="q"  id="q5">
                                                </td>
                                            </tr>

                                            <tr>
                                                <td class="name">
                                                    小 学 :
                                                </td>
                                                <td>
                                                    <input type="text" value="输入学校" class="input-text" id="primarySchool" autocomplete="off" />

                                                    <select class="select" name="year" id="primarySchoolYear">

                                                         <%@ include file="inc/all_year_options.inc" %>

                                                    </select>

                                                    <input type="button" value="搜索" class="input-submit" onclick="search(5);"/>

                                                </td>
                                            </tr>
                                        </tbody>
                                    </table>
                                    <div class="submit-btn">
                                        <input type="hidden" name="ajax" value="0">
                                        <input type="hidden" name="s" value="0">
                                        <input type="hidden" name="p" value="">

                                    </div>
                                <input type="hidden" name="requestToken" value="-1622959582"></form>
                            </fieldset>
                        </div>
                    </div>
                    
                </li>
            </ul>
        </div>
    </div><!--screen end-->

    <div class="share-ad-main">
    <div class="procedure second">选择好友<span>(为你找到<em id="result_sum">0</em>个结果，选出你认识的人)</span></div>
        <div class="friend-selector" id="search_result">
            <ul id="myknowfriend_con" class="clearfix"></ul>
        </div><!--friend-selector end-->
        <div style="display:none" class="no-content" id="noff">
		   <p>对不起，没有搜索到您要寻找的好友！<br/>*建议您检查学校、年份输入是否正确</p>
		</div>
        <div id="loadingf"></div>     
        <div class="more">
            <a href="javascript:;" id="showmore" onclick="showMore();">查看更多</a>
        </div><!-- end-->

    </div><!--share-ad-main end-->
    <div class="regist-box">
        <div class="regist-ad">
            <div class="procedure third">添加好友<span class="login">已经有人人网帐号？<a href="#">请登录</a></span></div>
            <a href="javascript:;" class="regist-btn" onclick="popReg();"></a>
        </div>
    </div><!--regist-box end-->

    <div class="footer-bg"></div>
</div><!--share-ad end-->
         <div class="popReg">
                   <div class="popWin">               
                            <div style="position:absolute;top:0;left:0;bottom:0;right:0;z-index:100;width:546px;height:467px;margin:auto;background:url(http://a.xnimg.cn/actimg/adSearch/img/pop.gif) no-repeat left top;">
                                     <div class="head">
                                               <h4>加入人人网&nbsp;&nbsp;找回老朋友&nbsp;&nbsp;结交新朋友</h4>
                                               <a href="javascript:;" class="close" onclick="closePop();">关闭</a>
                                     </div>
                                     <div class="form">
<form method="post" id="regform"  name="regform" action="http://reg.renren.com/s-c-i-reg.do">
                   <input type="hidden" id='msgId' value=''/>
                   <input type="hidden" name="fromUrl" id="fromUrl" />
                   <input type="hidden" name="iu" id="iu" />
                   <input type="hidden" name="ic" id="ic" />
                   
                   <input type="hidden" name="uuid" id="uuid" />
                   
                   <input type="hidden" name="ss" id="ss" value="null"  />
                   <input type="hidden" name="action_id" id="action_id" value="206234" /> <input type="hidden" name="pageId" id="pageId" value="" />
                   <input type="hidden" name="accType" id="accType" value="" />
                   <input type="hidden" name="g" id="g" value="null"/>
                   <input type="hidden" name="b" id="b" />
                   <input type="hidden" name="friend_ids" id="friend_ids"/>
                   <dl id="d_email">
                   <dt><label for="regEmail">电子邮箱:</label></dt>
                   <dd><input type="text" name="regEmail" id="regEmail" class="inputtext" /><span id="dmmg"></span><span id="regEmail_error_span" class="box-error"><b id="regEmail_error_span_i"></b></span><p class="tabsort">你还可以选择 <a href="javascript:;" id="xid_reg_handle" tabindex="-1">帐号</a> 注册</p></dd>
                   </dl>
                   
                   <dl id="d_xid" style="display:none">
                   <dt><label for="nicknameId">输入帐号:</label></dt>
                   <dd><input type="text" name="nicknameId" id="nicknameId" class="inputtext" /><span id="mg"></span><span id="nicknameId_error_span" class="box-error"><b id="nicknameId_error_span_i"></b></span><p class="tabsort"> <a href="javascript:;" id="regmail_reg_handle" tabindex="-1">返回邮箱</a> 注册</p></dd>
                   </dl>
                   <dl>
                   <dt><label for="pwd">设置密码:</label></dt>
                   <dd><input type="password" maxlength="20" name="pwd" id="pwd" class="inputtext" /><span id="mmg"></span><span id="pwd_error_span" class="box-error"><b id="pwd_error_span_i"></b></span></dd>
                   </dl>
                   <dl class="dl_pad">
                   <dt><label for="name">真实姓名:</label></dt>
                   
                   <dd><input type="text" name="name" id="name" class="inputtext" /><span id="xmg"></span><span id="name_error_span" class="box-error"><b id="name_error_span_i"></b></span></dd>
                   </dl>
                   <dl class="dl_gender">
                   <dt><label>性别:</label></dt>
                   <dd><label for="male"><input type="radio" name="gender" id="male" value="男生" />男</label><label for="female"><input type="radio" name="gender" id="female" value="女生" />女</label><span id="gender_error_span" class="box-error"><b id="gender_error_span_i"></b></span></dd>
                   </dl>
                   <dl id="p_birthday">
                   <dt><label>生日:</label></dt>
                   <dd style="padding-top:6px;">
                   <%@ include file="inc/all_year_options_reg.inc" %>
                   <span id="birthday_error_span" class="box-error"><b id="birthday_error_span_i"></b></span>
                   <p><a href="javascript:;" id="birtips" tabindex="-1">为什么要填写我的生日？</a></p>
                   </dd>
                   </dl>
                   <dl>
                   <dt>&nbsp;</dt>
                   <dd><img id="verifyPic" src="http://icode.renren.com/getcode.do?t=register_1279120859&rnd=1283942865311" alt=""/><a href="javascript:refreshCode();" tabindex="-1">看不清换一张?</a></dd>
                   </dl>
                   <dl>
                   <dt><label for="code">验证码:</label></dt>
                   <dd><input type="text" name="icode" id="icode" class="inputtext validate-code" size="5" maxlength="5" /><input type="hidden" name="key_id" value=1279120859 /><span id="icode_error_span" class="box-error"><b id="icode_error_span_i"></b></span></dd>
                   
                   </dl>
                   <dl>
                   <dt>&nbsp;<input type="hidden" name="activity" value="" id="activity" /><input type="hidden" name="iv" value="" id="iv" /><input type="hidden" name="from" value="" id="from" /></dt>            
        </dl>
         <input type="submit" id="button_submit" class="inputsubmit" value="" />

                   <script type="text/javascript">
                            new RegCheck();
                   </script>
</form>
                                                        </div>
                            </div>
                            <iframe width="526" height="427" frameborder="0" style="border:0px none;position:absolute;top:20px;left:10px;z-index:-1;"></iframe>
                   </div>
         </div>

    <input type="hidden" name="friend_ids" id="friend_ids"/>
</body>
</html>

