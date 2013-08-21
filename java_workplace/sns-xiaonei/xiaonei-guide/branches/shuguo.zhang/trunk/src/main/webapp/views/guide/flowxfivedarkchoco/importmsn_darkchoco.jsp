<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html"%>
<%@ taglib uri="http://www.renren.com/tags-core" prefix="xiaonei"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<%@page import="com.xiaonei.reg.guide.root.interfaces.IGuideUserLeadType"%><html xmlns="http://www.w3.org/1999/xhtml">

<head><!-- _fi_darkchoco_ -->
    <%@ include file="/inc/head-guide-flow.inc" %>
    <link href="${applicationScope.urlSmallStatic}/csspro/module/school-selector.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all">
    <link href="${applicationScope.urlSmallStatic}/csspro/apps/guide-message-register.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all">
    <title>${domainIdentified["siteName_fillinfo"]} - 寻找朋友</title>
</head>
<body id="home" class="guide">
<%@ include file="/inc/header-guide-flow.inc" %>
	<div id="content">
	<%@ include file="inc/logo-div.inc" %>
	<%
    int brown0 = IGuideUserLeadType.XFiveBrown_0;
    %>
    <c:set var="user_type_brown0" value="<%=brown0%>" />
    <c:choose>
        <c:when test="${requestScope.lead_user_type == user_type_brown0}">
            <div class="nav-register first clearfix">
                <p class="online">1.寻找朋友</p>
                <p>2.完善资料，找到熟人</p>
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

    <div class="user-data">
    <!-- 
        <div class="title titleimg">
            <h1>你好<span><%=(String)request.getAttribute("user_name") %></span> 欢迎来到${domainIdentified["siteName"]}网</h1>
            <p>把你认识的人加为好友，随时获得好友动态。</p>
        </div>title end
     -->   
        <div class="pinfo-form import-msn" style="display:none">  </div>
        <div class="msn-Import msnAccount">
        <p id="notice" class="errorTips" style="display:none">帐号或密码错误，请尝试重新输入。</p>
        <h1>通过MSN或邮箱寻找你已经在人人上的朋友们</h1>
        
            <dl>
	            <dt>
	                <!-- <label class="hidden">邮箱或MSN :</label> -->
	                <label class="email" for="email"><input type="radio" id="email" name="emailmsn" <%="webmail".equals(request.getAttribute("initform"))?"checked=\"true\"":""%> /> <img src="${applicationScope.urlStatic}/imgpro/guide/email.gif"/> 邮箱　　　　</label>
	                <label class="msn" for="msn"><input type="radio" id="msn" name="emailmsn" <%="msnsend_form".equals(request.getAttribute("initform"))?"checked=\"true\"":""%>/> <img src="${applicationScope.urlStatic}/imgpro/guide/msn.gif"/> MSN</label>
                </dt>
                <form target="loadingf" class="autoSubmit" subtn="add_friends" action="${applicationScope.urlInvite}/regGudDpt.do" id="msnsend_form" method="post" name="msnsend" style="display:block">
                    <input type="hidden" name="rxurl" value="${rurl}"/>
                    <input type="hidden" name="oxurl" value="${ourl}"/>
                    <input type="hidden" name="regss" value="shortform" />
                    <input type="hidden" name="importer" value="@hotmail.com" />
                    <input type="hidden" name="header_type" value="${isappguide}"/>
                    <input type="hidden" name="ifs" value="_AAA"/>
                
                        <!-- <dt>寻找你MSN联系人</dt> -->
                        <dd>MSN帐号 : <input id="msn_num" type="text" class="input-text" name="uAccount" tabindex="1" value="${user_email}" rule="blank" schema="xn_noempty:demo@hotmail.com;xn_isemail" /></dd>
                        <dd>MSN密码 : <input id="msn_pwd" type="password" class="input-text" name="pwd" tabindex="2" rule="blank" schema="xn_noempty" /></dd>
                        
                    <!-- 
                    <p class="form-action">
                        <input id="submit_msn" class="input-submit" type="button" value="搜索（这是老的 要换成新样式）"/>
                        <a id="skip_btn_msn" href="imports" class="jumpover">跳过</a>
                        <img id="load_img_msn" class="loadingimg" src="${applicationScope.urlStatic}/img/indicator.gif" />
                    </p>
                     -->
                     <div class="btn-box">
		                <input type="button" id="submit_msn" value="搜索好友" class="input-submit" />
		                <img id="load_img_msn" class="loadingimg" src="${applicationScope.urlStatic}/img/indicator.gif" />
		                <a href="imports" id="skip_btn_msn" class="skip">跳过</a>
		                <div class="register-tips">
		                    <div class="arrow"></div>
		                    <p>${domainIdentified["siteName"]}网不会记录你的密码，<br />请放心使用。</p>
		                </div>
		            </div><!---btn-box end-->
                </form>
                
                <!-- 
                <dt>寻找你MSN联系人_不是这个</dt>
                <dd>MSN帐号 : _不是这个<input type="text" class="input-text" id="" name=""/></dd>
                <dd>MSN密码 : _不是这个<input type="text" class="input-text" id="" name=""/></dd>
                 -->
                 
                 <form target="loadingf" class="autoSubmit" method="post" id="webmail" action="${applicationScope.urlInvite}/regGudDpt.do" >
                     <input type="hidden" name="rxurl" value="${rurl}"/>
                     <input type="hidden" name="oxurl" value="${ourl}"/>
                     <input type="hidden" name="regss" value="shortform" />
                     <input type="hidden" name="appId" value=""/>
                     <input type="hidden" name="url" value=""/>
                     <input type="hidden" name="app_html" value=""/>
                     <input type="hidden" name="inv_id" value=""/>
                     <input type="hidden" name="header_type" value="${isappguide}"/>
                     <input type="hidden" name="ifs" value="_AAA"/>
                     
                        <dd>
                         <label for="">邮箱地址 :</label>
                         <input id="email_num" type="text" class="input-text" name="uAccount" rule="blank" value="${mailhead}" schema="xn_noempty:demo@mail.com;lc_noemail" />
                         <span class="atsign">&#64;</span>
                         <select id="email_type" name="importer" class="select-short" rule="blank" schema="xn_noempty:请选择邮箱" >
                             <option value="">请选择邮箱</option>
                             <option value="@163.com" <%="@163.com".equals(request.getAttribute("mailtail"))?"selected=\"selected\"":""%>>163.com</option> 
                             <option value="@126.com" <%="@126.com".equals(request.getAttribute("mailtail"))?"selected=\"selected\"":""%>>126.com</option>
                             <option value="@hotmail.com" <%="@hotmail.com".equals(request.getAttribute("mailtail"))?"selected=\"selected\"":""%>>hotmail.com</option>
                             <option value="@sina.com" <%="@sina.com".equals(request.getAttribute("mailtail"))?"selected=\"selected\"":""%>>sina.com</option>
                             <option value="@yahoo.com.cn" <%="@yahoo.com.cn".equals(request.getAttribute("mailtail"))?"selected=\"selected\"":""%>>yahoo.com.cn</option>
                             <option value="@yahoo.cn" <%="@yahoo.cn".equals(request.getAttribute("mailtail"))?"selected=\"selected\"":""%>>yahoo.cn</option>
                             <option value="@yahoo.com" <%="@yahoo.com".equals(request.getAttribute("mailtail"))?"selected=\"selected\"":""%>>yahoo.com</option>
                             <option value="@live.cn" <%="@live.cn".equals(request.getAttribute("mailtail"))?"selected=\"selected\"":""%>>live.cn</option>
                             <option value="@live.com" <%="@live.com".equals(request.getAttribute("mailtail"))?"selected=\"selected\"":""%>>live.com</option>
                             <option value="@gmail.com"  <%="@gmail.com".equals(request.getAttribute("mailtail"))?"selected=\"selected\"":""%>>gmail.com</option>
                             <option value="@msn.com" <%="@msn.com".equals(request.getAttribute("mailtail"))?"selected=\"selected\"":""%>>msn.com</option>
                             <option value="@msn.cn" <%="@msn.cn".equals(request.getAttribute("mailtail"))?"selected=\"selected\"":""%>>msn.cn</option>
                             <option value="@xnmsn.cn" <%="@xnmsn.cn".equals(request.getAttribute("mailtail"))?"selected=\"selected\"":""%>>xnmsn.cn</option>
                         </select>
                        </dd>
                     
                        <dd>
                         <label for="">邮箱密码 :</label>
                         <input id="email_pwd" type="password" name="pwd" class="input-text" rule="blank" schema="xn_noempty"/>
                         <!-- <span class="description">${domainIdentified["siteName"]}网不会记录你的密码，请放心使用。</span> -->
                        </dd>                  
                     <div class="btn-box">
                         <input id="submit_email" class="input-submit" type="button" value="搜索好友"/>
                         <img id="load_img_email" class="loadingimg" src="${applicationScope.urlStatic}/img/indicator.gif" />
                         <a id="skip_btn_email" href="imports" class="skip">跳过</a>
                         <div class="register-tips">
                            <div class="arrow"></div>
                            <p>${domainIdentified["siteName"]}网不会记录你的密码，<br />请放心使用。</p>
                         </div>
                     </div>
                 </form>
            </dl>
            
        </div>
    </div><!--user-data end-->
    </div><!--content end-->
	
	<div id="container"  style="display:none">
		
		<div id="opi" class="page-wrapper clearfix">
			<div class="full-page-holder">
				<div class="full-page clearfix">
					<div class="compatible">
						<div id="content">
						
							<div class="new-guide">
								<h3 class="welcome">欢迎来到${domainIdentified["siteName"]}网，<%=(String)request.getAttribute("user_name") %></h3>
								<div class="personal-info">
									<h4>快速寻找${domainIdentified["siteName"]}网上的好友</h4>
									<p class="description">你可能有很多朋友已经在这里了，搜索你的MSN或邮箱帐号联系人是最快找到他们的方法。</p>
									
									
								</div>
							</div>
						
						</div>
					</div>
				</div>
			</div>
		</div>
	<c:set var="hideFooterLink" value="true" />
    <c:set var="noWebPager" value="true" />
	</div>
	<%@ include file="/inc/footer-guide-flow.inc" %>
	<img width="0" height="0" src="http://guide.${domainIdentified["sys.domain"]}/xtrs?nur=rggp5v0a0<%= (new java.util.Random()).nextFloat()%>"/>
	<iframe id = "loadingf" name="loadingf" src="http://www.renren.com/ajaxProxy.html" width="1" height="1" style="visibility:hidden">
	</iframe>

<!-- 好吧, 脚本就放在这里 -->
<!-- 组件 -->
<script type="text/javascript" src="http://s.xnimg.cn/jspro/xn.ui.pageTools.js?ver=$revxxx$"></script>

<!-- 特定于页面的代码 -->
<script type="text/javascript" src="http://s.xnimg.cn/jspro/guide/xn.page.guide.importMSNMaill-v1.2.js?ver=$revxxx$"></script>
</body>
</html>
