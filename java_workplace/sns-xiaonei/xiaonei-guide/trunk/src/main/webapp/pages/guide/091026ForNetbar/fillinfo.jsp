<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html"%>
<%@ taglib uri="http://www.renren.com/tags-core" prefix="xiaonei"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
	<meta http-equiv="X-UA-Compatible" content="IE=EmulateIE7" />
	<meta name="Description" content="人人,人人网是一个真实的社交网络，联络你和你周围的朋友。加入人人,人人网你可以:联络朋友，了解他们的最新动态；和朋友分享相片、音乐和电影；找到老同学，结识新朋友；用照片和日志记录生活,展示自我。" />
	<meta name="Keywords" content="Xiaonei,人人,人人,大学,同学,同事,白领,个人主页,博客,相册,群组,社区,交友,聊天,音乐,视频,校园,人人,人人网" />
	<link href="http://s.xnimg.cn/csspro/base/layout.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
	<link href="${applicationScope.urlSmallStatic}/favicon-${domainIdentified["siteNameAbbr"]}.ico?ver=2" rel="shortcut icon" type="image/x-icon" />
	<!--[if lte IE 6]><style type="text/css" media="screen">/* <![CDATA[ */ @import url(http://s.xnimg.cn/csspro/patch/ie6.css?ver=$revxxx$); /* ]]> */</style><![endif]--><!--[if gte IE 7]><style type="text/css" media="screen">/* <![CDATA[ */ @import url(http://s.xnimg.cn/120444/patch/ie7.css?ver=$revxxx$); /* ]]> */</style><![endif]-->
	<link href="http://s.xnimg.cn/csspro/apps/reg-guide.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all">
	<script type="text/javascript" src="http://s.xnimg.cn/jspro/base.js?ver=$revxxx$"></script>
	
	<!-- 
	<script type="text/javascript" src="http://s.xnimg.cn/jspro/xn.form.validator.js?ver=$revxxx$"></script>
	 -->
	
<script type="text/javascript">
XN.getFileVersion(
[
'http://s.xnimg.cn/csspro/apps/profile.css?ver=$revxxx$',
'http://s.xnimg.cn/jspro/xn.app.recommendFriend.js?ver=$revxxx$',
'http://s.xnimg.cn/jspro/xn.ui.pager.js?ver=$revxxx$',
'http://s.xnimg.cn/jspro/xn.ui.multiFriendSelectorBox.js?ver=$revxxx$',
'http://s.xnimg.cn/csspro/module/friendSelector.css?ver=$revxxx$'
]);
</script>
	<title>${domainIdentified["siteName_fillinfo"]} - 寻找感兴趣的人</title>
</head>

<body class="cyberbar">

<script type="text/javascript">
	new Ajax.Request('/autr.do', {
	    method: 'post',
	    parameters: 'i=20301001',
	    onSuccess: function(transport){
	    }
	});
</script>

<div id="container">
	<%@ include file="inc/menu_withif.inc" %>
	<div id="opi" class="page-wrapper clearfix">
		<div class="full-page-holder">
			<div class="full-page clearfix">
				<div class="compatible">
					<div id="content">
						<div class="new-guide">
							<h3 class="welcome">欢迎来到${domainIdentified["siteName"]}网，
							<%
							String name = (String)request.getAttribute("user_name");
							if(name !=null){
								out.println(name);
							}
							else{
								out.println("我的朋友");
							}
							%></h3>
							

						<form id="netBarForm" action="/finbs.do" method="post">
							<div class="personal-info">
								<h4 style="font-size:14px;">5秒<span style="font-weight:normal">填写资料。找老乡、找同城好友！</span></h4>

								<p id="err_panel" class="show-error" style="display:none">
									你还没有选择你现在在哪
								</p>
								<p id="" class="show-error" style="display:none">
									你还没有选择你的家乡
								</p>
								<p id="" class="show-error" style="display:none">
									你还没有设置那些人可以浏览你的页面
								</p>
								<div class="pinfo-form">

										<p>
											<label>你现在在哪 : </label>
											<select id="work_cur" name="fi_currentprovince" class="select-short cur-plc">
												<%@ include file="inc/provinces.inc"%>
											</select>
                                            <input id="work_cur_city_name" type="hidden" name="fi_currentcityname" />
											<input id="work_cur_city_code" type="hidden" name="fi_currentcitycode" />
											<select id="work_cur_city" name="" class="select-short" style="display:none">
												<option value="-1" selected="selected">地区</option>
											</select>
										</p>
										
										<p>
											<label>你的家乡是 : </label>

											<select id="myhome" name="fi_homeprovince" class="select-short cur-home">
												<%@ include file="inc/provinces.inc"%>
											</select>
                                            <input id="myhome_city_name" type="hidden" name="fi_homecityname" />
											<input id="myhome_city_code" type="hidden" name="fi_homecitycode" />
											<select id="myhome_city" name="" class="select-short" style="display:none">
												<option value="-1" selected="selected">地区</option>
											</select>
										</p>
										
										<!-- 
										<p>
											<label>谁可以浏览我 : </label>
											<select id="whocan" name="fi_whocanseeme" class="select-long">

												<option value="-1" selected="selected">设置谁可以看到你的页面</option>
												<option value="1">只有我自己可见</option>
												<option value="2">只有我的好友可见</option>
												<option value="3">好友及同城、同学校、同公司的人可见</option>
												<option value="4">所有人可见</option>
											</select>

										</p>
										 -->
										
										<p class="form-action">
											<input id="goon" type="button" class="input-submit" value="保存并开始" />
										</p>
								</div>
							</div>
						</form>	
							


										
								</div>
							</div>
						</div>
					</div>
				</div>
			</div>
		</div>
	</div>
	<c:set var="hideFooterLink" value="true" />
    <c:set var="noWebPager" value="true" />
	<%@ include file="/inc/footer-noad.inc" %>
</div>






<!-- 好吧, 脚本就放在这里 -->
<!-- 组件 -->
<script type="text/javascript" src="http://s.xnimg.cn/jspro/xn.ui.schoolSelectors.js?ver=$revxxx$"></script>
<script type="text/javascript" src="http://s.xnimg.cn/jspro/xn.ui.currentPlace.js?ver=$revxxx$"></script>
<script type="text/javascript" src="http://s.xnimg.cn/jspro/xn.ui.pageTools.js?ver=$revxxx$"></script>

<!-- 特定于页面的代码 -->
<script type="text/javascript">
XN.namespace( 'page.netBar' );
XN.page.netBar.checkForm = function(){
	var emsg = {
		work_cur : '你还没有选择你现在在哪 ',
		work_cur_city_code : '你还没有选择你现在那个地区',
		myhome : '你还没有选择你的家乡 ',
		myhome_city_code : '你还没有选择你的家乡所在的地区',
		whocan : '请选择浏览权限'
	}

	var fields = [ 'work_cur' , 'work_cur_city_code' , 'myhome' , 'myhome_city_code' ];
	var epts =  XN.ui.pageTools.filter( fields , [ '' , '请填写小学' , '请填写大学' , '请填写高中', '请填写初中', '请填写中专/技校' , 0 , '0' , '-1' , -1 ] , false );
	
	if( epts.length ){
		$( 'err_panel' ).innerHTML = emsg[ epts[ 0 ].id  ];	
		$( 'err_panel' ).show();
		return false;
	}

	new Ajax.Request('/autr.do', {
         method: 'post',
         parameters: 'i=20301002',
         onSuccess: function(transport){
         }
    });	
	
	$( 'err_panel' ).hide();
	
	$( 'netBarForm' ).submit();
	
	return false;
	
}

XN.dom.ready(function(){

	
	XN.event.addEvent( 'goon' , 'click' , function( e ){
		XN.event.stop( e );
		XN.page.netBar.checkForm();
	});
	
	//var error_panel = new Shower( [ 'err_panel' ] );
});


</script>
<!-- 
<script type="text/javascript" src="http://s.xnimg.cn/jspro/guide/xn.page.guide.addStage.js?ver=$revxxx$"></script>

 -->

</body>
</html>