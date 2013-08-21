<%@ page contentType="text/html;charset=UTF-8" session="false" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/fmt" prefix="fmt" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>
<%@ taglib uri="http://www.renren.com/tags-core" prefix="xn" %>
<%@ taglib uri="http://jakarta.apache.org/taglibs/string-1.1" prefix="str" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml">
	<head>
		<%@ include file="/inc/headpro.inc" %><%-- 头部信息 --%>
		<%@ include file="home_css_js.jsp" %><%-- CSS And JS--%>
        <script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/xn.ui.multiFriendSelectorBox.js?ver=$revxxx$"></script>
		<script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/xn.ui.pager.js?ver=$revxxx$"></script>
		<link href="${applicationScope.urlSmallStatic}/csspro/module/friendSelector.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
	<title>${domainIdentified["messages.domain.title"]} - ${visiter.name}</title>
	</head>
	<body id="home" class="home">	
		<c:set var="channel" value="首页" />
        <c:set var="cleanpage" value="1" />
		<%@ include file="/inc/header.inc" %>
    <div class="page-wrapper clearfix" id="opi">
                <div class="full-page-holder">
                    <div class="full-page clearfix">
                        <style type="text/css">
                            .user-info{margin-bottom:15px;}
                            .user-info .user-avatar{float:left; width:50px;}
                            .user-info .user-detail{margin-left:60px;zoom:1; font-weight:bolder;padding-top:5px; font-size:14px;}
                            .user-info .user-detail p{margin-bottom:10px;}
                            .user-info .user-detail p.welcome .name{margin-right:15px;}
                            .user-info .user-detail p.desc{color:#666666;}
                            .commends .legend{color:#888;margin-bottom:-16px;}
                            .commends .acts{text-align:right; clear:both;}
                            .commends .selector ul{overflow:hidden; zoom:1;}
                            .commends .selector ul li{margin-right:10px!important;}
                            .friendSelectorBox .selector{height:auto!important;overflow:auto!important;}
                        </style>
                        <div class="user-info clearfix">
                            <div class="user-avatar">
                                <a href="http://${applicationScope.domain}/profile.do?id=${visiter.id}"><img src="${visiter.tinyFullUrl}"/></a>
                            </div>
                            <div class="user-detail">
                                <p class="welcome"><a class="name" href="http://${applicationScope.domain}/profile.do?id=${visiter.id}">${visiter.name}</a><span>你好！欢迎回到开心网</span></p>
                                <p class="desc">以下是你可能认识的人，你可以添加他们为好友</p>
                            </div>
                        </div>
                        <div class="commends clearfix">
                            <p class="legend">用鼠标选中好友头像</p>
                            <div id="commends">	</div>
                            <div class="acts">
                                <input type="button" value="发送邀请" id="submit-btn" class="input-submit"/>
                                <input type="button" value="回到首页" id="back-btn" class="input-submit gray"/>
                            </div>
                        </div>
                    </div>
                </div>
            </div>

        <script type="text/javascript">
        (function(){
            var friendsData= ${requestScope.data};
            <%--XN.json.parse('{"candidate":[{"id":306555611,"netName":"\u5b81\u6ce2\u5927\u5b66","name":"\u738b\u4f73\u4f73","head" :"http:\/\/hdn.xnimg.cn\/photos\/hdn321\/20091223\/1835\/tiny_PvJh_57757p019117.jpg"},{"id":282079587 ,"netName":"\u5317\u4eac\u5e02\u7b2c\u4e8c\u5341\u56db\u4e2d\u5b66","name":"\u5218\u59e5","head":"http :\/\/hdn.xnimg.cn\/photos\/hdn121\/20091201\/1210\/h_tiny_yuHO_7b290000923e2f75.jpg"},{"id":282079695 ,"netName":"\u5357\u5e73\u5e02","name":"\u5f20\u5bb6d","head":"http:\/\/hdn.xnimg.cn\/photos\/hdn321 \/20091201\/1210\/h_tiny_hx03_7b29000092762f75.jpg"},{"id":42033604,"netName":"\u5929\u6d25\u5de5\u5927" ,"name":"\u80e1\u6ce2[Hubot]","head":"http:\/\/hdn.xnimg.cn\/photos\/hdn411\/20090716\/1015\/tiny_Npbe_37364k204237 .jpg"},{"id":282079327,"netName":"","name":"\u674e\u94c1","head":"http:\/\/hdn.xnimg.cn\/photos\/hdn421 \/20091201\/1205\/h_tiny_XruC_7b290000918f2f75.jpg"},{"id":282078819,"netName":"\u6d4e\u5b81\u5e02","name" :"\u674e\u51fa","head":"http:\/\/hdn.xnimg.cn\/photos\/hdn221\/20091113\/1125\/h_tiny_ZrIA_62f700000fb12f74 .jpg"},{"id":250805736,"netName":"\u5317\u4eac\u5e02","name":"\u6797\u60e0\u534e","head":"http:\/\/hdn .xnimg.cn\/photos\/hdn521\/20091004\/2015\/tiny_SSrQ_99325j019117.jpg"},{"id":282079275,"netName":"California" ,"name":"\u674e\u8087ssss","head":"http:\/\/hdn.xnimg.cn\/photos\/hdn221\/20091201\/1205\/h_tiny_apxR_7b29000091462f75.jpg"}],"qkey":"friend","groups":["\u5927\u5b66\u540c\u5b66","\u666e\u901a\u670b\u53cb"]}');--%>
            
            if(!friendsData || friendsData.candidate.length==0){
                location.href='http://friend.'+ XN.env.domain +'/myfriendlistx.do#item_1';
                return;
            }
              
            var params = {
                noCompleteButton: true,
                noCancelButton: true,
                createInputElements: false,
                noAjaxData: true,
                provideData: friendsData,
                noFilter:true,
                noSearchInput:true
            };

            var gotoHome = function(){
                 location.href = 'http://' + XN.env.domain; 
            }

            var  selector = new XN.ui.multiFriendSelectorBox(params);
            $('commends').setContent( selector );
         
            $('submit-btn').onclick = function(){
               var friends = selector.getSelectedFriends();
                if(friends.length === 0 ){
                    XN.DO.showError('你还没有选择好友');
                    return;
                }
                new XN.net.xmlhttp({
                    url : 'http://friend.' + XN.env.domain + '/ajaxPUKRequest',     
                    data : 'param=' + friends.join(','),
                    onSuccess : function(r){
                        XN.DO.showMessage('发送申请成功'); 
                        setTimeout(gotoHome,2000);         
                    },
                    onError : function(r){
                           gotoHome();          
                    }        
                });
            
            } 

            $('back-btn').onclick = function(){
                    location.href = 'http://' + XN.env.domain;           
            }
         })()
        </script>
		<%@ include file="/inc/footerdiv.inc" %>		
	</body>
</html>

