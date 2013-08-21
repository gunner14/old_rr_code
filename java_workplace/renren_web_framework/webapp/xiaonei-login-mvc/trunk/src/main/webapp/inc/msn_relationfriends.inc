<c:if test="${empty msnDisplay}">
<c:set var="msnDisplay" value="none" />
</c:if>
<div style="display: ${msnDisplay};" class="find-friends" id="find-msn">
	<form class="form-holder" method="post" action="${applicationScope.urlProfile}/rltFrdDsp.do">
		<div class="formcontent">
			<p class="intro" id="msn_altmsg">输入你的MSN帐号信息，看看你的MSN好友们有没有注册${domainIdentified["siteName"]}。</p>
			<div id="msn_step1">
				<p>
					<label>你的MSN帐号:</label>
					<input type="text" value="" id="uAccount" name="uAccount" class="inputtext" autocomplete="off" onkeydown="javascript:if(event.keyCode==13){ if(!chkForm()) {return false;}; save_info(); $('msn_step1').style.display='none';$('msn_step2').style.display='inline';$('msn_altmsg').innerHTML='我们不会记录你的帐号密码或者未经允许向你的好友发送邮件。';  $('pwd').focus() ;return false;}"/>
				</p>
				<p>
					<label class="empty">提交:</label>
					<input type="button" onClick="if(!chkForm()) {return false;};save_info();$('msn_step1').style.display='none';$('msn_step2').style.display='inline';$('msn_altmsg').innerHTML='我们不会记录你的帐号密码或者未经允许向你的好友发送邮件。';$('pwd').focus();" value="下一步" class="inputsubmit" id="msn_btn1"/>
				</p>
			</div>
			<div style="display: none;" id="msn_step2">
				<p id="msn_step2">
					<label>你的MSN密码:</label>
					<input type="password" id="pwd" name="pwd" class="inputtext"/>
					</p>
				<p>
					<label class="empty">提交:</label>
					<input type="submit" value="搜索你的好友" class="inputsubmit" id="msn_btn1"/>
					<input type="button" id="msn_btn1" class="inputsubmit gray" value="取消" onclick="$('msn_step1').style.display='block';$('msn_step2').style.display='none';"/>
				</p>
			</div>
			<div id="reqlist_container_div" class="reqbox">
			</div>
		</div>
		<input type=hidden name="importer" value="@hotmail.com">
	</form>
</div>