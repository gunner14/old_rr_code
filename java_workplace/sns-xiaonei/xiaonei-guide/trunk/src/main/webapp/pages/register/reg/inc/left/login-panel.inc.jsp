<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<div class="login-panel">
					<div class="inner">
					<div class="innertop"></div>
						<h3>用户登录:</h3>
						<form method="post" id="loginForm" class="login-form" action="${applicationScope.urlPassport}/PLogin.do" >
							<p class="top">
								<label for="email">帐号:</label>
								<input type="text" name="email" class="input-text" value="" id="email" tabindex="1" />
							</p>
							<p>
								<label for="password">密码:</label>
								<input type="password" id="password" name="password" error="请输入密码" class="input-text" tabindex="2"/>
							</p>
							<p>
								<label for="autoLogin" class="labelCheckbox">
									<input type="checkbox" name="autoLogin" id="autoLogin" value="true"
										tabindex="3" />
									下次自动登录
								</label>
							</p>
							<p>
								<a class="float-right" href="http://${applicationScope.sysdomain}/GetPasswd.do" tabindex="5">找回密码</a>
								<input type="hidden" name="origURL" value="http://${applicationScope.sysdomain}/SysHome.do" />
								<input type="hidden" name="domain" value="${applicationScope.sysdomain}" />
								<input type="submit" id="login"  class="input-submit" value="登录" tabindex="4" />
							</p>
						</form>
						<div class="innerbottom"></div>
					</div>					
				</div>