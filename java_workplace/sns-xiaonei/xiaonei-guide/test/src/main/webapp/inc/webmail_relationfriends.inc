<c:if test="${empty webmailDisplay}">
<c:set var="webmailDisplay" value="none" />
</c:if>
<div style="display: ${webmailDisplay};" class="find-friends" id="find-mail">
	<form class="form-holder" method="post" action="${applicationScope.urlProfile}/rltFrdDsp.do" onsubmit="return chkMSNform()">
		<div class="formcontent">
			<p class="intro">输入你的联系邮箱信息，看看你的邮箱联系人中有没有人注册了${domainIdentified["siteName"]}。</p>
			<p>
				<label>你的email帐号:</label>
				<input type="text" class="inputtext" name="uAccount" id="emailAccount" value="" tabIndex="98"/> @
					<select id="importer_webmail" name="importer" >
						<option value="">请选择邮箱</option>
						<option value="@hotmail.com">hotmail.com</option>
						<option value="@163.com">163.com</option>
						<option value="@126.com">126.com</option>
						<option value="@sina.com">sina.com</option>
						<option value="@yahoo.com.cn">yahoo.com.cn</option>
						<option value="@gmail.com">gmail.com</option>
						<option value="@live.cn">live.cn</option>
						<option value="@live.com">live.com</option>
						<option value="@msn.com">msn.com</option>	
						<option value="@yahoo.cn">yahoo.cn</option>
						<option value="@yahoo.com">yahoo.com</option>
				</select>
			</p>
			<p>
				<label>你的email密码:</label>
				<input type="password" class="inputtext" name="pwd" id="emailpwd" tabIndex="99"/>
			</p>
			<p>
				<label class="empty">提交:</label>
				<input type="submit" class="inputsubmit" value="搜索你的好友"/>
			</p>
			<p>
				<label class="empty">注意:</label>
				<span class="alttext">我们不会记录你的帐号密码或者未经允许向你的好友发送邮件。</span>
			</p>
		</div>
	</form>
</div>