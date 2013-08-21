
<%@page
	import="com.xiaonei.reg.register.logic.pretreatment.validate.CheckBirthday"%>
<%@page
	import="com.xiaonei.reg.register.logic.pretreatment.validate.CheckGender"%>
<%@page
	import="com.xiaonei.reg.register.logic.pretreatment.validate.CheckName"%>
<%@page
	import="com.xiaonei.reg.register.logic.pretreatment.validate.CheckPwd"%>
<%@page
	import="com.xiaonei.reg.register.logic.pretreatment.validate.CheckAccType"%>
<%@page
	import="com.xiaonei.reg.register.logic.pretreatment.validate.CheckAntispam"%>
<%@page import="java.util.Map"%>
<%@page import="java.util.Map.Entry"%>
<%@page import="com.xiaonei.reg.register.constants.IRegCheckCodeDef"%>
<%@page import="com.xiaonei.reg.common.ObjectHolder"%>
<%@page import="java.util.concurrent.ConcurrentHashMap"%>
<%@page
	import="com.xiaonei.reg.register.logic.pretreatment.validate.base.BaseRegCheckLogic"%>
<%@page import="java.util.List"%>
<%@page import="org.apache.commons.lang.StringUtils"%><%@page
	import="com.xiaonei.reg.usertrace.model.RegOprDefine"%>
<%@page import="com.xiaonei.reg.usertrace.logic.UserTraceLogic"%>
<%@page import="com.xiaonei.platform.core.opt.ice.WUserAdapter"%>
<%@page import="com.xiaonei.platform.core.model.User"%>
<%@page import="com.xiaonei.reg.register.logic.additional.ActivateLogic"%>
<%@page import="com.xiaonei.reg.register.constants.IAccountType"%>
<%@page import="net.sf.json.JSONObject"%>
<%@page import="com.xiaonei.reg.register.pojo.RegisterPojo"%>
<%@page import="com.xiaonei.reg.register.logic.RegisterLogicFactory"%>
<%@page
	import="com.xiaonei.platform.core.opt.base.chain.impl.struts.util.BaseActionUtil"%>
<%@page import="com.xiaonei.reg.register.form.RegPureIDCommonForm"%>
<%@page import="com.xiaonei.reg.register.form.RegShortCommonForm"%>
<%@page
	import="com.xiaonei.reg.register.logic.pretreatment.validate.RegCheckManager"%>
<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%
	int succ = 0;
	int fail = 1;
	/* 搞个form */
	String key = request.getParameter("k");
	if (!"342nvbew4j8xcvww8896n3z-z".equals(key)) {
		JSONObject o = new JSONObject();
		o.put("code", fail);
		o.put("msg", "密钥错误");
		out.print(o.toString());
		return;
	}
	final RegShortCommonForm regForm = new RegShortCommonForm();
	regForm.setSs(request.getParameter("ss"));
	regForm.setAction_id(request.getParameter("action_id"));
	regForm.setAccType(IAccountType.XID);
	regForm.setNicknameId(request.getParameter("nicknameId"));
	regForm.setPwd(request.getParameter("pwd"));
	regForm.setName(request.getParameter("name"));
	regForm.setGender(request.getParameter("gender"));
	regForm.setBirth_year(request.getParameter("birth_year"));
	regForm.setBirth_month(request.getParameter("birth_month"));
	regForm.setBirth_day(request.getParameter("birth_day"));
	regForm.setG(request.getParameter("g"));
	regForm.setB(request.getParameter("b"));

	/* 1,表单数据验证 */
	String rtStr = IRegCheckCodeDef.OK;
	rtStr = new CheckAccType().check(regForm);
	if (!IRegCheckCodeDef.OK.equals(rtStr)) {
		JSONObject o = new JSONObject();
		o.put("code", fail);
		o.put("msg", rtStr);
		out.print(o.toString());
		return;
	}
	rtStr = new CheckPwd().check(regForm);
	if (!IRegCheckCodeDef.OK.equals(rtStr)) {
		JSONObject o = new JSONObject();
		o.put("code", fail);
		o.put("msg", rtStr);
		out.print(o.toString());
		return;
	}
	rtStr = new CheckName().check(regForm);
	if (!IRegCheckCodeDef.OK.equals(rtStr)) {
		JSONObject o = new JSONObject();
		o.put("code", fail);
		o.put("msg", rtStr);
		out.print(o.toString());
		return;
	}
	rtStr = new CheckGender().check(regForm);
	if (!IRegCheckCodeDef.OK.equals(rtStr)) {
		JSONObject o = new JSONObject();
		o.put("code", fail);
		o.put("msg", rtStr);
		out.print(o.toString());
		return;
	}
	rtStr = new CheckBirthday().check(regForm);
	if (!IRegCheckCodeDef.OK.equals(rtStr)) {
		JSONObject o = new JSONObject();
		o.put("code", fail);
		o.put("msg", rtStr);
		out.print(o.toString());
		return;
	}
	// anti spamer
	rtStr = CheckAntispam.getInstance().checkAntispam(regForm, request,
			response);

	if (!IRegCheckCodeDef.OK.equals(rtStr)) {
		JSONObject o = new JSONObject();
		o.put("code", fail);
		o.put("msg", rtStr);
		out.print(o.toString());
		return;
	}

	/* 2,业务逻辑,生成用户啥的... */
	final RegisterPojo regPojo = new RegisterPojo(regForm,
			BaseActionUtil.getClientIP(request));
	int userId;
	try {
		userId = -RegisterLogicFactory.getRegShortCommon().register(
				regPojo);
	} catch (final Exception e) {
		/* err */
		JSONObject o = new JSONObject();
		o.put("code", fail);
		o.put("msg", "系统错误,请稍候尝试");
		out.print(o.toString());
		return;
	}

	/* 3,激活 */
	User user;
	try {
		user = WUserAdapter.getInstance().get(userId);
		ActivateLogic.getInstance().activateUser(user);
	} catch (Exception e) {
		JSONObject o = new JSONObject();
		o.put("code", succ);
		o.put("msg", "注册成功,激活失败");
		out.print(o.toString());
		return;
	}

	UserTraceLogic.getInstance().insertRegHis(request, user,
			regPojo.getRegEmail(), regForm.getAction_id(), 0,
			regForm.getSs(), null, regForm.getG(), regForm.getB(), 0,
			true);
	UserTraceLogic.getInstance().insertRegHis(request, user,
			regPojo.getRegEmail(),
			String.valueOf(RegOprDefine.NORMAL_REG_OK), 0,
			regForm.getSs(), null, regForm.getG(), regForm.getB(), 0,
			true);

	JSONObject o = new JSONObject();
	o.put("code", succ);
	o.put("msg", "注册成功,并且激活成功");
	out.print(o.toString());
%>
