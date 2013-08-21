<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %> 
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html" %>
<form class="main_regform" method="post" id="regform"  name="regform" action="/s-reg.do">
	<input type="hidden" name="fromUrl" id="fromUrl" />
	<input type="hidden" name="iu" id="iu" />
	<input type="hidden" name="ic" id="ic" />
	<input type="hidden" name="uuid" id="uuid" />
	<input type="hidden" name="ss" id="ss"  />
	<input type="hidden" name="action_id" id="action_id" />
	<input type="hidden" name="i" id="i" value="${requestScope.inviterId}" />
	<input type="hidden" name="e" value="${requestScope.inviteeEmail}" />
	<input type="hidden" name="appId" id="appId" />
	<input type="hidden" name="appName" id="appName" />
	<input type="hidden" name="g" id="g" />
	<input type="hidden" name="b" id="b" />
	<input type="hidden" name="inviterId" id="inviterId" />
	<input type="hidden" name="inviteType" id="inviteType" />
	<input type="hidden" name="code" id="code" />
	<input type="hidden" name="from" id="from" />
	<input type="hidden" name="inviteeEmail" id="inviteeEmail" />
	<input type="hidden" name="appToken" id="appToken" />
	<input type="hidden" name="groupName" id="groupName" />
	
	<input type="hidden" name="accType" id="accType" value="1"></input>
	
	<div class="form_row" id="d_email">
		<label id="regmail_label" for="regemail">电子邮箱:</label>
		<div class="input_wrap">
			<input type="text" name="regEmail" id="regEmail" class="inputtext" />
			<div class="box-error" id="regEmail_error_span">
				<div class="box-error-outer">
					<div class="box-error-inner">
						<p id="regEmail_error_span_i"></p>
					</div>
				</div>
			</div>
			<span id="dmmg" style="display:none;"><img src="http://s.xnimg.cn/img/reg/fzcg_dh.gif"/></span>
		</div>
	</div>
	<div class="form_row" id="p_pwd">
		<label for="pwd">设置密码:</label>
		<div class="input_wrap">
			<input type="password" maxlength="20" name="pwd" id="pwd" class="inputtext" />
			<div class="box-error" id="pwd_error_span" >
				<div class="box-error-outer">
					<div class="box-error-inner">
						<p id="pwd_error_span_i"></p>
					</div>
				</div>
			</div>
			<span id="mmg" style="display:none;"><img src="http://s.xnimg.cn/img/reg/fzcg_dh.gif"/></span>
		</div>
	</div>
	<div class="form_row" id="p_name">
		<label for="name">真实姓名:</label>
		<div class="input_wrap">
			<input type="text" name="name" id="name" class="inputtext"  />
			<span id="xmg" style="display:none;"><img src="http://s.xnimg.cn/img/reg/fzcg_dh.gif"/></span>
			<div class="box-error" id="name_error_span">
				<div class="box-error-outer">
					<div class="box-error-inner">
						<p id="name_error_span_i"></p>
					</div>
				</div>
			</div>
		</div>
	</div>
	<div class="form_row" id="p_gender">
		<label>性别:</label>
		<div class="input_wrap">
			<label class="labelRadio"><input type="radio" name="gender" id="gender" value="男生" />男</label>
			<label class="labelRadio"><input type="radio" name="gender" id="gender" value="女生" />女</label>
			<div class="box-error" id="gender_error_span">
				<div class="box-error-outer">
					<div class="box-error-inner">
						<p id="gender_error_span_i"></p>
					</div>
				</div>
			</div>
		</div>
	</div>
		<div class="form_row" id="p_birthday">
		<label>生日:</label>
		<div class="input_wrap">
			<select name="birth_year">
				<option value="">请选择</option>
				<c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
				<c:set var="strutsOptionYearComponent_end" value="1940" />
				<%@ include file="/inc/strutsOptionBirthdayYearComponent.inc.jsp"%>
			</select> 年
			<select name="birth_month">
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
			<div class="box-error" id="birthday_error_span">
				<div class="box-error-outer">
					<div class="box-error-inner">											
						<p id="birthday_error_span_i"></p>
					</div>
				</div>
			</div>
			<p class="b_tip"><a href="javascript:;" onclick="XN.DO.confirm('出于安全考虑，同时为了保证人人网用户的真实性，凡注册人人网用户都需要填写真实的出生日期。<br /><span class=\'gray\'>你可以在个人主页上隐藏你的出生日期。</span>', '为什么需要提供我的生日日期？')">为什么要填写我的生日？</a></p>
		</div>
	</div>
	<div class="form_row icode_row">
		<label class="empty">验证码:</label>
		<div class="input_wrap">
					<%
			java.util.Random random = new java.util.Random();
			int id = random.nextInt(1431655764);
			%>
			<img class="validate-num" id="verifyPic" src="http://icode.${applicationScope.sysdomain}/getcode.do?t=register_<%=id%>&rnd=<%=System.currentTimeMillis()%>"/>
			<a href="javascript:refreshCode();">看不清换一张?</a></span>
		</div>
	</div>

	
	<div class="form_row icode_input_row" id="p_icode">
		<label for="code">验证码:</label>
		<div class="input_wrap">
			<input type="text" name="icode" id="icode" class="inputtext validate-code" size="5" maxlength="5" />									

			<input type="hidden" name="key_id" value=<%=id%> /> 
			
			<div class="box-error" id="icode_error_span">
				<div class="box-error-outer">
					<div class="box-error-inner">
						<p id="icode_error_span_i"></p>
					</div>
				</div>
			</div>
		</div>
	</div>
	
	<div class="form_row">
			<input type="hidden" name="activity" value="" id="activity" />
			<input type="hidden" name="iv" value="" id="iv" />
			<input type="hidden" name="from" value="" id="from" />
			<label class="empty">&nbsp;</label>
			<div class="input_wrap">
				<input type="image" id="button_submit" src="http://s.xnimg.cn/imgpro/reg/app_reg_button.png"/>
			</div>
	</div>

</form>
						<script type="text/javascript">
							new RegCheck();
						</script>
<%@ include file="/pages/register/reg/inc/form/rose/set_form.inc.jsp" %>