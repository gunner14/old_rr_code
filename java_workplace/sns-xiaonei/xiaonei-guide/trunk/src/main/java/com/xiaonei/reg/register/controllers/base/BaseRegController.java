package com.xiaonei.reg.register.controllers.base;

import java.util.HashMap;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.AsSuperController;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.platform.core.opt.base.chain.impl.BizFilterUtil;
import com.xiaonei.platform.core.opt.base.chain.impl.struts.util.BaseActionUtil;
import com.xiaonei.reg.common.RegLogger;
import com.xiaonei.reg.register.form.base.RegForm;
import com.xiaonei.reg.usertrace.logic.UserTraceLogic;

/**
 * BaseRegAction
 * <p/>
 * 注册使用，请注册的童鞋继承之
 * <p/>
 * 移出了PassportValidation, Navigation, Activated
 * 
 * @author wang-tai(tai.wang@opi-corp.com) 2009-2-26 - 下午07:11:31
 */
@AsSuperController
public abstract class BaseRegController {

	protected static final UserTraceLogic userTraceLog = UserTraceLogic
			.getInstance();

	protected abstract Map<String, RegForward> setRegMapping(
			final Map<String, RegForward> mapping);

	
	@Get
    @Post
	public String get(Invocation invocation,  RegForm regForm) {
		return exec(invocation, regForm);
	}	

	private String exec(Invocation invocation, RegForm regForm) {
		final Map<String, RegForward> mapping = new HashMap<String, RegForward>();
		setRegMapping(mapping);
		if(mapping!=null){
		    this.printObj(mapping);
		}
		RegForward rt = executeRt(new RegMapping(mapping), regForm, invocation
				.getRequest(), invocation.getResponse());
		if (null != rt) {
			return rt.go();
		}
		return null;
	}

	private void printObj(Map<String, RegForward> map) {
        if (map != null) {
            for (java.util.Map.Entry<String, RegForward> b : map.entrySet()) {
                String k = (String)b.getKey();
                RegForward v = (RegForward)b.getValue();
                RegLogger.getLoggerWithType(BaseRegController.class).debug(
                        "key: " + k + ",value: " + v.getPath());
            }
        }
    }
	
	/**
	 * 需要实现
	 * 
	 * @param mapping
	 * @param regForm
	 * @param request
	 * @param response
	 * @return
	 */
	public abstract RegForward executeRt(RegMapping mapping, RegForm regForm,
			HttpServletRequest request, HttpServletResponse response);

	public static String getFromIp(HttpServletRequest request) {
		return BaseActionUtil.getClientIP(request);
	}

	public void addMsg(HttpServletRequest request,
			HttpServletResponse response, String msg) {
		BizFilterUtil.addMsg(request, response, msg);
	}

	public void addErr(HttpServletRequest request,
			HttpServletResponse response, String msg) {
		BizFilterUtil.addErr(request, response, msg);
	}

	public void addMsg(HttpServletRequest request, String msg) {
		BizFilterUtil.addMsg(request, msg);
	}

	public void addErr(HttpServletRequest request, String msg) {
		BizFilterUtil.addErr(request, msg);
	}
}
