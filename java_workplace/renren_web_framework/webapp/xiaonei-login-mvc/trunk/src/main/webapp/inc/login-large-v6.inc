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
				<p class="center">您的帐号已停止使用，如有疑问请联系<a style="background:none;padding-left:0;" target="_blank" href="http://help.renren.com/#http://help.renren.com/support/contomvice?pid=2&selection={couId:193,proId:342,cityId:1000375}">客服</a></p>
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
				<!--	<iframe name="iframelogin" id="iframelogin" width="100%" height="0" frameborder="0" allowtransparency="true" scrolling="no"></iframe>-->
			<form method="post" id="loginForm" class="login-form" action="${applicationScope.urlPassport}/PLogin.do">
				<span id="errorMessage" class="errors_div" style="display:none;"></span>
				<dl class="top clearfix">
					<dt><label for="email">帐号:</label></dt>
					<dd>
						<c:set var="email" value="${_de}" />
						<input type="text" name="email" class="input-text" id="email" tabindex="1" value="${_email}" />
					</dd>
				</dl>
				<dl class="pwd clearfix">
					<dt><label for="password">密码:</label></dt>
					<dd>
						<input type="password" id="password" name="password" error="请输入密码" class="input-text" tabindex="2" autocomplete="off"/>
						<label class="pwdtip" id="pwdTip" for="password">请输入密码</label>
						<a class="forgetPwd" id="forgetPwd" href="http://safe.${applicationScope.domain}/findPass.do" stats="home_findpassword">忘记密码？</a>
					</dd>
				</dl>
				<div class="caps-lock-tips" id="capsLockMessage" style="display:none"></div>
				<dl class="savepassword clearfix">
					<dt>
						<label title="为了确保您的信息安全，请不要在网吧或者公共机房勾选此项！" for="autoLogin" class="labelCheckbox">
							<input type="checkbox" name="autoLogin" id="autoLogin" value="true" tabindex="4" />下次自动登录
						</label>
					</dt>
					<dd>
						<span class="getpassword" id="getpassword"><a href="http://safe.${applicationScope.domain}/findPass.do" stats="home_findpassword">忘记密码？</a></span>
					</dd>
				</dl>
				<dl id="codeimg" class="codeimg clearfix">
					<dt></dt>
					<dd>

					<img id="verifyPic_login" src="http://icode.renren.com/getcode.do?t=web_login&rnd=Math.random()"/>

					<a href="javascript:refreshCode_login();">换一张</a>
					</dd>
				</dl>
				<dl id="code" class="code clearfix">
					<dt><label for="code">验证码:</label></dt>
					<dd>
						<input type="text" id="icode" name="icode" class="input-text" tabindex="3" autocomplete="off" />
						<label class="codetip" id="codeTip" for="icode">请输入验证码</label>
					</dd>
				</dl>
				<dl class="bottom">
					<input type="hidden" name="origURL" value="${origURL}" />
					<input type="hidden" name="domain" value="${applicationScope.sysdomain}" />
					<input type="hidden" name="key_id" value="1" />
					<input type="hidden" name="captcha_type" id="captcha_type" value="web_login" />
					<input type="submit" id="login" class="input-submit login-btn" stats="loginPage_bigAD_login" value="登录人人网" tabindex="5"/>
				</dl>
			</form>
			<div class="regnow">
				<input type="button" onclick="window.location='${regDomain}${regUrl}'" id="regnow"  class="input-button login-btn" value="立即注册帐号" tabindex="6"  stats="loginPage_bigAD_signUp" />
			</div>
			<div class="largeStudentReg">
				<input type="button" id ="newStudentReg"  onclick="window.location='http://wwv.renren.com/xn.do?ss=10810&rt=1'" class="input-button login-btn" value="大一新生快速入口" tabindex="7"/>	
			</div>
			<dl class="login_corp" style="margin-bottom:10px;">
				<dt>使用其它帐号登录：</dt>
				<dd><a href="#" onclick="loginByMSN.openMSNWindow();" id="login_msn" stats="loginPage_MSN_link">MSN</a></dd>
				<dd><a id="login_360" href="https://openapi.360.cn/oauth2/authorize?client_id=5ddda4458747126a583c5d58716bab4c&response_type=code&redirect_uri=http://www.renren.com/bind/tsz/tszLoginCallBack&scope=basic&display=default" stats="loginPage_360_link">360</a></dd>
				<dd class="login_list"><a id="login_tianyi" href="https://oauth.api.189.cn/emp/oauth2/authorize?app_id=296961050000000294&response_type=code&redirect_uri=http://www.renren.com/bind/ty/tyLoginCallBack" stats="loginPage_tianyi_link">天翼</a></dd>
			</dl>
		</div>
        </div>
        <div class="ad-panel" id="ad100000000068" stats="loginPage_bigAD_link">
            
        </div>
        <script type="text/javascript" src="http://jebe.xnimg.cn/41801/ad_syshome.js"></script>
    </div>
    <style type="text/css">
		.login-recommend{
			margin:0 auto;
			width:980px;
			border:none;
		}
		.login-recommend dl{
			float:left;margin-right:20px;
		}
		.login-recommend .login-download{
			float:left;
			width:290px;
			margin:25px 0 0;
			border-left:1px solid #DDD;
			padding:10px 0 10px 20px;
		}
		.login-recommend .login-download a{
			width:100px;
			margin:0;
			padding-bottom:6px;
		}
		.login-download a.symbian{
			padding-left:25px;
			background-position: 3px -254px;
		}
		.login-download a.windows{
			width:130px;
		}
		.login-download a.iphone{
			width:130px;
		}
	</style>
	<div class="login-recommend clearfix">
		<dl>
			<dt>站内功能推荐</dt>
			<dd class="clearfix">
				<c:set var="ads_play_random"><%=new java.util.Random().nextInt(4) %></c:set>
				<c:choose>
					<c:when test="${0==ads_play_random}">
						<div class="recommend-pic">
							<a href="http://im.renren.com/?subver=58&p01" target="_blank"><img src="http://a.xnimg.cn/n/apps/login/v6/res/rrdesk.png" alt="人人桌面" /></a>
						</div>
						<div class="recommend-intr">
							<h4><a style="zoom:1" href="http://im.renren.com/desktop/rrsetup-58.exe?word01">人人桌面3.5Beta</a> <img src="${applicationScope.urlStatic}/imgpro/icons/new-bullet.gif" class="new-table-img" /></h4>
							<p>不开网页上人人<br />唯一隐身登录器</p>
							<a href="http://im.renren.com/desktop/rrsetup-58.exe?b01" class="login-gray-btn" stats="loginPage_funcRecomm_zhuomian">下 载</a>
						</div>
					</c:when>
					<c:when test="${1==ads_play_random}">
						<div class="recommend-pic">
							<a href="http://im.renren.com/?subver=58&p02" target="_blank"><img src="http://a.xnimg.cn/n/apps/login/v6/res/rrdesk.png" alt="人人桌面" /></a>
						</div>
						<div class="recommend-intr">
							<h4><a style="zoom:1" href="http://im.renren.com/desktop/rrsetup-58.exe?word02">人人桌面3.5Beta</a> <img src="${applicationScope.urlStatic}/imgpro/icons/new-bullet.gif" class="new-table-img" /></h4>
							<p>个性回复签名随意加 <br />———— ☜沙发王☞ </p>
							<a href="http://im.renren.com/desktop/rrsetup-58.exe?b02" class="login-gray-btn" stats="loginPage_funcRecomm_zhuomian">下 载</a>
						</div>
					</c:when>
					<c:when test="${2==ads_play_random}">
						<div class="recommend-pic">
							<a href="http://im.renren.com/?subver=58&p03" target="_blank"><img src="http://a.xnimg.cn/n/apps/login/v6/res/rrdesk.png" alt="人人桌面" /></a>
						</div>
						<div class="recommend-intr">
							<h4><a style="zoom:1" href="http://im.renren.com/desktop/rrsetup-58.exe?word03">人人桌面3.5Beta</a> <img src="${applicationScope.urlStatic}/imgpro/icons/new-bullet.gif" class="new-table-img" /></h4>
							<p>新＋视频聊天功能<br />与好友面对面！</p>
							<a href="http://im.renren.com/desktop/rrsetup-58.exe?b03" class="login-gray-btn" stats="loginPage_funcRecomm_zhuomian">下 载</a>
						</div>
					</c:when>
					<c:when test="${3==ads_play_random}">
						<div class="recommend-pic">
							<a href="http://im.renren.com/?subver=58&p04" target="_blank"><img src="http://a.xnimg.cn/n/apps/login/v6/res/rrdesk.png" alt="人人桌面" /></a>
						</div>
						<div class="recommend-intr">
							<h4><a style="zoom:1" href="http://im.renren.com/desktop/rrsetup-58.exe?word04">人人桌面3.5Beta</a> <img src="${applicationScope.urlStatic}/imgpro/icons/new-bullet.gif" class="new-table-img" /></h4>
							<p>相片全屏查看 <br />任你下载！</p>
							<a href="http://im.renren.com/desktop/rrsetup-58.exe?b04" class="login-gray-btn" stats="loginPage_funcRecomm_zhuomian">下 载</a>
						</div>
					</c:when>
					<c:otherwise></c:otherwise>
				</c:choose>
			</dd>
			<dd class="clearfix">
				<div class="recommend-pic">
					<a href="http://music.renren.com/fm?from=login"><img src="http://a.xnimg.cn/n/apps/login/v6/res/rrmusic.png" alt="人人电台" /></a>
				</div>
				<div class="recommend-intr">
					<h4>人人电台</h4>
					<p>用音乐灌溉你的生活！</p>
					<a href="http://music.renren.com/fm?from=login" class="login-gray-btn" stats="loginPage_funcRecomm_radio">去听听</a>
				</div>
			</dd>
			<dd class="last clearfix">
				<div class="recommend-pic">
					<a href="http://g.renren.com/?subver=32&P01" target="_blank"><img src="http://a.xnimg.cn/n/apps/login/v6/res/rrgame.png" alt="人人游戏" /></a>
				</div>
				<div class="recommend-intr">
					<h4><a style="zoom:1" href="http://g.renren.com/?subver=32&word01" target="_blank">人人游戏大厅2.2</a></h4>
					<p>帐号多开、定时提醒<br />批量加游戏好友</p>
					<a href="http://g.renren.com/lobby/rrgamesetup-32.exe?b01" class="login-gray-btn" stats="loginPage_funcRecomm_gameCenter">下载</a>
				</div>
			</dd>
		</dl>
		<div class="login-download clearfix">
			<a href="http://mobile.renren.com/showClient?v=android&psf=42045" class="android" stats="loginPage_download_android" >Android客户端</a>
			<a href="http://mobile.renren.com/showClient?v=iphone&psf=42045" class="iphone" stats="loginPage_download_iphone">iPhone/iPad客户端</a>
			<a href="http://mobile.renren.com/showClient?v=symbian&psf=42045" class="symbian" stats="loginPage_download_symbian">Symbian客户端</a>
			<a href="http://mobile.renren.com/showClient?v=windowPhone&psf=42045" class="windows" stats="loginPage_download_winPhone">Windows Phone客户端</a>
		</div>
	</div>