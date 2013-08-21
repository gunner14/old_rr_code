<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %> 
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html" %>

<form method="post" id="regform"  name="regform" action="http://reg.renren.com/s-c-i-reg.do">
	<input type="hidden" id='msgId' value=''/>
	<input type="hidden" name="fromUrl" id="fromUrl" />
	<input type="hidden" name="iu" id="iu" />
	<input type="hidden" name="ic" id="ic" />
	<input type="hidden" name="uuid" id="uuid" />
	<input type="hidden" name="ss" id="ss"  />
	<input type="hidden" name="action_id" id="action_id"  />
	<%@ include file="page_reg.inc" %>
	<input type="hidden" name="accType" id="accType" value="${accType}" />
	<input type="hidden" name="g" id="g" />
	<input type="hidden" name="b" id="b" />
<dl id="d_email">
<dt><label for="regEmail">电子邮箱:</label></dt>
<dd><input type="text" name="regEmail" id="regEmail" class="inputtext" /><span id="dmmg"></span><span id="regEmail_error_span" class="box-error"><b id="regEmail_error_span_i"></b></span><p class="tabsort">你还可以选择 <a href="javascript:;" id="xid_reg_handle" tabindex="-1">帐号</a> 注册或 <a href="http://wwv.renren.com/xn.do?ss=10131&rt=1"  tabindex="-1">手机号</a> 注册</p></dd>
</dl>
<dl id="d_xid" style="display:none">
<dt><label for="nicknameId">输入帐号:</label></dt>
<dd><input type="text" name="nicknameId" id="nicknameId" class="inputtext" /><span id="mg"></span><span id="nicknameId_error_span" class="box-error"><b id="nicknameId_error_span_i"></b></span><p class="tabsort"> <a href="javascript:;" id="regmail_reg_handle" tabindex="-1">返回邮箱</a> 注册或 <a href="http://wwv.renren.com/xn.do?ss=10131&rt=1" tabindex="-1">手机号</a> 注册</p></dd>
</dl>
<dl>
<dt><label for="pwd">设置密码:</label></dt>
<dd><input type="password" maxlength="20" name="pwd" id="pwd" class="inputtext" /><span id="mmg"></span><span id="pwd_error_span" class="box-error"><b id="pwd_error_span_i"></b></span></dd>
</dl>
<dl class="dl_pad">
<dt><label for="name">真实姓名:</label></dt>
<dd><input type="text" name="name" id="name" class="inputtext" /><span id="xmg"></span><span id="name_error_span" class="box-error"><b id="name_error_span_i"></b></span></dd>
</dl>
<dl class="dl_gender">
<dt><label>性别:</label></dt>
<dd><label for="male"><input type="radio" name="gender" id="male" value="男生" />男</label><label for="female"><input type="radio" name="gender" id="female" value="女生" />女</label><span id="gender_error_span" class="box-error"><b id="gender_error_span_i"></b></span></dd>
</dl>
<dl id="p_birthday">
<dt><label>生日:</label></dt>
<dd><select name="birth_year">
<option value="">请选择</option>
<c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
<c:set var="strutsOptionYearComponent_end" value="1900" />
<%@ include file="/inc/strutsOptionBirthdayYearComponent.inc.jsp"%>
</select> 年
<select name="birth_month" >
<option value="">--</option>
<c:set var="strutsOptionYearComponent_begin" value="12" />
<c:set var="strutsOptionYearComponent_end" value="1" />
<%@ include file="/inc/strutsOptionYearComponent.inc"%>
</select> 月
<select name="birth_day">
<option value="">--</option>
<c:set var="strutsOptionYearComponent_begin" value="31" />
<c:set var="strutsOptionYearComponent_end" value="1" />
<%@ include file="/inc/strutsOptionYearComponent.inc"%>
</select> 日
<span id="birthday_error_span" class="box-error"><b id="birthday_error_span_i"></b></span>
<p><a href="javascript:;" id="birtips" tabindex="-1">为什么要填写我的生日？</a></p>
</dd>
</dl>
<dl>
<dt>&nbsp;</dt>
<dd>
<%
java.util.Random random = new java.util.Random();
int id = random.nextInt(1431655764);
%>
<img id="verifyPic" src="http://icode.${applicationScope.sysdomain}/getcode.do?t=register_<%=id%>&rnd=<%=System.currentTimeMillis()%>"/><a href="javascript:refreshCode();" tabindex="-1">看不清换一张?</a></dd>
</dl>
<dl>
<dt><label for="code">验证码:</label></dt>
<dd><input type="text" name="icode" id="icode" class="inputtext validate-code" size="5" maxlength="5" /><input type="hidden" name="key_id" value=<%=id%> /><span id="icode_error_span" class="box-error"><b id="icode_error_span_i"></b></span></dd>
</dl>
<dl>
<dt>&nbsp;<input type="hidden" name="activity" value="" id="activity" /><input type="hidden" name="iv" value="" id="iv" /><input type="hidden" name="from" value="" id="from" /></dt>
<dd>
<input type="submit" id="button_submit" class="inputsubmit" value="" />
</dd>
</dl>
<script type="text/javascript" src="http://s.xnimg.cn/js/register/register-simple.js?ver=$revxxx$"></script>
<script type="text/javascript">
new RegCheck();
</script>
</form>
<%@ include file="/pages/register/reg/inc/form/rose/set_form.inc.jsp" %>