<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@page import="java.util.List"%>
<%@page import="com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil"%>
<%@page import="com.xiaonei.platform.core.model.User"%>
<%@page import="java.util.Map"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
	<head>
		<%@ include file="/inc/head-guide-flow.inc" %>
		<script src="${applicationScope.urlSmallStatic}/jspro/guide/xn.guide.darkcoral_statis.js?ver=$revxxx$" type="text/javascript" ></script>
		<link href="${applicationScope.urlSmallStatic}/apps/tag/css/tag-all-min.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all">
		<link href="${applicationScope.urlSmallStatic}/csspro/apps/guide-reg.css?ver=$revxxx$" rel="stylesheet" rev="stylesheet" media="all" />
		<title>${domainIdentified["siteName_fillinfo"]} - 选择你的兴趣</title>
	</head>
	<body style="height:100%;">
		<div class="main_wrap clearfix">
			<div class="reg_logo">选择你的兴趣</div>
			<div class="guide-main-site clearfix">
				<span class="login"><a href="http://www.renren.com/Logout.do" >退出</a></span>
				<div class="interest">
					<h3>热门兴趣</h3>
				</div>
				<div class="guide-content">
					<div class="interest-box" style="position:relative;">
					<%-- 男 --%>
					<%
					    Map<String,String> inserestMap=(Map<String,String>)request.getAttribute("guiderecommendnickname");
					    
					    User user=GuideXFiveRequestUtil.getCurrentHost(request);
					    if(user.getGender().equals("男生")){
					%>
						<ul class="interest-list" id="interest-list">
							<%-- T2 --%>
							<li class="" data-name="电影" data-site="<%=inserestMap.get("电影")%>">
								<p>电影</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/dianying.jpg" height="110"/>
								<a class="opa" href="#nogo" onclick="return false;"></a>
								<a class="followed" href="#nogo" onclick="return false;"></a>
							</li>
							<%-- T1 --%>
							<li class="" data-name="数码" data-site="<%=inserestMap.get("数码")%>">
								<p>数码</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/shuma.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<li class="" data-name="互联网" data-site="<%=inserestMap.get("互联网")%>">
								<p>互联网</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/hulianwang.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<%-- T3 --%>
							<li class="" data-name="音乐" data-site="<%=inserestMap.get("音乐")%>">
								<p>音乐</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/yinyue.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<li class="" data-name="动漫" data-site="<%=inserestMap.get("动漫")%>">
								<p>动漫</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/dongman.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<li class="" data-name="建筑" data-site="<%=inserestMap.get("建筑")%>">
								<p>建筑</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/jianzu.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<li class="" data-name="汽车" data-site="<%=inserestMap.get("汽车")%>">
								<p>汽车</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/qiche.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							
							
							
							<%-- T2 --%>
							<li class="" data-name="旅行" data-site="<%=inserestMap.get("旅行")%>">
								<p>旅行</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/lvxing.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							
							<%-- T1 --%>
							<li class="" data-name="游戏" data-site="<%=inserestMap.get("游戏")%>">
								<p>游戏</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/youxi.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<li class="" data-name="摄影" data-site="<%=inserestMap.get("摄影")%>">
								<p>摄影</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/sheying.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							
							
							<%-- T3 --%>
							<li class="" data-name="科学" data-site="<%=inserestMap.get("科学")%>">
								<p>科学</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/kexue.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<li class="" data-name="军事" data-site="<%=inserestMap.get("军事")%>">
								<p>军事</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/junshi.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<li class="" data-name="篮球" data-site="<%=inserestMap.get("篮球")%>">
								<p>篮球</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/lanqiu.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<li class="" data-name="自然" data-site="<%=inserestMap.get("自然")%>">
								<p>自然</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/ziran.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							
							<%-- T4 --%>
							<li class="" data-name="武器" data-site="<%=inserestMap.get("武器")%>">
								<p>武器</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/wuqi.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<li class="" data-name="文艺" data-site="<%=inserestMap.get("文艺")%>">
								<p>文艺</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/wenyi.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<li class="" data-name="短片" data-site="<%=inserestMap.get("短片")%>">
								<p>短片</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/duanpian.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<li class="" data-name="星座" data-site="<%=inserestMap.get("星座")%>">
								<p>星座</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/xingzuo.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							
							<li class="" data-name="设计" data-site="<%=inserestMap.get("设计")%>">
								<p>设计</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/sheji.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<li class="" data-name="创意" data-site="<%=inserestMap.get("创意")%>">
								<p>创意</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/chuangyi.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<li class="" data-name="艺术品" data-site="<%=inserestMap.get("艺术品")%>">
								<p>艺术品</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/yishupin.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<%-- T5 --%>
							<li class="" data-name="服饰" data-site="<%=inserestMap.get("服饰")%>">
								<p>服饰</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/fushi.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<li class="" data-name="时尚" data-site="<%=inserestMap.get("时尚")%>">
								<p>时尚</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/shishang.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<li class="" data-name="购物" data-site="<%=inserestMap.get("购物")%>">
								<p>购物</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/gouwu.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<li class="" data-name="厨艺" data-site="<%=inserestMap.get("厨艺")%>">
								<p>厨艺</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/chuyi.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<li class="" data-name="家居" data-site="<%=inserestMap.get("家居")%>">
								<p>家居</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/jiaju.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<li class="" data-name="宠物" data-site="<%=inserestMap.get("宠物")%>">
								<p>宠物</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/chongwu.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<li class="" data-name="美食" data-site="<%=inserestMap.get("美食")%>">
								<p>美食</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/meishi.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
						
						</ul>
						
						<%-- 女 --%>
						<%
                            }
						    else{
						%>
						<ul class="interest-list" id="interest-list">
							
							<li class="" data-name="时尚" data-site="<%=inserestMap.get("时尚")%>">
								<p>时尚</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/shishang.jpg" height="110"/>
								<a class="opa" href="#nogo" onclick="return false;"></a>
								<a class="followed" href="#nogo" onclick="return false;"></a>
							</li>
							<%-- T1 --%>
							<li class="" data-name="星座" data-site="<%=inserestMap.get("星座")%>">
								<p>星座</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/xingzuo.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<li class="" data-name="音乐" data-site="<%=inserestMap.get("音乐")%>">
								<p>音乐</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/yinyue.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<%-- T3 --%>
							<li class="" data-name="美食" data-site="<%=inserestMap.get("美食")%>">
								<p>美食</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/meishi.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<li class="" data-name="艺术品" data-site="<%=inserestMap.get("艺术品")%>">
								<p>艺术品</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/yishupin.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<li class="" data-name="动漫" data-site="<%=inserestMap.get("动漫")%>">
								<p>动漫</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/dongman.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<li class="" data-name="购物" data-site="<%=inserestMap.get("购物")%>">
								<p>购物</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/gouwu.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							
							
							
							<%-- T2 --%>
							<li class="" data-name="旅行" data-site="<%=inserestMap.get("旅行")%>">
								<p>旅行</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/lvxing.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							
							<%-- T1 --%>
							<li class="" data-name="电影" data-site="<%=inserestMap.get("电影")%>">
								<p>电影</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/dianying.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<li class="" data-name="服饰" data-site="<%=inserestMap.get("服饰")%>">
								<p>服饰</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/fushi.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							
							
							<%-- T3 --%>
							<li class="" data-name="厨艺" data-site="<%=inserestMap.get("厨艺")%>">
								<p>厨艺</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/chuyi.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<li class="" data-name="文艺" data-site="<%=inserestMap.get("文艺")%>">
								<p>文艺</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/wenyi.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<li class="" data-name="宠物" data-site="<%=inserestMap.get("宠物")%>">
								<p>宠物</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/chongwu.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<li class="" data-name="家居" data-site="<%=inserestMap.get("家居")%>">
								<p>家居</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/jiaju.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							
							<%-- T4 --%>
							<li class="" data-name="创意" data-site="<%=inserestMap.get("创意")%>">
								<p>创意</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/chuangyi.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<li class="" data-name="摄影" data-site="<%=inserestMap.get("摄影")%>">
								<p>摄影</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/sheying.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<li class="" data-name="设计" data-site="<%=inserestMap.get("设计")%>">
								<p>设计</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/sheji.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<li class="" data-name="互联网" data-site="<%=inserestMap.get("互联网")%>">
								<p>互联网</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/hulianwang.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							
							<li class="" data-name="建筑" data-site="<%=inserestMap.get("建筑")%>">
								<p>建筑</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/jianzu.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<li class="" data-name="汽车" data-site="<%=inserestMap.get("汽车")%>">
								<p>汽车</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/qiche.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<li class="" data-name="自然" data-site="<%=inserestMap.get("自然")%>">
								<p>自然</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/ziran.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<%-- T5 --%>
							<li class="" data-name="科学" data-site="<%=inserestMap.get("科学")%>">
								<p>科学</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/kexue.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<li class="" data-name="军事" data-site="<%=inserestMap.get("军事")%>">
								<p>军事</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/junshi.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<li class="" data-name="游戏" data-site="<%=inserestMap.get("游戏")%>">
								<p>游戏</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/youxi.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<li class="" data-name="篮球" data-site="<%=inserestMap.get("篮球")%>">
								<p>篮球</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/lanqiu.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<li class="" data-name="数码" data-site="<%=inserestMap.get("数码")%>">
								<p>数码</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/shuma.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<li class="" data-name="短片" data-site="<%=inserestMap.get("短片")%>">
								<p>短片</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/duanpian.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
							<li class="" data-name="武器" data-site="<%=inserestMap.get("武器")%>">
								<p>武器</p>
								<img src="http://a.xnimg.cn/imgpro/guide/interest/wuqi.jpg" height="110"/>
								<a class="opa"></a>
								<a class="followed"></a>
							</li>
						</ul>
						<% }%>
					</div>
				</div>
				<div class="btns clearfix">
					<div class="selectedIn">
						<span>已选择：</span>
						<div class="selected-box" id="selected-box">
						
						</div>
					</div>
					<div class="next">
						<a id="submit" class="go-to-home">下一步</a>
						<a href="http://guide.renren.com/flowrecmodify5and6step?skip=1" id="skip">跳过</a>
					</div>       
				</div>
				<div class="tip"></div>
			</div>
			<div class="tiaokuan">
				点击免费开通账号表示您同意并遵守<a href="http://${applicationScope.sysdomain}/info/agreement.jsp" target="_blank">人人网服务条款</a>
				
			</div>
		</div>
		<script type="text/javascript">
		    <%-- 这里的userid是限制尾号用的，5和6无弹层，7和8有弹层 --%>
			FollowSiteByInterest.init(<%=user.getId()%>,'http://guide.renren.com/flowrecmodify5and6step');
		</script>
	</body>
</html>
