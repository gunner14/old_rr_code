<%@ page contentType="text/html;charset=UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<%@page import="com.xiaonei.reg.guide.root.interfaces.IGuideUserLeadType"%><html xmlns="http://www.w3.org/1999/xhtml">

<head><!-- _pv_darkcorn_ -->
	<%@ include file="/inc/head-guide-flow.inc" %>
	<link href="${applicationScope.urlSmallStatic}/csspro/module/school-selector.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
    <link href="${applicationScope.urlSmallStatic}/csspro/module/friendSelector.css?ver=$revxxx$" media="all" type="text/css" rel="stylesheet" /> 
	<link href="${applicationScope.urlSmallStatic}/csspro/apps/guide-register2.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all">
	<title>${domainIdentified["siteName_fillinfo"]} - 选择你认识的人加为好友</title>

</head>

<body id="home" class="guide">
		<%@ include file="/inc/header-guide-flow.inc" %>
        <script type="text/javascript">
	    	var recommandFriends =  XN.json.parse('${requestScope.lead_may_known}');
            <%--XN.json.parse('{"candidate":[{"id":306555611,"netName":"\u5b81\u6ce2\u5927\u5b66","name":"\u738b\u4f73\u4f73","head" :"http:\/\/hdn.xnimg.cn\/photos\/hdn321\/20091223\/1835\/tiny_PvJh_57757p019117.jpg"},{"id":282079587 ,"netName":"\u5317\u4eac\u5e02\u7b2c\u4e8c\u5341\u56db\u4e2d\u5b66","name":"\u5218\u59e5","head":"http :\/\/hdn.xnimg.cn\/photos\/hdn121\/20091201\/1210\/h_tiny_yuHO_7b290000923e2f75.jpg"},{"id":282079695 ,"netName":"\u5357\u5e73\u5e02","name":"\u5f20\u5bb6d","head":"http:\/\/hdn.xnimg.cn\/photos\/hdn321 \/20091201\/1210\/h_tiny_hx03_7b29000092762f75.jpg"},{"id":42033604,"netName":"\u5929\u6d25\u5de5\u5927" ,"name":"\u80e1\u6ce2[Hubot]","head":"http:\/\/hdn.xnimg.cn\/photos\/hdn411\/20090716\/1015\/tiny_Npbe_37364k204237 .jpg"},{"id":282079327,"netName":"","name":"\u674e\u94c1","head":"http:\/\/hdn.xnimg.cn\/photos\/hdn421 \/20091201\/1205\/h_tiny_XruC_7b290000918f2f75.jpg"},{"id":282078819,"netName":"\u6d4e\u5b81\u5e02","name" :"\u674e\u51fa","head":"http:\/\/hdn.xnimg.cn\/photos\/hdn221\/20091113\/1125\/h_tiny_ZrIA_62f700000fb12f74 .jpg"},{"id":250805736,"netName":"\u5317\u4eac\u5e02","name":"\u6797\u60e0\u534e","head":"http:\/\/hdn .xnimg.cn\/photos\/hdn521\/20091004\/2015\/tiny_SSrQ_99325j019117.jpg"},{"id":282079275,"netName":"California" ,"name":"\u674e\u8087ssss","head":"http:\/\/hdn.xnimg.cn\/photos\/hdn221\/20091201\/1205\/h_tiny_apxR_7b29000091462f75.jpg"}],"qkey":"friend","groups":["\u5927\u5b66\u540c\u5b66","\u666e\u901a\u670b\u53cb"]}');--%>
        </script>
	<div id="content">
		<div id="" class="friends-box">
		<%
		try{
			com.xiaonei.platform.core.model.User host = (com.xiaonei.platform.core.model.User)request.getAttribute("visiter");
			com.xiaonei.platform.core.model.User friend = com.xiaonei.reg.guide.util.GuideUtil.getAFriend(host);
			if(friend != null){
				request.setAttribute("visiter",friend);
			}
		}
		catch(Throwable e){
			
		}
		%>
	    	<div class="pic"><img src="${visiter.tinyFullUrl }" alt="${visiter.name }" /></div>
		    <span class="name">${visiter.name }</span>
		    <span class="online-icon"><img src="http://a.xnimg.cn/imgpro/icons/online_1.gif" alt="在线" /> 当前在线</span>
		</div>
	   <%
    int brown0 = IGuideUserLeadType.XFiveBrown_0;
    %>
    <c:set var="user_type_brown0" value="<%=brown0%>" />
    <c:choose>
        <c:when test="${requestScope.lead_user_type == user_type_brown0}">
            <div class="nav-register second clearfix">
                <p>1.寻找朋友</p>
                <p class="online">2.完善资料，找到熟人</p>
                <p>3.上传头像，进入首页</p>
            </div>
        </c:when>
        <c:otherwise>
            <div class="nav-register second clearfix">
		        <p>1.完善资料</p>
		        <p class="online">2.找到好友</p>
		        <p>3.上传头像，进入首页</p>
		    </div>
        </c:otherwise>
    </c:choose>  

	
    <div class="user-data clearfix">
        <div class="title titleimg">
            <h1><strong>填写你的个人信息，10秒找到好友</strong></h1>
        </div><!--title end-->

        <div id="friendSelector-container" class="friendSelector-box friendSelector-main"> </div>
        <div class="btn-box">
        <form action="/pvbrown-s?from=sg_reg_all" method="post" id="postFriendsForm">
            <input type="hidden" value="" name="friends"/>
            <input type="hidden" value="" name="fcount"/>
            <input type="button" class="input-submit" value="把选中的人加为好友" id="submit-btn"/>
            <a href="?skip=1" id="submit-skip" class="skip">跳过</a>
        </form>
        <div id="showScroll"></div>
        </div>
    </div><!--user-data end-->
