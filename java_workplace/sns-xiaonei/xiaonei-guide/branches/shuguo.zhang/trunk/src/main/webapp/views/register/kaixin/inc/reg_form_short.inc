<%@ page pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>
<%@ include file="/inc/errors.inc" %>
	<input type="hidden" name="i" value="700001678" />
	<input type="hidden" name="ayid" value="${requestScope.ayid}" />
	<input type="hidden" name="e" value="${requestScope.inviteeEmail}" />
	<input type="hidden" name="fromUrl" id="fromUrl" />
	<input type="hidden" name="appId" id="appId" />
	<input type="hidden" name="inviterId" id="inviterId" value="700001678" />
	<input type="hidden" name="inviteType" id="inviteType" value="2" />
	<input type="hidden" name="code" id="code" value="AFKTrbBj2H"/>
	<input type="hidden" name="from" id="from" />
	<input type="hidden" name="inviteeEmail" id="inviteeEmail" />
	<input type="hidden" name="appToken" id="appToken" />
	<input type="hidden" name="groupName" id="groupName" />
	<input type="hidden" name="ss" id="ss" />
	<input type="hidden" name="g" id="g" value="${requestScope.g}" />
	<input type="hidden" name="b" id="b" value="${requestScope.b}" />
	<%@ include file="jsanti.inc" %>
	<fieldset class="account-field">
		<legend>账户信息</legend>
		<table>
			<tr>
				<th>你的Email:</th>
				<td class="inputholder">
					<span class="required">*</span>
					<input type="text" name="regEmail" id="regEmail" tabindex="1" class="text input-text"/>
					<%@ include file="/views/register/kaixin/inc/reg_email_recomm.inc" %>
				</td>
				<td>
					<div class="zc_cwts hide" id="regEmail_ok" class="text"  style="display:none;"></div>
					<div id="regEmail_err" class="zc_cwts hide"></div>
				</td>
			</tr>
			<tr>
				<th>你的密码:</th>
				<td class="inputholder">
					<span class="required">*</span>
					<input type="password" name="pwd" id="pwd" tabindex="2" class="text input-text" />
				</td>
				<td>
					<div class="zc_cwts hide" id="pwd_ok"></div>
					<div id="pwd_err" class="zc_cwts hide"></div>
				</td>
			</tr>
		</table>						
	</fieldset>
	<fieldset>
		<legend>详细信息</legend>
		<table>
			<tr>
				<th>你的姓名:</th>
				<td class="inputholder">
					<span class="required">*</span>
					<input type="text" name="name" id="name" tabindex="3" class="text input-text" />
				</td>
				<td>
					<div class="zc_cwts hide" id="name_ok"></div>
					<div id="name_err" class="zc_cwts hide"></div>
				</td>
			</tr>
			<tr>
				<th>性别:</th>
				<td class="clearfix">
					<span class="required">*</span>
					<label for="male" class="reg_radio"><input type="radio" name="gender" value="男生" tabindex="4"/>男</label>
					<label for="woman" class="reg_radio"><input type="radio" name="gender" value="女生"  tabindex="5"/>女</label>
				</td>
				<td>
					<div class="zc_cwts hide" id="gender_ok"></div>
					<div id="gender_err" class="zc_cwts hide"></div>
				</td>
			</tr>
			<tr style="display:none">
				<th></th>
				<td class="inputholder">
					<input type="hidden" name="birth_year" id="birth_year" value="1700"/>
					<input type="hidden" name="birth_month" id="birth_month" value="1"/>
					<input type="hidden" name="birth_day" id="birth_day" value="1"/>
				</td>
				<td>							
					<div id="birth_year_err" class="zc_cwts hide">&nbsp;</div>
					<div id="birth_month_err" class="zc_cwts hide">&nbsp;</div>
					<div class="zc_cwts hide" id="birth_day_ok" style="display:none;">&nbsp;</div>
					<div id="birth_day_err" class="zc_cwts hide">&nbsp;</div>
				</td>
				</tr>
				<tr style="display:none">
				<th></th>
				<td class="inputholder">
					<input type="hidden" name="homeprovince" id="homeprovince" value=""/>
					<input type="hidden" name="homecitycode" id="homecitycode" value=""/>
					<input type="hidden" name="workplacename" value=""/>
					<input type="hidden" name="stage" value="30"/>
					<input type="hidden" name="basicControl" value="99"/>
				</td>
				<td>			
					<div id="homeprovince_err" class="zc_cwts hide">&nbsp;</div>
					<div id="homecitycode_err" class="zc_cwts hide">&nbsp;</div>
					<div class="zc_cwts hide" id="homecitycode_ok" style="display:none;">&nbsp;</div>
				</td>
			</tr>
		</table>
	</fieldset>

	<fieldset>
		<legend>验证程序</legend>
		<table>
			<tr>
				<th>验证码:</th>
				<td class="inputholder coder">
					<p class="float-left coder_input">
						<span class="required">*</span>
						<input type="text" name="icode" id="icode" class="text" size="5" maxlength="5" tabindex="6"/>&nbsp;&nbsp;
					</p>
					<p class="float-left coder_img">
						<%
							java.util.Random random = new java.util.Random();
							int id = random.nextInt(1431655764);
						%>
						<input type="hidden" id="key_id" name="key_id" value="<%=id%>"/> 
						<img id="verifyPic" src="${applicationScope.urlIcode}/getcode.do?t=register_<%=id%>&rnd=<%=System.currentTimeMillis()%>"/>
						<a href="javascript:refreshCode();" onclick="javascript:void(0);" tabindex="7">看不清楚 换一张</a>
					</p>

				</td>
				<td>
					<div class="zc_cwts hide" id="icode_ok"></div>
					<div id="icode_err" class="zc_cwts hide"></div>
				</td>
			</tr>
			<tr style="display:none">
					<th></th>
					<td class="inputholder"><span class="required"></span>
						<input type="hidden" name="msnAccountOk" value="0" id="msnAccountOk" />
						<input type="hidden" id="msnValid" name="msnValid" value="0"/>
						<input type="hidden" name="msnValid2" value="0" id="msnValid2" />
						<input type="text" id="ismsn" name="ismsn" value="abc"/>
					</td>
					<td>
						<div style="display: none;" id="ismsn_ok" class="zc_cwts hide"></div>
						<div class="zc_cwts hide" id="ismsn_err"></div>
					</td>
			</tr>