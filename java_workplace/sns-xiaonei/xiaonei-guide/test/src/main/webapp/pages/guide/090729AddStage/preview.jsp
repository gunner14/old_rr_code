<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
	<head>
		<%@ include file="/inc/head-guide-flow.inc" %>
		<link href="http://s.xnimg.cn/css/dialogpro.css?ver=$revxxx$" rel="stylesheet" type="text/css">
		<link href="http://s.xnimg.cn/css/friends.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all">
		<link href="http://s.xnimg.cn/css/guide.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all">
		<script type="text/javascript" src="http://s.xnimg.cn/jspro/xn.ui.pager.js?ver=$revxxx$"></script>
		<script type="text/javascript" src="http://s.xnimg.cn/jspro/xn.ui.multiFriendSelectorBox.js?ver=$revxxx$"></script>
		<style type="text/css">
			.friendSelectorBox .selector{ height:362px; overflow:hidden; }
			.friendSelectorBox .selector ul li{ width:148px;}
		</style>
		<title>${domainIdentified["siteName_fillinfo"]} - 寻找朋友</title>

	</head>
	<body>
	<script type="text/javascript">
    new Ajax.Request('/xtrs?pma=p_reg_add_friend_old_m_a_arrive', {
        method: 'get',
        parameters: '',
        onSuccess: function(transport){
        }
    });
    </script>   
		<c:set var="hideLink" value="yes" />
		<%@ include file="inc/menu_withif.inc" %>
		<div id="opi" class="page-wrapper clearfix">
			<div class="full-page-holder">
				<div class="full-page clearfix">
					<div class="compatible">
						<div id="content">
						
							<div class="new-guide">
								<h3 class="welcome">欢迎来到${domainIdentified["siteName"]}网，<%=(String)request.getAttribute("user_name") %></h3>
								<%@ include file="inc/navbar_2preview.inc" %>								
<div class="personal-info">
									<h4>选择你认识的人</h4>
									<p class="description">把你认识的人加为好友，随时获得好友动态。</p>
									<div id="fsb-holder">
									</div>
									<div class="confirm-friends clearfix">
										<span class="float-right">
											<input id="add_friends" type="submit" class="input-submit" value="把选中的人加为好友" />
											<a id="skip_btn" href="?skip=1" class="jumpover">跳过</a>
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
		<script type="text/javascript" src="http://s.xnimg.cn/jspro/guide/xn.page.guide.selectFriends.js?ver=$revxxx$"></script>
	</body>
</html>