</div><!--content end-->
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/xn.ui.multiFriendSelectorBox.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/xn.ui.pager.js?ver=$revxxx$"></script>
<script type="text/javascript" >
        (function(){
            <%--统计--%>
            var guide_stats = function( tag ) {
                var t = new Image();
                var rand = Math.random(); 
                t.src = 'http://guide.' + XN.env.domain + '/xtrs?nur=' + tag + '&t=' + rand ;
                 
            };
	    
            <%--到达--%>
//	    XN.dom.ready(function(){
  //          guide_stats( 'rggp2v4a0' ) ;
//	    })
			
            var container = $('friendSelector-container');
            if(!recommandFriends || recommandFriends.candidate.length==0){
	    	location.href = $('submit-skip').href;
                //container.setContent('没有找到好友 ：（');
                return;
            };
              
            var params = {
                noCompleteButton: true,
                noCancelButton: true,
                createInputElements: false,
                noAjaxData: true,
                provideData: recommandFriends,
                noFilter:true,
                noSearchInput:true,
                showNetname:true,
                friendsCountPerPage:100000
            };
            var  selector = new XN.ui.multiFriendSelectorBox(params);
            var srid = selector.ID;
            
            
            
            <%--全部/已选/未选--%>
            XN.event.addEvent('mfsbox' + srid + '_unselectTab', 'click', function(e){
                guide_stats('rggp2v4a1');
            });
            XN.event.addEvent('mfsbox' + srid + '_selectedTab', 'click', function(e){
                guide_stats('rggp2v4a1');
            });
            XN.event.addEvent('mfsbox' + srid + '_showAllTab', 'click', function(e){
                guide_stats('rggp2v4a1');
            });

            XN.event.addEvent('mfsbox' + srid + '_pager', 'click', function(e){
                guide_stats('rggp2v4a2');
            });
            
           
            XN.event.addEvent('submit-skip', 'click', function(e){
                guide_stats('rggp2v4a4');
            });
            XN.event.addEvent('submit-btn', 'click', function(e){
                guide_stats('rggp2v4a3');
            });
            <%--统计结束--%>

            container.setContent( selector );
            
            setTimeout(function(){
				$('showScroll').scrollIntoView(false)
			},300);

            $('postFriendsForm').fcount.value = recommandFriends.candidate.length;
         
            $('submit-btn').onclick = function(){
               var friends = selector.getSelectedFriends();
               if(friends.length === 0 ){
                    XN.DO.showError('你还没有选择好友');
                    return;
               }  

               var xhr = new XN.net.xmlhttp({
                    url: 'ajaxGuideBuddyReplyNotify.do',
                    data : 'sid=' + XN.cookie.get('id') + '&fids=' + friends.join(','), 
                    onSuccess : function(){
                       $('postFriendsForm').friends.value = friends.join(',');
                       $('postFriendsForm').submit();
                    },
                    onError : function(){
                       $('postFriendsForm').friends.value = friends.join(',');
                       $('postFriendsForm').submit();
                    }
                });
            

            } 

         })()
</script>
<%@ include file="/inc/footer-guide-flow.inc" %>
<script type="text/javascript">var page_id = "p21";</script>
<%@ include file="/inc/guide_stat.inc" %>
</body>
</html>
