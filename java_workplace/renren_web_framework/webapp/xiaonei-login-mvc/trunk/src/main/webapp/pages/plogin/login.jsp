<%@ page contentType="text/html;charset=UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html" %>
<%@ page import="com.xiaonei.platform.core.opt.OpiConstants"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<%@ include file="/inc/headpro.inc" %>
	<link href="${applicationScope.urlSmallStatic}/csspro/apps/login.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
	<title>人人网，中国最大实名制SNS社交网络。加入人人网，找到老同学，结识新朋友。 </title>
</head>
<body id="registerpage" class="login">
	<c:set var="invite" value="yes" />
	<%@ include file="/inc/header.inc" %>
	<div id="opi" class="page-wrapper clearfix">
		<div class="full-page-holder">
			<div class="full-page">
			<c:if test="${domainIdentified['sys.domain']=='renren.com'}">
			<a href="http://www.renren.com/renren/index.html" style="border:0; margin:0 auto; padding:0 0 10px; display:block; width:900px;" target="_blank">
				<img src="${applicationScope.urlStatic}/imgpro/login/xntrr.png?ver=$revxxx$"/>
			</a>
			</c:if>
				<div class="login-page clearfix">
					<div class="full-login">
						<div class="login-error">
							<%@ include file="/inc/noscript.inc" %>
							<%@ include file="/inc/errors2.inc" %>
							<c:if test="${requestScope.SERSUICIDE}">
                            <div class="errors_div">
                                <div class="inner">
										<h3>你的帐号由于下列某种原因，需解锁才能登录</h3>
										<ol>
											<li>删除过帐号</li>
											<li>长时间没有登录网站</li>
											<li>安全原因</li>
										</ol>
                                        <a class="unlock" href="${applicationScope.urlSafeCenter}/relive.do">立即解锁</a>
                                    </div>
								</div>
							</c:if>
						</div>
						<div class="shadow">
							<div class="login-panel">
									<form method="post" id="loginForm" action="${applicationScope.urlPassport}/PLogin.do" focus="email">
										<h2>登录${domainIdentified["messages.domain.name"]}网</h2>
										<p class="clearfix">
											<label for="email">帐号:</label>
											<%
												String emailTemp = request.getParameter("e");
												if(emailTemp == null || "".equals(emailTemp)){
													String url=request.getAttribute("origURL")!=null?(String)request.getAttribute("origURL"):(String)request.getParameter("origURL");
													if(url != null){
														try{
															String[] origURL=url.split("\\?");
															if(origURL.length>=2){
																for(String p:origURL[1].split("&")){
																	String[] args=p.split("=");
																	if(args.length>=2){
																		if("e".equals(args[0]))
																			emailTemp = new String(new sun.misc.BASE64Decoder().decodeBuffer(p.substring(p.indexOf("=")+1)));
																			emailTemp=com.xiaonei.antispam.AntiSpamUtils.getInstance().htmlSecurityEscape(emailTemp.trim());
																	}																
																}
															}
														}catch (Exception e){
														}
													}
												}
												else{
													emailTemp = new String(new sun.misc.BASE64Decoder().decodeBuffer(emailTemp));
													emailTemp=com.xiaonei.antispam.AntiSpamUtils.getInstance().htmlSecurityEscape(emailTemp.trim());
												}
												if(emailTemp != null && com.xiaonei.platform.core.utility.StringFormat.isValidEmailAddr(emailTemp)){
													request.setAttribute("loginemail",emailTemp);											
												%>
												<html:text property="email" styleClass="input-text" styleId="email" value="${loginemail}" tabindex="1" />
												<%
												}else{
												%>
											<c:choose>
												<c:when test="${empty requestScope.user}">
													<html:text property="email" styleClass="input-text" styleId="email" value="${_de}" tabindex="1" />
												</c:when>
												<c:otherwise>
													<html:text property="email" styleClass="input-text" styleId="email" value="${requestScope.user.email}" tabindex="1" />
												</c:otherwise>
											</c:choose>
											<%
												}
											%>
										</p>
										<p class="clearfix">
											<label for="password">密码:</label>
											<input type="password" id="password" name="password" class="input-text" tabindex="2" />
										</p>
										<%-- 用于临时验证需求的web代码 删了怪可惜的呵呵 --%>
										<c:if test="${requestScope.needValidate}">
										<div class="authcode" style="padding-left:85px">
											<div class="authcode-img">
												<img src="http://icode.renren.com/getcode.do?t=vlcode&rnd=1252308494766"  id="verifyPic" class="validate-num"/>
												<a href="javascript:$('verifyPic').src+='&'+new Date();void(0);">看不清换一张?</a>
											</div>
											<div class="authcode-text">
												<label for="code">验证码:</label>
												<div class="input_wrap">
													<input type="text" size="5" class="input-text" tabindex="3" name="icode"/>
												</div>
											</div>
										</div>
										</c:if>
										<p class="right">
											<span class="float-right">
												<a tabindex="5" href='http://safe.${applicationScope.domain}/findPass.do'>取回密码</a>
											</span>
											<label for="autoLogin">
												<input type="checkbox" name="autoLogin" id="autoLogin" value="true" tabindex="4" class="input-checkbox" />
												下次自动登录
											</label>
										</p>
										<p class="right">
										    <%
                                        		String origURL = request.getParameter("origURL");
                                        		if(origURL != null && origURL.trim().length()>0){
                                        		    if(!com.xiaonei.antispam.AntiSpamUtils.getInstance().isSafeUrl(origURL)){
                                        		        origURL="";
                                        		    }
                                        			request.setAttribute("origURL",origURL);
                                        		}                                        		
                                        		String domain = request.getParameter("domain");
                                        		if(domain != null && domain.trim().length()>0){
                                        		    if(!com.xiaonei.antispam.AntiSpamUtils.getInstance().isSafeUrl(domain)){
                                        		        domain=OpiConstants.getCurrentDomain();;
                                        		    }
                                        			request.setAttribute("domain",domain);
                                        		}   
                                        	%>
											<input type="hidden" name="origURL" value="${requestScope.origURL}" />
											<c:if test="${empty requestScope.domain}">
												<input type="hidden" name="domain" value="${domainIdentified['sys.domain']}" />
											</c:if>
											<c:if test="${!empty requestScope.domain}">
												<input type="hidden" name="domain" value="${requestScope.domain}" />
											</c:if>
											<input type="hidden" name="formName" value="${requestScope.formName}" />
											<input type="hidden" name="method" value="${requestScope.method}" />
											<input type="hidden" name="isplogin" value="true" />
											<input type="submit" id="login" tabindex="4" name="submit" class="input-submit large" value="登录" />
										</p>
										<div class="separator"></div>
										<p class="no-account">还没有开通你的${domainIdentified["messages.domain.name"]}网帐号？ <a href="http://wwv.${applicationScope.sysdomain}/xn.do?ss=10113&rt=27" class="reg-now">立即注册</a></p>
									</form>
							</div>
						</div>
					</div>
					<c:choose>
						<c:when test="${domainIdentified['sys.domain']=='renren.com'}">
					<div class="float-right" style="margin-top:60px;"><img src="http://s.xnimg.cn/imgpro/login/show.gif" width="475" height="201" /></div>
						</c:when>
						<c:when test="${domainIdentified['sys.domain']=='kaixin.com'}">
					<div class="float-right" style="margin-top:60px;"><img src="http://s.xnimg.cn/imgpro/login/show-kx.gif" width="475" height="201" /></div>
						</c:when>
					</c:choose>
				</div>
			</div>
		</div>
	</div>
	<script>
	XN.dom.ready(function(){
		var tip = '用户邮箱/手机号/用户名';
		var email = $( 'email' );
		if ( email.value == '' || email.value == tip ){
			new XN.FORM.inputHelper( 'email' ).setDefaultValue( tip );        	
        	XN.event.addEvent( 'email' , 'click' ,function(){
        		if ( $( 'email' ).value == tip ){
        			$( 'email' ).value = '';
        		}
        	});
        	XN.event.addEvent( 'email' , 'blur' , function(){
        		if ( $( 'email' ).value == tip ){
        			$( 'email' ).style.color = '#888';
        		}
        	});
        	$('email').style.color = '#333';
        	$('email').focus();
        	setTimeout(function(){
        		$('email').value = tip;
        		$('email').select();
        	},0);
		}
	});
	//收藏和设为首页
	(function(){
		if ( !window.XN ) XN = {};
		if ( !XN.Browser ) XN.Browser = XN.BORWSER = {};
		XN.Browser.addHomePage = function(url){
			if(!!(window.attachEvent && !window.opera)){
					document.body.style.behavior = 'url(#default#homepage)';
					document.body.setHomePage(url);
				}else{
				  if(window.clipboardData && clipboardData.setData){
				clipboardData.setData("text", url);
			  }else{
				alert( '您的浏览器不允许脚本访问剪切板，请手动设置~' );
				return;
			  }
			  alert('网址已经拷贝到剪切板,请您打开浏览器的选项,\n把地址粘到主页选项中即可~');
			}
			return true;		
		};
		XN.Browser.addBookMark = function(url,title){
			var ctrl = (navigator.userAgent.toLowerCase()).indexOf('mac') != -1 ? 'Command/Cmd' : 'CTRL';					
			try{
					window.external.addFavorite(url,title || '${domainIdentified["messages.domain.name"]}网-因为真实,所以精彩');
			}catch(e){
				try{
					window.sidebar.addPanel(url,title || '${domainIdentified["messages.domain.name"]}网-因为真实,所以精彩');
				}catch(e){
					alert('您可以尝试通过快捷键' + ctrl + ' + D 添加书签~');
				}
			}		
		};
	})();
	</script>
	<%@ include file="/inc/footer-noad.inc" %>
	<%@ include file="/pages/reg/active_stat.jsp" %>
</body>
</html>
