<%@ page contentType="text/html;charset=UTF-8" session="false" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
        <meta http-equiv="X-UA-Compatible" content="IE=EmulateIE7" />
        <meta name="Description" content="人人网（校内）是一个真实的社交网络，联络你和你周围的朋友。加入校内你可以:联络朋友，了解他们的最新动态；和朋友分享相片、音乐和电影；找到老同学，结识新朋友；用照片和日志记录生活,展示自我。" />
        <meta name="Keywords" content="renren,人人,人人网,Xiaonei,校内,校内,大学,同学,同事,白领,个人主页,博客,相册,群组,社区,交友,聊天,音乐,视频,校园,应用" />
        <link href="${applicationScope.urlSmallStatic}/a2281/csspro/base/layout.css" rel="stylesheet" type="text/css" media="all" />
        <link href="${applicationScope.urlSmallStatic}/favicon.ico" rel="shortcut icon" type="image/x-icon" />
        <link href="${applicationScope.urlSmallStatic}/a2223/csspro/module/app_ad.css" rel="stylesheet" type="text/css" media="all" />
        <!--[if lte IE 6]><style type="text/css" media="screen">/* <![CDATA[ */ @import url(http://s.xnimg.cn/127667/csspro/patch/ie6.css); /* ]]> */</style><![endif]--><!--[if gte IE 7]><style type="text/css" media="screen">/* <![CDATA[ */ @import url(http://s.xnimg.cn/127667/csspro/patch/ie7.css); /* ]]> */</style><![endif]--><!-- <link href="http://s.xnimg.cn/111894/css/appspro.css" rel="stylesheet"
        type="text/css" media="all" /> --><!-- <link href="http://s.xnimg.cn/116446/css/dev-base.css" rel="stylesheet" -->
	 <script type="text/javascript">
            function clearEmail(input){
                if (input && !clearEmail.isFirstClick) {
                    input.value = "";
                    clearEmail.isFirstClick = true;
                }
            }
            
            function checkForm(){
                var email = document.getElementById("email");
                if (email && email.value == "请输入您的邮箱或手机号") {
                    email.value = "";
                }
                return true;
            }
            function cancel() {
            	var url = "${requestScope.ab.callbackUrl}";
				window.location.href=url;
			}
        </script>
	
	<title>人人网，中国最大实名制SNS社交网络。加入人人网，找到老同学，结识新朋友。 </title>
    </head>
    <body class="index">
      <%@ include file="/inc/header-not-login.inc" %> 
    <div id="opi" class="page-wrapper clearfix">
        <div class="full-page-holder">
            <div class="full-page clearfix">
                <div class="dialog_content">
                    <div style="font-size: 12px; text-align: center;">
					<%@ include file="/inc/noscript.inc" %>
						<%@ include file="/inc/errors.inc" %>
                        <div class="app-connect">		  
                            <div class="connectBox">
                            <p class="mb30">
                                   		通过人人连接
							<a target="_blank" href="${applicationScope.urlApp}/apps/application.do?id=${appId}">${appName}</a>，在这个网站上与好友互动，与好友在人人网上分享新鲜事。这个网站能自动发送你的动态信息到你的人人网主页上。
                             </p>   
                                <div class="connect-pic">
								<c:if test="${!empty appLogo}">
									 <img class="partner-logo" src="${appLogo}"/>
								</c:if>
                                    <img class="partner" src="${applicationScope.urlStatic}/imgpro/avatar/d_default_girl_50.gif?ver=$revxxx$"/><img class="myself" src="${applicationScope.urlStatic}/imgpro/avatar/d_default_girl_50.gif?ver=$revxxx$"/>
                                </div>
                            </div>
                <div class="connectBox2">
               <form method="post" action="${requestScope.passportLoginUrl}" focus="email" id="loginForm" onsubmit="return checkForm();">
			 <div class="connect-login mt20">
			<p>
                                            <label for="">
                                                人人网帐号:
                                            </label>
                                        </p>
                                        <p>
                                            <input type="text" tabindex="1" onfocus="clearEmail(this);" value="请输入您的邮箱或手机号" name="email" class="input-text" id="email"/>
                                        </p>

                                        <p>
                                            <label for="">
                                                密码:
                                            </label>
                                        </p>
                                        <p>
                                            <input type="password" tabindex="2" name="password" class="input-text" id="password"/>
                                        </p>

			<p class="formbuttons">
			    <input type="hidden" name="domain" value="renren.com" />
			    <input type="hidden" name="origURL" value="${requestScope.origURL}" />
				<input type="hidden" name="formName" value="${requestScope.formName}" />
				<input type="hidden" name="method" value="${requestScope.method}" />
				<input type="hidden" name="api_key" value="${requestScope.api_key}" />
				<input type="hidden" name="v" value="${requestScope.api_version}" />
				<input type="hidden" name="auth_token" value="${auth_token}"/>
				<input type="hidden" name="context" value="${requestScope.context}"/>
			</p>
			 </div>
			 <p class="grayinfo"> 如果继续,即表示你允许${appName}访问你的个人信息等，并且同意遵守<a href="http://renren.com/info/agreement.jsp" target="_blank">人人网服务条款</a>，使用${appName}，即表示你同意${appName}的相关服务条款。
              </p>
			   <p class="action clearfix">
                  <span><a href="http://wwv.renren.com/xn.do?ss=10730&amp;rt=15&amp;b=31151" target="_blank">注册人人网</a>(renren.com) </span>
                    <input type="button"  value="取消" tabindex="4" class="input-submit gray" onclick="cancel();"/><input type="submit" value="连接" tabindex="3" class="input-submit"/>
               </p>           
		</form>
            </div>
                </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
<%@ include file="/inc/footer-noad.inc" %>
</body>
</html>
