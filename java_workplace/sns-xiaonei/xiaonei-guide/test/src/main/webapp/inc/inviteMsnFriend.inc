<%@ page pageEncoding="UTF-8" %>
<%@ taglib uri="http://jakarta.apache.org/taglibs/string-1.1" prefix="str" %>
<form action="${applicationScope.urlProfile}/msnsend.do" method="post" class="form-invite">
						
<fieldset>
<legend class="nopadding">寻找你的msn好友</legend>
<p>输入你的msn帐号信息，搜索你在${domainIdentified["siteName"]}的朋友。</p>
<p>
	<label>你的MSN帐号:</label>
	<input type="text" value="" id="uAccount" name="uAccount" class="inputtext"/>
</p>
<p>
	<label>你的MSN密码:</label>
	<input type="password" id="pwd" name="pwd" class="inputtext"/>
</p>
<p>
	<input type="submit" onclick="javascript:if(chkForm()) {return true;} else {return false;}" value="搜索你的好友" class="inputsubmit"/>
</p>
<p style="height: 40px; margin-top: 10px;">
	<span class="alttext">我们不会记录你的帐号密码或者未经允许向你的好友发送邮件。</span>
</p>
<h2>搜索你的同学、同事</h2>
<p>
<a href="${applicationScope.urlBrowse}/scs.do" class="moresearch">搜索同学>></a>
<a href="${applicationScope.urlBrowse}/wos.do" class="moresearch">搜索同事>></a></p>
</fieldset>

<input type="hidden" value="" name="isUpload"/>
<input type="hidden" value="find" name="action"/>
</form>
