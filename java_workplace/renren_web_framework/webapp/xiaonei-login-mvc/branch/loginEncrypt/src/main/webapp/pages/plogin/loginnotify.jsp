<%@ page contentType="text/html;charset=UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
        <meta http-equiv="X-UA-Compatible" content="IE=EmulateIE7" />
        <meta name="Description" content="人人网是一个真实的社交网络，联络你和你周围的朋友。加入人人网你可以:联络朋友，了解他们的最新动态；和朋友分享相片、音乐和电影；找到老同学，结识新朋友；用照片和日志记录生活,展示自我。" />
        <meta name="Keywords" content="Xiaonei,人人网,人人网,大学,同学,同事,白领,个人主页,博客,相册,群组,社区,交友,聊天,音乐,视频,校园" />
        <link href="http://s.xnimg.cn/favicon.ico" rel="shortcut icon" type="image/x-icon" />
        <title>登录提示</title>
        <style type="text/css">
            body, div, span, object, frame, h1, h2, h3, h4, h5, h6, p, blockquote, a, code, em, img, q, small, strong, dd, dl, dt, li, ol, ul, fieldset, form, label, table, tbody, tr, th, td, input {margin: 0;padding: 0}
            body {font-size: 12px;font-family: Tahoma, Verdana, STHeiTi, simsun, sans-serif;background:#F0F5F8;}
            a img,:link img,:visited img {border: 0}
            a{color:#005EAC;text-decoration:none;}
			a:hover{text-decoration:underline;}
			.contariner{width:760px; margin:40px auto;}
			.logos img{margin-right:20px;}
			.content h2{color:#000;font-size:16px;font-weight:normal;} 
			.content{border:2px solid #899EB0;padding:40px; background:#FFF no-repeat; margin-top:30px;position:relative;}
			.content .arrow{position:absolute;left:50px;top:-14px;_top:-12px;width:25px;height:14px; background:url(${applicationScope.urlStatic}/imgpro/bg/arrow-merge.png?ver=$revxxx$) no-repeat;}
			.content .intro{margin:30px 0;font-size:14px;line-height:2.0;}
			.content .input-text{ vertical-align:middle;}
			.content label{color:#808080;}
			.content .iknow{ vertical-align:middle; width:220px;height:36px; background-repeat:no-repeat; border:0;cursor:pointer;}
			.continue{display:inline-block; width:131px; height:36px; background:url(${applicationScope.urlStatic}/imgpro/bg/continue.png?ver=$revxxx$) no-repeat; vertical-align:middle;border:0;cursor:pointer;}
         .or{color:#666666; margin:0 10px;}
        </style>
    </head>
    <body>
        <div class="contariner">
            <div class="logos">
                <img src="${applicationScope.urlStatic}/imgpro/bg/logo_renren.png?ver=$revxxx$" /><img src="${applicationScope.urlStatic}/imgpro/bg/logo_kaixin.png?ver=$revxxx$" />
            </div>
            <div class="content">
            	<div class="arrow"></div>
                <h2>您即将使用${requestScope.fromname}网的帐号 <span style="color:#000;font-weight:bolder;">${requestScope.host.name}(${requestScope.host.account})</span> 访问${requestScope.toname}网</h2>
                <p class="intro">
                    人人网与开心网已经实现“互联互通”，您即将使用${requestScope.fromname}网的帐号 <strong>${requestScope.host.account}</strong> 访问${requestScope.toname}网。
                    <br/>
                    如果想使用您已有的${requestScope.toname}网的帐号，请<a href="http://login.${requestScope.to}/Logout.do?origURL=http://login.${requestScope.to}/GLogin.do"><strong>退出后重新登录</strong></a>。想了解更多请<c:if test="${requestScope.to=='renren.com'}"><a href="http://www.renren.com/renren/index-v2.html">点击这里</a>。
                    	</c:if><c:if test="${requestScope.to=='kaixin.com'}"><a href="http://www.kaixin.com/kaixin/index-v2.html">点击这里</a>。
                    	</c:if>
                </p>
                <form method="POST" action="/LN.do" >
                <input type="checkbox" name="known" id="notips" class="input-text" />
                <%
					String origURL = request.getParameter("origURL");
            		if(origURL != null && origURL.trim().length()>0){
            			origURL = com.xiaonei.antispam.AntiSpamUtils.getInstance().htmlSecurityEscape(origURL);
            			request.setAttribute("origURL",origURL);
            		}
                %>
                <input type="hidden" name="origURL" value="${requestScope.origURL}"/>
                <label for="notips">
                    不再出现此类提示
                </label>
				<br />
                <p style="margin-top:30px;">
                	<c:if test="${requestScope.to=='renren.com'}">
                	<input type="button" onclick="window.location.href='http://login.${requestScope.to}/Logout.do?origURL=http://login.${requestScope.to}/GLogin.do'" class="iknow" style="background-image:url(${applicationScope.urlStatic}/imgpro/bg/rr_login.btn.png?ver=$revxxx$);" />
                	</c:if>
                	<c:if test="${requestScope.to=='kaixin.com'}">
                	<input type="button" onclick="window.location.href='http://login.${requestScope.to}/Logout.do?origURL=http://login.${requestScope.to}/GLogin.do'" class="iknow" style="background-image:url(${applicationScope.urlStatic}/imgpro/bg/kx_login.btn.png?ver=$revxxx$);" />
                	</c:if>
					<span class="or">或</span>
					<input type="submit" value="" class="continue" />
			    </p>
               	</form>
            </div>
        </div>
    </body>
</html>