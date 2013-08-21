<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html"%>
<link href="http://s.xnimg.cn/csspro/base/layout.css?ver=$revxxx$"
	rel="stylesheet" type="text/css" media="all" />

<form class="main_regform" method="post" id="regform"  name="regform"
	action="/s-c-i-reg.do">
	<input type="hidden" name="fromUrl" id="fromUrl" />
	<input type="hidden" name="iu" id="iu" />
	<input type="hidden" name="ic" id="ic" />
	<input type="hidden" name="uuid" id="uuid" />
	<input type="hidden" name="ss" id="ss"  />
	<input type="hidden" name="action_id" id="action_id" />
	<input type="hidden" name="g" id="g" />
	<input type="hidden" name="b" id="b" />
	<input type="hidden" name="accType" id="accType" value="2"></input>
	<div class="form_row" id="d_email"><label
		id="regmail_label" for="regemail">邮箱:</label>
	<div class="input_wrap"><input type="text" name="regEmail"
		id="regEmail" class="inputtext" />
	<div class="box-error" id="regEmail_error_span">
	<div class="box-error-outer">
	<div class="box-error-inner">
	<p id="regEmail_error_span_i"></p>
	</div>
	</div>
	</div>
	<span id="dmmg" style="display: none;"><img
		src="http://s.xnimg.cn/img/reg/fzcg_dh.gif" /></span> <span
		id="span_hotmail_tip" class="noqqalt"></span>
		</div>
	</div>

	<div class="form_row" id="d_xid" style="display: none;"><label
		id="regmail_label" for="regemail">注册ID:</label>
	<div class="input_wrap"><input type="text" name="nicknameId"
		id="nicknameId" class="inputtext" />
	<div class="box-error" id="regnickname_error_span">
	<div class="box-error-outer">
	<div class="box-error-inner">
	<p id="regnickname_error_span_i"></p>
	</div>
	</div>
	</div>
	<span id="dmmg" style="display: none;"><img
		src="http://s.xnimg.cn/img/reg/fzcg_dh.gif" /></span> <span
		id="span_hotmail_tip" class="noqqalt">请输入注册后登录所使用的用户名。</span></div>
	</div>

	<div class="form_row" id="p_pwd"><label for="pwd">设置密码:</label>
	<div class="input_wrap"><input type="password" maxlength="20"
		name="pwd" id="pwd" class="inputtext" />
	<div class="box-error" id="pwd_error_span">
	<div class="box-error-outer">
	<div class="box-error-inner">
	<p id="pwd_error_span_i"></p>
	</div>
	</div>
	</div>
	<span id="mmg" style="display: none;"><img
		src="http://s.xnimg.cn/img/reg/fzcg_dh.gif" /></span></div>
	</div>
	<div class="form_row" id="p_name"><label for="name">真实姓名:</label>
	<div class="input_wrap"><input type="text" name="name" id="name"
		class="inputtext" /> <span id="xmg"
		style="display: none;"><img
		src="http://s.xnimg.cn/img/reg/fzcg_dh.gif" /></span>
	<div class="box-error" id="name_error_span">
	<div class="box-error-outer">
	<div class="box-error-inner">
	<p id="name_error_span_i"></p>
	</div>
	</div>
	</div>
	</div>
	</div>
	<div class="form_row" id="p_gender"><label>性别:</label>
	<div class="input_wrap"><label class="labelRadio"><input type="radio"
		name="gender" id="gender" value="男生" />男</label> <label
		class="labelRadio"><input type="radio" name="gender" 
		id="gender" value="女生" />女</label>
	<div class="box-error" id="gender_error_span">
	<div class="box-error-outer">
	<div class="box-error-inner">
	<p id="gender_error_span_i"></p>
	</div>
	</div>
	</div>
	</div>
	</div>

	<div class="form_row icode_row"><label class="empty">验证码:</label>
	<div class="input_wrap">
	<%
		java.util.Random random = new java.util.Random();
			int id = random.nextInt(1431655764);
	%> <img class="validate-num" id="verifyPic"
		src="http://icode.${applicationScope.sysdomain}/getcode.do?t=register_<%=id%>&rnd=<%=System.currentTimeMillis()%>" />
	<a href="javascript:refreshCode();">看不清换一张?</a></span></div>
	</div>


	<div class="form_row icode_input_row" id="p_icode"><label
		for="code">验证码:</label>
	<div class="input_wrap"><input type="text" name="icode"
		id="icode" class="inputtext validate-code" size="5" maxlength="5"
		/> <input type="hidden" name="key_id" value=<%=id%> />

	<div class="box-error" id="icode_error_span">
	<div class="box-error-outer">
	<div class="box-error-inner">
	<p id="icode_error_span_i"></p>
	</div>
	</div>
	</div>
	</div>
	</div>

	<div class="form_row"><input type="hidden" name="activity"
		value="" id="activity" /> <input type="hidden" name="iv" value=""
		id="iv" /> <input type="hidden" name="from" value="" id="from" /> <label
		class="empty">&nbsp;</label>
	<div class="input_wrap"><input type="image" id="button_submit"
		src="http://s.xnimg.cn/imgpro/reg/open_reg_button.png" /></div>
	</div>

</form>
						<script type="text/javascript">
							new RegCheck();
						</script>
<%@ include file="/pages/register/reg/inc/form/rose/set_form.inc.jsp" %>