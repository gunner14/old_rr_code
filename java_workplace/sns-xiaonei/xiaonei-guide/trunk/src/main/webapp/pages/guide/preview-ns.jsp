<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
	<head>
		<%@ include file="/inc/head.inc" %>
		<link media="all" type="text/css" rel="stylesheet" href="http://s.xnimg.cn/css/guide.css?ver=$revxxx$" />
		<link href="http://s.xnimg.cn/css/friends.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
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

		<title>${domainIdentified["siteName_fillinfo"]}</title>
	</head>
	<body id="pageFriends" class="msninvite-bg">
		<c:set var="hideLink" value="yes" />
		<div id="container" >
			<%@ include file="/inc/header-guide-flow.inc" %>
			<div id="opi" class="page-wrapper clearfix">
				<div class="full-page-holder">
					<div class="full-page clearfix guide-msninvite">
						<div class="compatible">
							<div id="content">
								<div class="guide-invite-steps clearfix">
									<ul class="current1">
										<li class="current">
											<h4>第1步:</h4>
											<p>找到我的朋友</p>
										</li>
										<li>
											<h4>第2步:</h4>
											<p>上传头像</p>
										</li>
									</ul>
								</div>		
								<!--begin:msn-->
								<div class="invite_box">
									<div class="identities cube">
										<h2>点击头像，选择您可能认识的人：</h2>
										<div id="list-results" class="progress-holder">
											<script type="text/javascript">
												var friends = new Array();

												function showarray(){
													var pars = "friends=" + friends;
													if(friends == null || friends.length==0){
														alert("请选择您想加的好友");
														//window.location = "${this_url}";
														return;
													}

													new Ajax.Request('/requestFriend.do', {
															method: 'post',
															parameters: pars,
															onSuccess: function(transport) {
																if(transport.responseText=='1'){
																	window.location="${next_url}";
																}else{
																	alert("请选择你加的好友");
																	window.location="${this_url}";
																}
															}
													});
												}
												function selector(id){
													var s = document.getElementById(id);
													var len = friends.length;
													if(s.className == "select"){
														s.className = "";
														for(var i=0;i<friends.length;i++){
															if(friends[i]==id){
																friends.remove(i);
															}
														}
													}else{
														s.className = "select";
														friends[len] = id;
													}
												}
												Array.prototype.remove=function(dx)
												{
													if(isNaN(dx)||dx>this.length){return false;}
													for(var i=0,n=0;i<this.length;i++)
													{
														if(this[i]!=this[dx])
														{
															this[n++]=this[i];
														}
													}
													this.length-=1;
												};
											</script>
										<!--真正的复选加好友-->
										<div class="selector">
											<ul>
												<c:forEach var="item" items="${requestScope.mixList}" begin="0" end="61">
													<li id="${item.id}" class="" onclick="selector(${item.id})">
														<a href="#"> 
															<span class="picbox">
																<span class="pic" style=" background-image:url(${item.tinyUrl});">
																	<img class="added" src="${applicationScope.urlStatic}/img/friend_selector_pic_selected.gif" />
																</span>
															</span>
															<h4>${item.name}</h4>
														</a> 
													</li>
												</c:forEach>
											</ul>
										</div>
										<!--End 真正的复选加好友-->
									</div>				

								</div>
								<p class="addingfri">
									<a class="float-right" href="${skip_url}">跳过</a>
									<input type="submit" id="add_friends" class="btn-5" value="加为好友" onclick="showarray();return false;"/>						
								</p>
							</div>
							<!--end:msn-->
						</div>
					</div>
				</div>
			</div>
		</div>
		<c:set var="hideFooterLink" value="true" />
    <c:set var="noWebPager" value="true" />
			<%@ include file="/inc/footer-noad.inc" %>
		</div>
	</body>
</html>
