<%@ page contentType="text/html;charset=UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<!-- _pv_brown_ -->
<head>
	<%@ include file="/inc/head-guide-flow.inc" %>
	<link href="${applicationScope.urlSmallStatic}/csspro/module/school-selector.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
	
	<link href="${applicationScope.urlSmallStatic}/csspro/apps/guide-new.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
    <link href="${applicationScope.urlSmallStatic}/csspro/module/friendSelector.css?ver=$revxxx$" media="all" type="text/css" rel="stylesheet" /> 
	<title>${domainIdentified["siteName_fillinfo"]} - 选择你认识的人加为好友</title>

</head>
<body id="home" class="guide">
		<%@ include file="/inc/header-guide-flow.inc" %>
        <script type="text/javascript">
	    	var recommandFriends =  XN.json.parse('${requestScope.lead_may_known}');
            <%--XN.json.parse('{"candidate":[{"id":306555611,"netName":"\u5b81\u6ce2\u5927\u5b66","name":"\u738b\u4f73\u4f73","head" :"http:\/\/hdn.xnimg.cn\/photos\/hdn321\/20091223\/1835\/tiny_PvJh_57757p019117.jpg"},{"id":282079587 ,"netName":"\u5317\u4eac\u5e02\u7b2c\u4e8c\u5341\u56db\u4e2d\u5b66","name":"\u5218\u59e5","head":"http :\/\/hdn.xnimg.cn\/photos\/hdn121\/20091201\/1210\/h_tiny_yuHO_7b290000923e2f75.jpg"},{"id":282079695 ,"netName":"\u5357\u5e73\u5e02","name":"\u5f20\u5bb6d","head":"http:\/\/hdn.xnimg.cn\/photos\/hdn321 \/20091201\/1210\/h_tiny_hx03_7b29000092762f75.jpg"},{"id":42033604,"netName":"\u5929\u6d25\u5de5\u5927" ,"name":"\u80e1\u6ce2[Hubot]","head":"http:\/\/hdn.xnimg.cn\/photos\/hdn411\/20090716\/1015\/tiny_Npbe_37364k204237 .jpg"},{"id":282079327,"netName":"","name":"\u674e\u94c1","head":"http:\/\/hdn.xnimg.cn\/photos\/hdn421 \/20091201\/1205\/h_tiny_XruC_7b290000918f2f75.jpg"},{"id":282078819,"netName":"\u6d4e\u5b81\u5e02","name" :"\u674e\u51fa","head":"http:\/\/hdn.xnimg.cn\/photos\/hdn221\/20091113\/1125\/h_tiny_ZrIA_62f700000fb12f74 .jpg"},{"id":250805736,"netName":"\u5317\u4eac\u5e02","name":"\u6797\u60e0\u534e","head":"http:\/\/hdn .xnimg.cn\/photos\/hdn521\/20091004\/2015\/tiny_SSrQ_99325j019117.jpg"},{"id":282079275,"netName":"California" ,"name":"\u674e\u8087ssss","head":"http:\/\/hdn.xnimg.cn\/photos\/hdn221\/20091201\/1205\/h_tiny_apxR_7b29000091462f75.jpg"}],"qkey":"friend","groups":["\u5927\u5b66\u540c\u5b66","\u666e\u901a\u670b\u53cb"]}');--%>
        </script>
	<div id="content">
    <div class="user-data clearfix">

        <div class="title">
            <h1>选择你认识的人</h1>
            <p>把你认识的人加为好友，随时获得好友动态。</p>
        </div><!--title end-->

        <div id="friendSelector-container" class="friendSelector-box friendSelector-main"> </div>
        <div class="btn-box">
        <form action="/pvbrown-s?from=sg_reg_all" method="post" id="postFriendsForm">
            <input type="hidden" value="" name="friends"/>
            <input type="hidden" value="" name="fcount"/>
            <input type="button" class="input-submit" value="把选中的人加为好友" id="submit-btn"/>
            <a href="?skip=1" id="submit-skip">跳过</a>
        </form>
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

            var container = $('friendSelector-container');
            if(!recommandFriends || recommandFriends.candidate.length==0){
                container.setContent('没有找到好友 ：（');
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
                showNetname:true
            };
            var  selector = new XN.ui.multiFriendSelectorBox(params);
            var srid = selector.ID;
            
            <%--到达--%>
            guide_stats( 'rggp2v1a0' ) ;

            <%--点击选头像--%>
		    selector.addEvent( 'selectOne' , function(){
                guide_stats('rggp2v1a3');
            });
           
            <%--全部/已选/未选--%>
            XN.event.addEvent('mfsbox' + srid + '_unselectTab', 'click', function(e){
                guide_stats('rggp2v1a1');
            });
            XN.event.addEvent('mfsbox' + srid + '_selectedTab', 'click', function(e){
                guide_stats('rggp2v1a1');
            });
            XN.event.addEvent('mfsbox' + srid + '_showAllTab', 'click', function(e){
                guide_stats('rggp2v1a1');
            });

            XN.event.addEvent('mfsbox' + srid + '_pager', 'click', function(e){
                guide_stats('rggp2v1a2');
            });

            <%--跳过--%>
            XN.event.addEvent('submit-skip', 'click', function(e){
                guide_stats('rggp2v1a5');
            });
            <%--把选中的人加为好友--%>
            XN.event.addEvent('submit-btn', 'click', function(e){
                guide_stats('rggp2v1a4');
            });
            <%--统计结束--%>

            container.setContent( selector );

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
</body>
</html>
