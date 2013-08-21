<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@page import="java.util.List"%>
<%@page import="com.renren.sns.tag.view.render.SuggestSiteRender"%>
<%@page import="com.renren.sns.tag.view.MinisiteCategory"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
	<head>
		<%@ include file="/inc/head-guide-flow.inc" %>
		<script src="${applicationScope.urlSmallStatic}/jspro/guide/xn.guide.darkcoral_statis.js?ver=$revxxx$" type="text/javascript" ></script>
		<link href="${applicationScope.urlSmallStatic}/apps/tag/css/tag-all-min.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all">
		<link href="${applicationScope.urlSmallStatic}/csspro/apps/guide-reg.css?ver=$revxxx$" rel="stylesheet" rev="stylesheet" media="all" />
		<title>${domainIdentified["siteName_fillinfo"]} - 关注精选内容</title>
	</head>
	<body>
		<div class="main_wrap clearfix">
			<div class="reg_logo">关注精选内容</div>
			<div class="guide-site-main clearfix">
				<span class="login"><a href="http://www.renren.com/Logout.do">退出</a></span>
				<style type="text/css">
					.site-title{
						heigth:39px;
						background:#B5dbdc;
						margin-top:-1px;
						border-bottom:2px solid #00b9bd;
					}
					.site-title .title{
						width:152px;
						height:24px;
						color:#888;
						float:left;
						text-align:right;
						margin-top:18px;
					}
					.site-title #site-class{
						float:left;
						margin-top:12px;
					}
					.site-title #site-class li{
						line-height:29px;
						float:left;
					}
					.site-title #site-class a{
						font-weight:bold;
						font-size:14px;
						color:#000;
						display:block;
						height:30px;
						padding:0 12px;
						color:#555;
						_float:left;
						outline:none;
					}
					.site-title #site-class a:hover,.site-title #site-class .selected a{
						background:#00b9bd;
						color:#fff;
						text-decoration:none;
					}
					
					.guide-site-main{
						background-color: #FFFFFF;
						border: 1px solid #B4CCDD;
						position: relative;
					}
					
					
					#bottom-home{
						background:#f7f7f7;
						height:65px;
						border-top:1px solid #e6e6e6;
						position:fixed;
						bottom:0px;
						width:100%;
						_position:absolute;
						_bottom:auto;	
				_top:expression(eval(document.documentElement.scrollTop+document.documentElement.clientHeight-this.offsetHeight-(parseInt(this.currentStyle.marginTop,10)||0)-(parseInt(this.currentStyle.marginBottom,10)||0)));
					}
					#submit-home{
						position:absolute;
						top:15px;
						top:15px;
						left:50%;
						font-size:14px;
						margin-left:-30px;
						font-weight:bold;
						color:white;
						text-decoration:none;
						} 
					
				</style>
				<div class="site-title clearfix" style="height:42px;">
					<h1 class="title">
						选择分类:
					</h1>
					<ul id="site-class">
						<!-- 兴趣列表-->
						<%! 
						 List<MinisiteCategory>  showMinisiteList=null;
                         MinisiteCategory chooseminisite=null;
						 int selectNumber=0;
						%>

						<%
						 showMinisiteList=(List<MinisiteCategory>)request.getAttribute("showminisitelist");
						 chooseminisite=(MinisiteCategory)request.getAttribute("chooseminisite");
						 selectNumber=(Integer)request.getAttribute("slectnumber");
						 MinisiteCategory minisiteCategory=null;
						 int size=showMinisiteList.size();
						 for(int  i=0;i<size;i++){
                            minisiteCategory=showMinisiteList.get(i);
						    if(i==selectNumber){
							
						%>
						<li data-tagid="<%=chooseminisite.gettagId()%>" data-tagname="<%=chooseminisite.getTagName()%>" class="selected"><a href="#nogo"><%=chooseminisite.getTagName()%></a></li>

						<%
							}
						    else{
                              
						%>
						 <li data-tagid="<%=minisiteCategory.gettagId()%>" data-tagname="<%=minisiteCategory.getTagName()%>"><a href="#nogo"><%=minisiteCategory.getTagName()%></a></li>
						<%
							}
						      }	
						%>
						
					</ul>
				</div>


				<%! 
				 List<SuggestSiteRender> suggestSiteList=null;
				%>

				<%
				 suggestSiteList=(List<SuggestSiteRender>)request.getAttribute("suggestsitelist");
				%>

                <div id="returnhtml">
				<div style="text-align:right;margin: 15px 90px -35px auto;" class="clearfix">
					<a href="#nogo" id="followall" style="display:block;float:right;margin-left:20px">全部关注</a>
					<span style="color:#888;display:block;float:right">共<%=suggestSiteList.size()%>个</span>
				</div>
				<div id="" class="guide-content" style="border-top:none;">
					<div id="friendPool" class="hisfriends-box" style="width:850px">
						<div class="smallsite-home" style="padding-bottom:0px;width:815px;position:relative;margin: 0 auto;">
							<div class="recommend" id="recommendSite" style="display:block;">
								<div id="tagContainer" class="recommend-content clearfix" style="padding-bottom:70px;">
								<!--小站类表 -->
									<ul id="site-list">
									

										<c:forEach var="suggest" items="<%=suggestSiteList%>">

											<li data-id="${suggest.id}" data-name="${suggest.profileUrl}">            
												 <a class="recommend-pic">
												 <img src="${suggest.headSource}" title="${suggest.name}" alt="${suggest.name}">
													</a>
													<h3><a title="${suggest.name}" href="#nogo">${suggest.name}</a></h3>
													<div class="recommend-descript">
													${suggest.description}
													</div>
													<p class="attention-num">${suggest.followerCount}人关注</p>
													
												   <c:choose>
														<c:when test="${suggest.following}">
															<div class="recommend-btn2">已关注</div>
														</c:when>
														<c:otherwise>
															<div class="recommend-btn"><a  href="javascript:;" class="recommend-att-btn">关注</a></div>
														</c:otherwise>
													</c:choose>
											</li>
										</c:forEach>										
									</ul>
								</div>
							</div>
						</div>
					</div>
				</div>
				</div>
			</div>
		</div>
		<div id="bottom-home">
			<a type="submit" class="input-submit" id="submit-home" href="http://guide.renren.com/flowrecommendmodifystep">进入首页</a>
		</div>
		<script>
			FollowSite.init();
		</script>
	</body>
</html>
