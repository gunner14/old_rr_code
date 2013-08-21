package com.xiaonei.passport.web.event;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.xiaonei.passport.constants.LoginConfigureKey;
import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.web.BeanFactory;
import com.xiaonei.passport.web.adapt.LoginConfigureImpl;
import com.xiaonei.passport.web.constants.OrigUrlDefaultDef;
import com.xiaonei.passport.web.event.after.IAfterLoginListern;
import com.xiaonei.passport.web.event.before.IBeforeLoginListern;
import com.xiaonei.passport.web.event.fail.IFailLoginListern;
import com.xiaonei.passport.web.model.LoginResult;
import com.xiaonei.passport.web.model.PassportForm;
/**
 * 触发事件,去调用相应的操作
 * @author xiaodong.yuan
 *
 */
public class EventProcess {
	private static EventProcess _instance=new EventProcess();
	
	private EventProcess(){}
	public static EventProcess getInstance(){
		return _instance;
	}
	
	/**
	 * 登陆成功后触发的事件 
	 * @param request
	 * @param response
	 * @param pForm
	 * @param result
	 * @param key
	 */
	public void fireLoginSuccessEvent(HttpServletRequest request,
			HttpServletResponse response, PassportForm pForm, LoginResult result, LoginConfigureKey key) {
		java.util.List<String> beanNames = LoginConfigureImpl.getLocalInstance().getLoginConfigure(
				pForm.getLoginRegistrationId(),
				key);
		if(beanNames==null){
			return ;
		}
		Class<IAfterLoginListern> clazz = IAfterLoginListern.class;
		LoginStatusCode code = LoginStatusCode.LOGIN_OK;
		for (String beanName : beanNames) {
			Object object = BeanFactory.getBean(beanName);
			if (object != null && clazz.isInstance(object)) {
				IAfterLoginListern logic = (IAfterLoginListern) object;
				LoginStatusCode rt = logic.process(request, response, pForm,
						result);
				if (rt != LoginStatusCode.LOGIN_OK) {
					code = rt;
					break;
				}
			}
		}
		if (LoginStatusCode.LOGIN_OK != code) {
			fireErrorLoginEvent(request, response, pForm, code.getCode(), result, key);
			return;
		}

	}
	/**
	 * 登陆出错后调用的方法
	 * @param request
	 * @param response
	 * @param pForm
	 * @param code
	 * @param result
	 * @return
	 */
	public String fireErrorLoginEvent(HttpServletRequest request,
			HttpServletResponse response, PassportForm pForm,
			int code, LoginResult result,LoginConfigureKey key) {
		java.util.List<String> beanNames = LoginConfigureImpl.getLocalInstance().getLoginConfigure(
				pForm.getLoginRegistrationId(),
				key);
		Class<IFailLoginListern> clazz = IFailLoginListern.class;
		String origUrl = IFailLoginListern.defualtErrorUrl;
		for (String beanName : beanNames) {
			Object object = BeanFactory.getBean(beanName);
			if (object != null && clazz.isInstance(object)) {
				IFailLoginListern logic = (IFailLoginListern) object;
				if (logic.support(request, response, pForm, code, result)) {
					return logic.process(request, response, pForm, code,
							result);
				}
			}
		}
		return origUrl;

	}
	/**
	 * 登陆前参数验证
	 * @param request
	 * @param response
	 * @param pForm
	 * @param key
	 * @return
	 */
	public LoginStatusCode fireBeforeLoginEvent(HttpServletRequest request,
			HttpServletResponse response, PassportForm pForm, String key) {
		java.util.List<String> beanNames = LoginConfigureImpl.getLocalInstance().getLoginConfigure(
				pForm.getLoginRegistrationId(),
				key);
		Class<IBeforeLoginListern> clazz = IBeforeLoginListern.class;
		for (String beanName : beanNames) {
			Object object = BeanFactory.getBean(beanName);
			if (object != null && clazz.isInstance(object)) {
				IBeforeLoginListern logic = (IBeforeLoginListern) object;
				LoginStatusCode rt = logic.process(request, response, pForm);
				if (rt != LoginStatusCode.LOGIN_OK) {
					return rt;
				}
			}
		}
		return LoginStatusCode.LOGIN_OK;
	}
}
