<%@ page pageEncoding="UTF-8" %>
<%@ taglib uri="http://jakarta.apache.org/taglibs/string-1.1" prefix="str" %>
<c:choose>
	<c:when test="${requestScope.visiter.loginCount <= 6}">
		<div id="sidebar">
			<c:if test="${!empty requestScope.host}">
				<div id="mymenu">
					<h3>我的菜单:</h3>
					<ul>
						<li id="myhome"><a href="${applicationScope.urlMain}/profile.do?id=${requestScope.hostid}" class="hasedit">我的页面</a><a href="${applicationScope.urlMain}/BasicInfo.do" class="edit">编辑</a></li>						
						<li id="myfriend"><a href="${applicationScope.urlFriend}/myfriendlistx.do">我的好友</a></li>
						<c:if test="${visiter.status < 2}">
							<li id="myclass"><a href="${applicationScope.urlClass}/myclasses.do">我的班级</a></li>
						</c:if>
						<li id="myblog"><a href="${applicationScope.urlBlog}/MyBlog.do" class="hasedit">我的日志</a><a href="${applicationScope.urlBlog}/NewEntry.do" class="edit">发表</a></li>
						<li id="myphoto"><a href="${applicationScope.urlPhoto}/myalbum.do" class="hasedit">我的相册</a><a href="#" onclick="javascript:upload()" class="edit">上传</a></li>
						<c:if test="${visiter.status < 2}">						
							<li id="mygroup"><a href="${applicationScope.urlGroup}/MyThreadList.do">我的群组<c:if test="${requestScope.visiter.source == 5}">/圈子</c:if></a></li>
						</c:if>
						<li id="mymessage"><a href="${applicationScope.urlMsg}/GetMessageInList.do"><c:if test="${requestScope.visiter.newMessageCount>0}"><span class="new">[${requestScope.visiter.newMessageCount}新]</span></c:if>站内信件</a></li>
						<li id="myaccount"><a href="${applicationScope.urlMain}/privacyhome.do">帐号信息</a></li>
					</ul>
				</div>
			</c:if>
		</div>	
	</c:when>
	<c:otherwise>
		<div id="sidebar">
			<c:choose>
				<c:when test="${empty requestScope.host}">
					<div id="loginpanel">
						<h3>用户登录:</h3>
						<form method="post" id="loginForm"  action="${applicationScope.urlLogin}/Login.do" onsubmit="getEl('email').value=Trim(getEl('email').value);" focus="email">
							<p>
								<label for="email">登录帐号:</label>
								<input type="text" name="email" class="inputtext" id="email" tabindex="1" />
							</p>
							<p>
								<label for="password">密码:</label>
								<input type="password" id="password" name="password" class="inputtext" tabindex="2" />
							</p>
							<p>
								<label for="autoLogin" class="labelCheckbox"><input type="checkbox" name="autoLogin" id="autoLogin" value="true" tabindex="3" />下次自动登录</label>
							</p>
							<p>
							  <input type="hidden" name="origURL" value="${requestScope.origURL}" />
								<input type="submit" id="login" name="submit" class="subbutton" value="登录" tabindex="4" />
								<a href="${applicationScope.urlMain}/GetPasswd.do">找回密码</a>
							</p>
						</form>
						<a href="${applicationScope.urlReg}/Register.do" class="portal"><strong>注册</strong>我们期待你的加入</a>
						<a href="${applicationScope.urlMain}/info/Demo.do" class="portal"><strong>页面介绍</strong>先看一下演示。</a>
						<dl id="welcome">
							<dt>在${domainIdentified["siteName"]}你可以:</dt>
							<dd>找到久违的老同学</dd>
							<dd>了解你周围朋友的最新动态</dd>
							<dd>用照片、日志记录你的生活，和朋友们一起分享</dd>
							<dd>和朋友们分享喜欢的音乐、电影、书籍</dd>
						</dl>
					</div>
				</c:when>
				<c:otherwise>
					<div id="mymenu">
						<h3>我的菜单:</h3>
						<ul>
							<li id="myhome"><a href="${applicationScope.urlMain}/profile.do?id=${requestScope.hostid}" class="hasedit">我的页面</a><a href="${applicationScope.urlMain}/BasicInfo.do" class="edit">编辑</a></li>
							<c:if test="${visiter.status < 2}">
								<li id="myclass"><a href="${applicationScope.urlClass}/myclasses.do">我的班级</a></li>
							</c:if>
							<li id="myfriend"><a href="<c:if test="${requestScope.visiter.friendCount<=0}">${applicationScope.urlGuide}</c:if><c:if test="${requestScope.visiter.friendCount>0}">${applicationScope.urlFriend}</c:if>/myfriendlistx.do">我的好友</a></li>
							<li id="myblog"><a href="${applicationScope.urlBlog}/MyBlog.do" class="hasedit">我的日志</a><a href="${applicationScope.urlBlog}/NewEntry.do" class="edit">发表</a></li>
							<li id="myphoto"><a href="${applicationScope.urlPhoto}/myalbum.do" class="hasedit">我的相册</a><a href="#" onclick="javascript:upload()" class="edit">上传</a></li>
							<c:if test="${visiter.status < 2}">
								<li id="mygroup"><a href="${applicationScope.urlGroup}/MyThreadList.do">我的群组</a></li>
							</c:if>
							<li id="mymessage"><a href="${applicationScope.urlMsg}/GetMessageInList.do"><c:if test="${requestScope.visiter.newMessageCount>0}"><span class="new">[${requestScope.visiter.newMessageCount}新]</span></c:if>站内信件</a></li>
				<li id="mymore" onmouseover="$('moremymenu').style.display='block'" onmouseout="$('moremymenu').style.display='none'"><a href="#" class="ppm">更多功能</a>
					<ul id="moremymenu" style="display:none"  onmouseover="$('moremymenu').style.display='block'" onmouseout="$('moremymenu').style.display='none'">
							<li id="myshare"><a href="${applicationScope.urlMain}/share/ShareList.do">我的分享</a></li>					
							<li id="myaccount"><a href="${applicationScope.urlMain}/privacyhome.do">帐号信息</a></li>
					</ul>
				</li>

							<c:if test="${requestScope.visiter.auth >= 5}"><li id="myadmin"><a href="${applicationScope.urlMain}/admin/adminmanage.do" class="sudo">管理页面</a></li></c:if>
							<c:if test="${requestScope.visiter.auth >= 20}"><li id="myadmin"><a href="${applicationScope.urlMain}/AdminList.do" class="sudo">超级管理页面</a></li></c:if>
		
						</ul>
					</div>
				</c:otherwise>
			</c:choose>
		
		<c:if test="${empty cleanpage}">
			<c:if test="${!empty partner && visiter.stage == 20}">
				<div class="block announcement link">
				<h3>合作伙伴</h3>
				<div>
				<p><a target="_blank" href="http://www.kaifulee.com/"><img src="${applicationScope.urlSmallStatic}/img/home/kaifu.gif"/></a></p>
				</div>
				</div>
			</c:if>
		</c:if>
		</div>
	</c:otherwise>
</c:choose>	
