<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://www.renren.com/tags-core" prefix="xn" %>
<%@ taglib uri="http://www.renren.com/tags-core" prefix="xiaonei" %>
<% com.xiaonei.platform.core.model.User user = com.xiaonei.platform.core.opt.base.BaseThreadUtil.currentHost(request); %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
	<head>
		<c:set var="INC_COMMON_CSS" value="true" />
	    <%@ include file="/inc/headpro.inc"%>
		<%@ include file="/views/guide/xf/common/top/guide_xf_css_js.jsp"%>
		<%@ include file="/views/guide/xf/common/top/guide_xf_json.jsp"%>		
  		<title>${domainIdentified["siteName_fillinfo"]} - ${requestScope.host.name}</title>
	</head>
	<body id="dashboardPage" class="home guide" g="999_999">
		<input type="hidden" id="COLOR_CLOSE_FLAG" value="${COLOR_CLOSE_FLAG}"/>
		<div id="zidou_template" style="display:none"></div>
		<div id="hometpl_style" style="display:none"><br/>
			<style><c:if test="${domainIdentified['sys.domain']=='renren.com'}">${homeTipCode}</c:if></style>
	    </div>
		<c:set var="cleanpage" value="ture" /> 
		<c:set var="channel" value="首页" />
		<%@ include file="/inc/header.inc"%>
		<%@ include file="/inc/wrapper_home.inc" %>
		<%@ include file="/pages/register/guide_app_menu.jsp"%><%--应用菜单 --%>	
		<div class="main-page"> 
			<div class="home-content-holder clearfix">
				<div class="home-body">  
					<%@ include file="/views/guide/xf/common/body/guide_vip_remind.jsp"%>	
					<%@ include file="/views/guide/xf/common/body/guide_body_notice.jsp"%>					
					<%@ include file="/views/guide/xf/common/middle/guide_invite_app.jsp"%>					
					<div id="publisher_frame" class="publisher">						
						<%@ include file="/inc/publisher/publisher-show.inc" %>
					</div>						
					<%@ include file="/views/guide/xf/zottf/body/guide_my_renren.jsp"%>						
   					<%@ include file="/views/guide/xf/zot/body/guide_xf_body.jsp"%>	
				</div>
				<div class="home-sidebar"> 
               		<%@ include file="/pages/register/guide_manage_page.jsp" %>  
               		<%@ include file="/pages/register/guide_usercount.jsp" %>  
               		<%@ include file="/views/guide/xf/common/right/guide_highschool_invite.jsp"%>			                										
         			<%@ include file="/pages/register/guide_footprint.jsp" %><%--最近来访部分--%> 
         			<%@ include file="/views/guide/xf/common/right/guide_xiaohua.jsp" %><%--符颖的校花校草--%> 
         			<%@ include file="/views/guide/xf/common/right/guide_sponsors_widget.jsp" %>
         			<%@ include file="/views/guide/xf/common/right/guide_frid_birthday.jsp" %>	
         			<%@ include file="/views/guide/common/right/guide_university_diary.jsp" %>		           				       				      
					<%@ include file="/views/guide/xf/common/right/guide_friend_list.jsp" %>									
         			<%@ include file="/pages/guide/right/guide_my_pages.jsp" %>   
         			<%@ include file="/views/guide/xf/common/right/guide_get_touch.jsp"%>	
         			<%@ include file="/views/guide/right/guide_ex_star.jsp" %><%-- 中学网络人气之星 --%>				   			
                </div>              	
			</div>
		</div></div></div></div>
		<%@ include file="/inc/footerdiv.inc" %>
	</body>
</html>
