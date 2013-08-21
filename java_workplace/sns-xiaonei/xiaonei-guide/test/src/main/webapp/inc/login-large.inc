<div class="login-wrap">        
        <div class="login-box">
            <div class="login-panel">
                <div class="yellow-error" id="yellow_error">
									<a class="close" href="javascript:closeError();"></a>
									<p class="wrong">您的用户名和密码不匹配</p>
									<p class="worp">为了帐号安全，已向您的邮箱： <strong id="sendemail"></strong>发送了一封确认信，请通过邮件内链接登录。</p>
									<p class="m-26"><a id="gotoEmail" href="#" target="_blank">打开邮箱查收确认信</a></p>
									<p class="m-26"><a href="javascript:closeError();">重新输入</a></p>
								</div>
								<div class="yellow-error" id="account_stop">
									<a class="close" href="javascript:closeStop();"></a>
									<p class="wrong"></p>
									<p class="center">您的帐号已停止使用，如有疑问请联系<a style="background:none;padding-left:0;" target="_blank" href="http://support.renren.com/helpcenter">客服</a></p>
								</div>
								<div class="yellow-error" id="account_lock">
									<a class="close" href="javascript:closeLock();"></a>
									<p class="wrong">您的帐号由于以下某种原因需要解锁才能登录</p>
									<ol>
										<li>删除过帐号</li>
										<li>长时间没有登录网站</li>
										<li>安全原因</li>
									</ol>
									<p class="center"><a href="${applicationScope.urlSafeCenter}/relive.do">立即解锁</a></p>
								</div>
								<form method="post" id="loginForm" class="login-form" action="${applicationScope.urlPassport}/PLogin.do">
									<p class="top clearfix">
										<label for="email">帐号:</label>
										<span id="errorMessage" class="errors_div" style="display:none;"></span>
										<c:set var="email" value="${_de}" />
										<input type="text" name="email" class="input-text" id="email" tabindex="1" value="${_email}"/>
									</p>
									<p class="pwd clearfix">
										<label for="password">密码:</label>
										<input type="password" id="password" name="password" error="请输入密码" class="input-text" tabindex="2" />
										<a class="forgetPwd" id="forgetPwd" href="http://safe.${applicationScope.domain}/findPass.do" stats="home_findpassword">忘记密码？</a>
									</p>
									<div class="caps-lock-tips" id="capsLockMessage" style="display:none"></div>
									<p id="code" class="code clearfix">
										<label for="code">验证码:</label>
										<input type="code" id="code" name="icode" class="input-text" tabindex="3" />
									</p>
									<p id="codeimg" class="codeimg clearfix">

										<img id="verifyPic_login" src="http://icode.renren.com/getcode.do?t=login&rnd=<%= new java.util.Random().nextInt()%>"/>

										<a href="javascript:refreshCode_login();">换一张</a>
									</p>
									<p class="savepassword clearfix">
										<label title="为了确保您的信息安全，请不要在网吧或者公共机房勾选此项！" for="autoLogin" class="labelCheckbox">
											<input type="checkbox" name="autoLogin" id="autoLogin" value="true" tabindex="4" />下次自动登录
										</label>
										<span class="getpassword" id="getpassword"><a href="http://safe.${applicationScope.domain}/findPass.do" stats="home_findpassword">忘记密码？</a></span>
									</p>
									<p class="bottom">
										<input type="hidden" name="origURL" value="${origURL}" />
										<input type="hidden" name="domain" value="${applicationScope.sysdomain}" />
										<input type="hidden" name="key_id" value="1" />
										<input type="submit" id="login" class="input-submit login-btn" value="登录人人网" tabindex="5"/>
									</p>
								</form>
                <div class="regnow regnow_ad">
                    <input type="button" onclick="window.location='http://wwv.renren.com/xn.do?ss=10113&rt=27&f=http%3A%2F%2Fwww.renren.com%2Fhome%3Fid%3D285105305'" id="regnow" class="input-button login-btn" value="立即注册帐号" tabindex="4" stats="home_quickreg" />
                </div>
				<p class="login_corp login_corp_ad">
					<a href="#" onclick="loginByMSN.openMSNWindow();" id="login_msn">MSN</a>
					<a id="login_360" href="https://openapi.360.cn/oauth2/authorize?client_id=5ddda4458747126a583c5d58716bab4c&response_type=code&redirect_uri=http://www.renren.com/bind/tsz/tszLoginCallBack&scope=basic&display=default">360</a>
					<a id="login_ty" href="https://oauth.api.189.cn/emp/oauth2/authorize?app_id=296961050000000294&response_type=code&redirect_uri=http://www.renren.com/bind/ty/tyLoginCallBack">天翼</a>
				</p>
            </div>
        </div>
        <div class="ad-panel" id="ad100000000068">
            
        </div>
        <script type="text/javascript" src="http://jebe.xnimg.cn/22635/ad_syshome.js"></script>
    </div>
    <div class="quick-wrap">
        <div class="ucando">
            <h3>人人网是一个真实社交网络，加入她你可以：</h3>
            <ul>
                <li>联络朋友，了解他们的最新动态</li>
                <li>用照片和日志记录生活，展示自我</li>
                <li>和朋友分享相片、音乐和电影</li>
                <li>自由、安全地控制个人隐私</li>
                <li>找到老同学结识新朋友</li>
            </ul>            
        </div>
        <ul class="quick-down"> 
            <li>
                <img class="mobile-url" src="http://a.xnimg.cn/a.gif" />
                <span>手机登录</span>
                <a stats="home_mobilelogin" target="_blank" href="http://mobile.renren.com/mobilelink.do?psf=8000201">http://3g.renren.com</a>
            </li>
            <li>
                <img class="mobile-app" src="http://a.xnimg.cn/a.gif" />
                <a href="http://mobile.renren.com/showClient?psf=41004">下载手机客户端</a>
            </li>
            <li>
                <img class="rrdesk-icon" src="http://a.xnimg.cn/a.gif" style="background: url(http://a.xnimg.cn/imgpro/icons/client3.png) left center no-repeat;" />
                <c:set var="ads_random"><%=new java.util.Random().nextInt(6) %></c:set>
                <c:choose>
                <c:when test="${ads_random == 0 }">
                <a stats="home_seeindex" target="_blank" href="http://im.renren.com/desktop/rrsetup-8.exe?word01">下載人人桌面</a> <span>多个帐号同时登录</span>
                </c:when>
                <c:when test="${ads_random == 1 }">
                <a stats="home_seeindex" target="_blank" href="http://im.renren.com/desktop/rrsetup-8.exe?word02">下載人人桌面</a> <span>相册快速下载</span>
                </c:when>
                <c:when test="${ads_random == 2 }">
                <a stats="home_seeindex" target="_blank" href="http://im.renren.com/desktop/rrsetup-8.exe?word03">下載人人桌面</a> <span>19套皮肤免费换</span>
                </c:when>
                <c:when test="${ads_random == 3 }">
                <a stats="home_seeindex" target="_blank" href="http://im.renren.com/desktop/rrsetup-8.exe?word04">下載人人桌面</a> <span>相册快速下载</span>
                </c:when>
                <c:when test="${ads_random == 4 }">
                <a stats="home_seeindex" target="_blank" href="http://im.renren.com/desktop/rrsetup-8.exe?word05">下載人人桌面</a> <span>小窗查看不留痕</span>
                </c:when>
				<c:when test="${ads_random == 5 }">
                <a stats="home_seeindex" target="_blank" href="http://im.renren.com/desktop/rrsetup-8.exe?word06">下載人人桌面</a> <span>相册快速下载</span>
                </c:when>
                <c:otherwise>
                </c:otherwise>
                </c:choose>
            </li>
            <li>
                <img class="rrgame-icon" src="http://a.xnimg.cn/a.gif" />
                <a stats="home_seeindex" target="_blank" href="http://g.renren.com/lobby/rrgamesetup-5.exe?word03">下载游戏助手</a>
                <span>帐号多开</span>
            </li>
        </ul>
    </div>