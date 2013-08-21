<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
		<meta http-equiv="X-UA-Compatible" content="IE=EmulateIE7" />
		<meta name="Description" content="人人,人人网是一个真实的社交网络，联络你和你周围的朋友。加入人人,人人网你可以:联络朋友，了解他们的最新动态；和朋友分享相片、音乐和电影；找到老同学，结识新朋友；用照片和日志记录生活,展示自我。" />
		<meta name="Keywords" content="Xiaonei,人人,人人,大学,同学,同事,白领,个人主页,博客,相册,群组,社区,交友,聊天,音乐,视频,校园,人人,人人网" />
		<link href="http://s.xnimg.cn/csspro/base/layout.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
		<link href="${applicationScope.urlSmallStatic}/favicon-${domainIdentified["siteNameAbbr"]}.ico?ver=2" rel="shortcut icon" type="image/x-icon" />
		<!--[if lte IE 6]><style type="text/css" media="screen">/* <![CDATA[ */ @import url(http://s.xnimg.cn/csspro/patch/ie6.css?ver=$revxxx$); /* ]]> */</style><![endif]-->
		<!--[if gte IE 7]><style type="text/css" media="screen">/* <![CDATA[ */ @import url(http://s.xnimg.cn/csspro/patch/ie7.css?ver=$revxxx$); /* ]]> */</style><![endif]-->
		<link href="http://s.xnimg.cn/css/dialogpro.css?ver=$revxxx$" rel="stylesheet" type="text/css" />
		<link href="http://s.xnimg.cn/css/friends.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
		<link href="http://s.xnimg.cn/css/guide.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
		<link href="http://s.xnimg.cn/csspro/apps/reg-guide.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
		<script type="text/javascript" src="http://s.xnimg.cn/jspro/base.js?ver=$revxxx$" ></script>
		<script type="text/javascript" src="http://s.xnimg.cn/jspro/xn.ui.pager.js?ver=$revxxx$"></script>
		<script type="text/javascript" src="http://s.xnimg.cn/jspro/xn.ui.multiFriendSelectorBox.js?ver=$revxxx$"></script>
		<script type="text/javascript">
		XN.getFileVersion(
		[
	        'http://s.xnimg.cn/csspro/apps/profile.css?ver=$revxxx$',
	        'http://s.xnimg.cn/jspro/xn.app.recommendFriend.js?ver=$revxxx$',
	        'http://s.xnimg.cn/jspro/xn.ui.pager.js?ver=$revxxx$',
	        'http://s.xnimg.cn/jspro/xn.ui.multiFriendSelectorBox.js?ver=$revxxx$',
	        'http://s.xnimg.cn/csspro/module/friendSelector.css?ver=$revxxx$'
		]);

		 new Ajax.Request('/autr.do', {
	            method: 'post',
	            parameters: 'i=20102001',
	            onSuccess: function(transport){
	            }
	      });
		
		</script>
		<style type="text/css">
			.friendSelectorBox .selector{ height:362px; overflow:hidden; }
			.friendSelectorBox .selector ul li{ width:148px;}
		</style>
		<title>${domainIdentified["siteName_fillinfo"]} - 寻找朋友</title>

	</head>
	<body>
		<c:set var="hideLink" value="yes" />
		<%@ include file="inc/menu_withif.inc" %>
		<div id="opi" class="page-wrapper clearfix">
			<div class="full-page-holder">
				<div class="full-page clearfix">
					<div class="compatible">
						<div id="content">
						
							<div class="new-guide">
								<h3 class="welcome">欢迎来到${domainIdentified["siteName"]}网，<%=(String)request.getAttribute("user_name") %></h3>
								<%@ include file="inc/navbar_netbar_2preview.inc"  %>								
<div class="personal-info">
									<h4 id="h_1" class="frd-normal">点击加你感兴趣的人为好友</h4>
									<div id="fsb-holder">
									</div>
									<h4 class="frd-star">和这几位明星成为好友吧</h4>
									<form action="" id="pages">
									<input type="hidden" id="pr" name="pr" value="${info.province_name}" />
									<input type="hidden" id="ct" name="ct" value="${info.city_name}" />
									<input type="hidden" id="ri" name="ri" value="${info.region_id}" />
									<ul id="pages_panel" class="people-list">
									
										<c:forEach var="page" items="${pagelist}">
											<li class="item">
												<div class="avatar">
													<a href="#nogo" style="background-image:url(${page.tinyUrl})">
	
													</a>
												</div>
												<div class="context">
													<p class="name">
													<c:out value="${page.name}"/>
														<img src="${applicationScope.urlStatic}/imgpro/icons/follow.gif" title="通过人人网真实身份验证" alt="通过人人网真实身份验证"/>
													</p>
													<p class="action">
														<input type="checkbox" id="${page.id}" name="${page.id}" checked="true" />
	
														<label for="${page.id}">加为好友</label>
													</p>
												</div>
											</li>
										</c:forEach>
									</ul>	
									</form>
									
									
									
									
									<div class="confirm-friends clearfix">
										<span class="float-right">
											<input id="add_friends" type="button" class="input-submit" value="把选中的人加为好友" />
											<a id="skip_friends" href="/pvnbs.do?skip=1" class="jumpover">跳过</a>
										</span>
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
<input type="hidden" id="next_url" value="${next_url}" />
<input type="hidden" id="this_url" value="${this_url}" />
		<!-- 特定于页面的代码 -->
<script type="text/javascript">

(function(){

	var cnt = 0;
	function inccnt(){
		cnt ++;
		if(cnt>=3){
			window.location = "${next_url}";
		}
		
	}
	//var alt = XN.page.selectFriends.alt;
    var friends = new Array();

    
	//在页面上取两个数据next_url和this_url
	try{
		var next_url = $( 'next_url' ).value;
		var this_url = $( 'this_url' ).value;
	}catch( e ){}
	
	
	
	
    function showarray(){
    	var pages = [];
		var ps = $( 'pages_panel' ).getElementsByTagName( 'input' );
		XN.array.each( ps , function(i,v){
			if( v.type == 'checkbox' && v.checked ){
				pages.push( v.id );
			}
		});
        
        var pars = "friends=" + friends;
        if ( (friends == null ||( friends.length == 0)) && !pages.length ) {
            XN.DO.alert("<center>请选择您想加的好友</center>");
            //window.location = "${this_url}";
            return;
        }
        
        //这下面是一段山寨的JS……
        var pageparas = "pages=" + pages;
        new Ajax.Request('/addp.do', {
            method: 'post',
            parameters: pageparas,
            onSuccess: inccnt
        });

        
        new Ajax.Request('/requestFriend.do', {
            method: 'post',
            parameters: pars,
            onSuccess: inccnt
        });

		new Ajax.Request('/autr.do', {
            method: 'post',
            parameters: 'i=20102002',
            onSuccess: inccnt
	     });
    }
    function selector(id){
        var s = document.getElementById(id);
        var len = friends.length;
        if (s.className == "select") {
            s.className = "";
            for (var i = 0; i < friends.length; i++) {
                if (friends[i] == id) {
                    friends.remove(i);
                }
            }
        }
        else {
            s.className = "select";
            friends[len] = id;
        }
    }
    Array.prototype.remove = function(dx){
        if (isNaN(dx) || dx > this.length) {
            return false;
        }
        for (var i = 0, n = 0; i < this.length; i++) {
            if (this[i] != this[dx]) {
                this[n++] = this[i];
            }
        }
        this.length -= 1;
    };
    
  
    //-----------------  好友选择器初始化代码 ---------------------
    
    var selector = new XN.ui.multiFriendSelectorBox({
        url: 'http://guide.' + XN.env.domain + '/pvnb-ff.do',
        noSearchInput: true,
        noFilter: true,
        noCompleteButton: true,
        noCancelButton: true,
        maxNum: -1,//最多可选择的好友数
        friendsCountPerPage : 16,
		param : {
			pr : $( 'pr' ).value,
			ct : $( 'ct' ).value,
			ri : $( 'ri' ).value
		} 
    });
	

  
    $('fsb-holder').setContent(selector);

    
    selector.addEvent('select', function(id){
    });
    
    selector.addEvent('deselect', function(id){
    });
	
	
	
	selector.addEvent('load', function(id){
		if( !this._friendsCount ){
			//window.location = 'guide.do';	
			$( 'fsb-holder' ).hide();
			$( 'h_1' ).hide();
		}
    });
	

	//给加为好友添加click事件监听, 点了那按钮就能将选中的人加为好友了
	XN.event.addEvent( 'add_friends' , 'click' , function(){
		friends = selector.getSelectedFriends();
		showarray();
		return false;
	});
	
	
	XN.event.addEvent( 'skip_friends' , 'click' , function(){
		 new Ajax.Request('/autr.do', {
	            method: 'post',
	            parameters: 'i=20102008',
	            onSuccess: inccnt
	      });
	      return false;
	});
	
/*

	*/
	
	
})();



</script>
	</body>
</html>
