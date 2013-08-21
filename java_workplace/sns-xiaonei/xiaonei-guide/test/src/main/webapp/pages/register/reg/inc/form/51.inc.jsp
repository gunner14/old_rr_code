<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %> 
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html" %>
					<form action="/commonreg.do" onsubmit="return false;" method="post" id="regform"  name="regform">
							<input type="hidden" name="fromUrl" id="fromUrl" />
							<input type="hidden" name="iu" id="iu" />
							<input type="hidden" name="ic" id="ic" />
							<input type="hidden" name="uuid" id="uuid" />
							<input type="hidden" name="g" id="g" />
							<input type="hidden" name="b" id="b" />
							<input type="hidden" name="ss" id="ss"  />
	  						<input type="hidden" name="action_id" id="action_id" />
							<div class="form_row" id="p_stage">
								<label for="">身份:</label>
								<div class="input_wrap">
									<select name="stage" id="stage" class="select" size="5"  onchange="regSelect()" style="height:auto" value="-1">
										<option value="-1">请选择身份</option>
										<option value="30">已工作</option>
										<option value="20">大学生</option>
										<option value="10">中学生/中专技校</option>
										<option value="90">以上都不是</option>
									</select>
								</div>
								<div class="box-error" id="stage_error_span">
									<div class="box-error-outer">
										<div class="box-error-inner">											
											<p id="stage_error_span_i">请选择您目前的身份</p>
										</div>
									</div>
								</div>
							</div>
	
							<input type="hidden" name="accType" id="accType" value="1"></input>
							<div  class="form_row" id="d_email">
								<label id="regmail_label" for="regemail">电子邮箱:</label>
								<div class="input_wrap">
									<input type="text" name="regEmail" id="regEmail" onblur="if(true == checkRegEmailKuang()){ajaxCheckEmailKuang();};" onfocus="$('stage_error_span').style.display='none';$('dmmg').style.display='none';if(10==getStage()){showRegMsgKuang('regEmail','请输入您的<font color=red>手机号</font>或<font color=red>电子邮箱</font>登录，我们不会公开您的帐号信息，请放心使用');}else{showRegMsgKuang('regEmail','请输入你的电子邮箱');}" class="inputtext" />
									<span id="dmmg" style="display:none;"><img src="http://s.xnimg.cn/img/reg/fzcg_dh.gif"/></span>
									<span id="span_hotmail_tip" class="noqqalt"><%--如果没有邮箱，请注册5GB的<a href="http://a.oadz.com/link/C/619/110685/q4zGLMnPLClHJygnjOtDH6GArQw_/a/1830?https://domains.live.com/members/signup.aspx?domain=xnmsn.cn" target="_blank">Hotmail</a>邮箱。 --%></span>
									<span id="span_xid_tip" onclick="javascirpt:regType('xid');" class="noqqalt" style="display:none">如果没有邮箱，可以直接使用<a>字符</a>注册</span>
								</div>
								<div class="box-error" id="regEmail_error_span">
									<div class="box-error-outer">
										<div class="box-error-inner">											
											<p id="regEmail_error_span_i">请输入你的电子邮箱</p>
										</div>
									</div>
								</div>
							</div>
								
							<div class="form_row" id="d_xid"  style="display:none">
								<label for="stage">输入帐号:</label>
								<div class="input_wrap">
									<input type="text" name="nicknameId" id="nicknameId" class="inputtext" onclick="$('stage_error_span').style.display='none';chkNick();" onblur="ajaxCheckNkKuang();"></input><span id="mg" style="display:none;"><img src="http://s.xnimg.cn/img/reg/fzcg_dh.gif"/></span>
									<span id="nkg" style="display:none;"><img src="http://s.xnimg.cn/img/reg/fzcg_dh.gif"/></span>
									<span onclick="javascirpt:regType('email');" class="noqqalt" >我想用<a>手机号或邮箱</a>注册</span>
								</div>
								<div class="box-error" id="nicknameId_error_span">
									<div class="box-error-outer">
										<div class="box-error-inner">											
											<p id="nicknameId_error_span_i">6-20位<font color="red">字母及数字组合</font>（不能为纯数字）;此帐号用来登录网站，注册后不可更改，请牢记。</p>
										</div>
									</div>
								</div>
							</div>
							
							<div class="form_row" id="p_pwd">
								<label for="passwd">设置密码:</label>
								<div class="input_wrap">
									<input type="password" maxlength="20" name="pwd" id="pwd" class="inputtext" onblur="if(true == checkPasswordKuang()){$('mmg').style.display='inline'}else{$('mmg').style.display='none'};" onfocus="$('mmg').style.display='none';showRegMsgKuang('pwd','密码必须由6-20个字符组成');" />
									<span id="mmg" style="display:none;"><img src="http://s.xnimg.cn/img/reg/fzcg_dh.gif"/></span>
								</div>
								<div class="box-error" id="pwd_error_span" >
									<div class="box-error-outer">
										<div class="box-error-inner">											
											<p id="pwd_error_span_i">密码必须由6-20个字符组成</p>
										</div>
									</div>
								</div>
							</div>	
							
							<div class="form_row" id="p_repwd">
								<label for="reenter">确认密码:</label>
								<div class="input_wrap">
									<input type="password" maxlength="20" name="repwd" id="repwd" onblur="if(true == checkRePasswordKuang()){$('rmmg').style.display='inline'}else{$('rmmg').style.display='none'};" onfocus="$('rmmg').style.display='none';showRegMsgKuang('repwd','请再输一次上面输入的密码');" class="inputtext" />
									<span id="rmmg" style="display:none;"><img src="http://s.xnimg.cn/img/reg/fzcg_dh.gif"/></span>									
								</div>
								<div class="box-error" id="repwd_error_span">
									<div class="box-error-outer">
										<div class="box-error-inner">											
											<p id="repwd_error_span_i">请再输一次上面输入的密码</p>
										</div>
									</div>
								</div>
							</div>
								
							<div class="form_row" id="p_name">
								<label for="name">真实姓名:</label>
								<div class="input_wrap">
									<input type="text" name="name" id="name" class="inputtext" onblur="if(true == checkUsernameKuang()){ajaxCheckNameKuang();};" onfocus="$('xmg').style.display='none';showRegMsgKuang('name','请输入<font style=\\'color: red;font-weight: bold\\'>真实中文姓名</font>，方便朋友查找。');" /><span id="xmg" style="display:none;"><img src="http://s.xnimg.cn/img/reg/fzcg_dh.gif"/></span>
								</div>
								<div class="box-error" id="name_error_span">
									<div class="box-error-outer">
										<div class="box-error-inner">											
											<p id="name_error_span_i">请输入真实中文姓名，方便朋友查找。</p>
										</div>
									</div>
								</div>
							</div>
							
							<div class="form_row" id="p_gender">
								<label>性别:</label>
								<div class="input_wrap">
									<label class="labelRadio"><input type="radio" name="gender" id="gender" value="男生" />男</label>
									<label class="labelRadio"><input type="radio" name="gender" id="gender" value="女生" />女</label>
								</div>
							</div>
							
							<div class="form_row" id="p_schoolType" style="display:none">
								<label for="">学校类型:</label>
								<div class="input_wrap">
									<select id="schoolType" name="schoolType" onchange="regSelect2()" >
										<option value="-1">请选择学校类型</option>
										<option value="0">高中</option>
										<option value="1">初中</option>
										<option value="2">中专/技校</option>
									</select>
									<span id="schoolType_error_span"></span>
								</div>
							</div>
							
							<div class="form_row" id="typeOfCoursespan" style="display:none">
								<label for="">类型:</label>
								<div class="input_wrap">
									<select name="typeOfCourse" id="typeOfCourse" class="select" >
										<%@ include file="/inc/strutsSelectOptionForDegree.inc" %>
									</select>
								</div>
							</div>
							
							<div class="form_row" id="p_universityname" style="display:none">
								<label for="schId">大学:</label>
								<div class="input_wrap">
									<input type="hidden" name="universityid" id="universityid" />
									<input type="text" name="universityname" id="universityname" class="inputtext" onclick="showpan();bodyclick=false;" readonly />
									<span id="universityname_error_span">人人网现在对全球大学生开放</span>
								</div>
							</div>
							
							<div class="form_row" id="p_highSchoolName0" style="display:none">
								<label for="schId">高中:</label>
								<div class="input_wrap">
									<input type="hidden" name="highSchoolCode" id="highSchoolCode0" />
									<input type="text" name="highSchoolName" id="highSchoolName0" class="inputtext" onclick="SchoolComponent.show(SchoolComponent.OPEN_HIGH_SCHOOL,'highSchoolName0','highSchoolCode0');" readonly />
									<span id="highSchoolName0_error_span">人人网现已开通全国56000所中学</span>
								</div>
							</div>
							
							<div class="form_row"  id="p_juniorHighSchoolName" style="display:none">
								<label for="">初中:</label>
								<div class="input_wrap">
										<input type="text" name="juniorHighSchoolName" id="juniorHighSchoolName" class="inputtext" onclick="SchoolComponent.show(SchoolComponent.JUNIOR_SCHOOL,'juniorHighSchoolName','juniorHighSchoolId');" />
										<input type="hidden" name="juniorHighSchoolId" id="juniorHighSchoolId"/>
										<span id="juniorHighSchoolName_error_span" style="display:inline;"></span>
								</div>
							</div>
							
							<div id="juniorHighSchoolContainer" class="posrela"></div>
								
							<div class="form_row" id="p_techHighSchoolName" style="display:none">
								<label for="">中专技校:</label>
								<div class="input_wrap">
										<input type="text" name="techHighSchoolName" id="techHighSchoolName" class="inputtext" onclick="SchoolComponent.show(SchoolComponent.COLLEGE_SCHOOL,'techHighSchoolName','techHighSchoolId');" />
										<input type="hidden" id="techHighSchoolId" name="techHighSchoolId" />
										<span id="techHighSchoolName_error_span"></span>
								</div>
							</div>
							<div id="techHighSchoolContainer" class="posrela"></div>
							
							
							<div id="d_workplace" style="display:none">
								<div class="form_row">
								<label>居住地:</label>
								<div class="input_wrap">
								<c:choose>
									<c:when test="${!empty requestScope.currentPro}">
									<select name="homeprovince"
									id="homeprovince" class="select" 
									onchange="Profile.getInCityListOfProvince(this.selectedIndex, 'homecitycode')"
									value="${requestScope.currentPro}">
									<%@ include file="/pages/profile/inProvinceHtml.jsp"%>
									</select>
								</div>
									
								<div class="box-error" id="province_error_span">
									<div class="box-error-outer">
										<div class="box-error-inner">											
											<p id="province_error_span_i">请输入省份</p>
										</div>
									</div>
								</div>
								
									<input type="hidden" name="homecityname" id="homecityname" />
								</div>
								
								<div class="form_row">
									<label class="empty">居住地:</label>
									<div class="input_wrap">
									<select
										name="homecitycode" id="homecitycode" class="select"
										
										onblur="Profile.selectCity(this.selectedIndex, 'homecitycode', 'homecityname');"
										>
										<option value="">选择城市: </option>
									</select>
									</div>
									<div class="box-error" id="city_error_span" >
										<div class="box-error-outer">
											<div class="box-error-inner">											
												<p id="city_error_span_i">请输入城市</p>
											</div>
										</div>
									</div>
								</div>
									</c:when>
									<c:otherwise>
									<select name="homeprovince"
									id="homeprovince" class="select" 
									onclick="Profile.getInCityListOfProvince(this.selectedIndex, 'homecitycode')">
									<%@ include file="/pages/profile/inProvinceHtml.jsp"%>
									</select>
									</div>
									<div class="box-error" id="city_error_span" >
										<div class="box-error-outer">
											<div class="box-error-inner">											
												<p id="city_error_span_i">请输入省份</p>
											</div>
										</div>
									</div>
								</div>
									<input type="hidden" name="homecityname" id="homecityname" />
									<div class="form_row">
										<label class="empty">居住地:</label>
										<div class="input_wrap">
										<select
											name="homecitycode" id="homecitycode" class="select"
											
											onblur="Profile.selectCity(this.selectedIndex, 'homecitycode', 'homecityname');"
											>
											<option value="">选择城市: </option>
										</select>
										</div>
										<div class="box-error" id="city_error_span" >
											<div class="box-error-outer">
												<div class="box-error-inner">											
													<p id="city_error_span_i">请输入城市</p>
												</div>
											</div>
										</div>
									</div>
									</c:otherwise>
								</c:choose>
									
							
						</div>
						
					

							<div class="form_row" id="p_icode">
								<label for="code">验证码:</label>
								<div class="input_wrap">
									<span class="code">请输入以下验证码，<a href="javascript:refreshCode();">看不清换一张?</a></span>
									<input type="text" name="icode" id="icode" class="inputtext validate-code" size="5" maxlength="5" />									
									<%
									java.util.Random random = new java.util.Random();
									int id = random.nextInt(1431655764);
									%>
									<input type="hidden" name="key_id" value=<%=id%>> 
									<img class="validate-num" id="verifyPic" src="http://icode.${applicationScope.sysdomain}/getcode.do?t=register_<%=id%>&rnd=<%=System.currentTimeMillis()%>"/>
								</div>
								<div class="box-error" id="icode_error_span">
									<div class="box-error-outer">
										<div class="box-error-inner">											
											<p id="icode_error_span_i">请再输一次上面输入的密码</p>
										</div>
									</div>
								</div>
							</div>

							<div class="form_row">
								<input type="hidden" name="activity" id="activity" value="${activity}" />
								<input type="hidden" name="iv" id="iv" value="${requestScope.iv}" />
								<input type="hidden" name="from" id="from" value="${requestScope.from}" />
								<c:if test="${requestScope.from == '29'}"><input type="hidden" name="ayid" value="<%=ayid%>" id="ayid" /></c:if>
								<label class="empty">&nbsp;</label>
								<div class="input_wrap">
									<input type="image" src="http://s.xnimg.cn/imgpro/reg/regbtn.gif" onclick="if(true != $('agree').checked){alert('您没有同意人人网服务条款');return;};if(true != regSubmitKuang()){return;};$('regform').submit();" style="cursor:pointer;" />
								</div>
							</div>
							
							<div class="form_row">
								<label class="empty">&nbsp;</label>
								<div class="input_wrap">
									<input type="checkbox" name="agree" id="agree" value="1" checked onclick="checkName('agree')" />
										我已阅读并同意遵守
										<a href="http://${applicationScope.sysdomain}/info/agreement.jsp" target="_blank">人人网服务条款</a>
								</div>
							</div>
						</form>
						<%@ include file="/pages/register/reg/inc/form/rose/set_form.inc.jsp" %>