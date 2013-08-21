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
<link href="${applicationScope.urlSmallStatic}/csspro/module/friendSelector.css?ver=$revxxx$" media="all" type="text/css" rel="stylesheet" />

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
<script type="text/javascript" src="http://xnimg.cn/jspro/xn.ui.multiFriendSelectorBox.js"></script>
<script type="text/javascript" src="http://xnimg.cn/jspro/xn.ui.friendSelecotorMenu.js"></script>

<style type="text/css">
.share-ad {
background: no-repeat scroll 0 top transparent;
padding:0 0 28px;
margin:1 auto;
}
</style>
<title>${domainIdentified["siteName_fillinfo"]} - 搜索结果</title>
</head>
<body>
<!-- <%@ include file="/inc/header.inc"%> -->
<div class="share-ad">
    <a class="logo" title="人人网 renren.com - 人人网校内是一个真实的社交网络，联系朋友，一起玩游戏" href="http://www.renren.com"></a>
    <div class="screen search-main-new">
        <div class="procedure first">搜索好友</div>
        <div id="friendSelector-container" class="friendSelector-box friendSelector-main"> </div>
        <div class="list">
            <ul id="search-list">

                <li class="open">
                    <h2>填写任意信息查找好友</h2>
                    <span>调用请求：http://browse.renren.com/searchgd?id=-948328&s=0&ajax=1&q=&p=[{"t":"high","name":"绵阳中学","year":""}]</span><br />
                    <span>参数：ajax=1&s=0</span><br />
                    <span>参数：p=[{"t":"high","name":"绵阳中学","year":""}]</span><br />
                    <span><font color="red" >p的不同取值有：</font><br />大学[{"t":"univ","name":"清华大学","id":"1001","depa":"历史系","year":"2010"}]</span><br />
                    <span>高中[{"t":"high","name":"绵阳中学","year":""}]</span><br /><!-- "clazz1":"11","clazz2":"11","clazz3":"11", -->
                    <span>中专[{"t":"sect","name":"东城职业技术学校","year":"2009"}]</span><br />
                    <span>初中[{"t":"juni","name":"北京市一一九中学","year":"2009"}]</span><br />
                    <span>小学[{"t":"elem","name":"我是一个小学","year":"2009"}]</span><br />
                    <span>公司[{"t":"work","name":"千橡互动"}]</span><br />
                    <span>家乡[{"t":"base","prov":"四川","city":"绵阳市"}]</span><br />
                    <span>所在地[{"t":"regi","prov":"四川","city":"绵阳市"}]</span><br /> <!-- "id":"60015003", -->
                    <form action="http://browse.renren.com/searchgd" >
                    <input type="hidden" name="s" value="0" />
                    <input type="hidden" name="ajax" value="1" />
                    <script>
                    function domytest2(){
                    	param = encodeURIComponent($("p").value);
               	         var selector = new XN.ui.multiFriendSelectorBox({
               	                  url : 'http://browse.renren.com/searchgd',
               	                  maxNum : -1,//最多可选择的好友数
               	               	  noAjaxData:true,
               	                  provideData:null
               	              });
               	              
               	              $( 'friendSelector-container' ).setContent( selector );
                    }
                    function doMySuccess(r){
                    	data = r.responseText;
                    	data = XN.string.trim(data);
                		if(!XN.string.isJSON(data)){
                			data = '{"candidate":[]}';
                		}
                		var cdata = XN.json.parse(data);
                		var ret = "";
                		for(i = 0;i< cdata.candidate.length;i++){
                    		ret = ret + (i+":"+cdata.candidate[i].id+" - "+cdata.candidate[i].name+" "+cdata.candidate[i].net+" <br />");
                		}
                        $('ret').innerHTML = ret;
                    }
                    function domytest(){
                    	//var ee = encodeURIComponent($("p").value);
                    	//$("p").value = ee;
                    	//var recommandFriends =  XN.json.parse('${requestScope.lead_may_known}');
                    	param = encodeURIComponent($("p").value);
                    	uparam = encodeURIComponent($("u").value);
                    	new XN.NET.xmlhttp({
                    		url:'http://browse.' + XN.env.domain + '/searchgd',
                    		data:'s=0&'+'ajax='+1+'&p='+param+'&u='+uparam,
                    		method:'get',
                    		asynchronous:true,
                    		onSuccess:function(r){
                    			doSuccess(r);
                    			doMySuccess(r);
                    			/*
	                    		data = r.responseText;
	                    		if(!XN.string.isJSON(data)){
	                    			data = '{"candidate":[]}';
	                    		}
	                    		data = XN.string.trim(data);
	                    		var search_restult = {};
	                    		try {
	                    			search_restult = XN.json.parse(data);
	                    		}
	                    		catch(err){
	                    		} 
	                    		//alert(search_restult.candidate[0].id);
	                  	        var selector = new XN.ui.multiFriendSelectorBox({
	                  	                  maxNum : -1,//最多可选择的好友数
	                  	               	  noAjaxData:true,
	                  	                  provideData:search_restult
                 	            });
                 	            alert(search_restult.candidate[0].id);
                 	            $( 'friendSelector-container' ).setContent( selector );
                 	            */
	                    		                    		
                    		},
                    		onError:function(r){
                    			//doSuccess(r);
                    		}
                    	}); 
                    }
                    </script>
                    <input size="100" id="p" type="text" name="p" value="" /><input size="100" id="u" type="text" name="u" value="" />
                    <input type="button" value="parse" onclick="domytest()"/>
                    <input type="submit" />
                    </form>
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

    <div class="footer-bg"></div>
</div><!--share-ad end-->
    <div id="ret" style="float:left">
    
    </div>
    <input type="hidden" name="friend_ids" id="friend_ids"/>
</body>
</html>

