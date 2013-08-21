package com.xiaonei.login.logic.afterlogin_chains;

import javax.servlet.http.HttpServletRequest;


import org.springframework.stereotype.Service;


import com.xiaonei.login.constants.LoginParamDef;
import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.logic.afterlogin_chains.AbsLoginResultChainNode;
import com.xiaonei.login.logic.captcha.CaptchaAdapterFactory;
import com.xiaonei.login.model.LoginModel;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;

@Service
public class WeakPasswordEnhenceChainNode extends AbsLoginResultChainNode {


	@Override
	public LoginStatusCode doChain(LoginModel loginModel,
			LoginStatusCode perCode) {
		Integer userId=getUserId(loginModel.getRequest());
		if(userId!=null&& validate(userId)){
			String origURL=loginModel.getRequest().getParameter(LoginParamDef.PARAM_ORIGURL);
			loginModel.getRequest().setAttribute("saftOrigURL", "http://safe.renren.com/notice/weak?origURL="+origURL);
		}
		return next.doChain(loginModel, perCode);
	}
	/**
	 * 判断是否为弱密码
	 * @param userId user id
	 * @return
	 */
	private boolean validate(int userId){
		return CaptchaAdapterFactory.getWeakPasswordInstance().isWeakPassword(userId);
	}
	
	/**
	 * 获取用户id
	 * @param request
	 * @return
	 */
	 protected Integer getUserId(HttpServletRequest request) {
	        Integer userId = (Integer) request.getAttribute(LoginParamDef.ATTRIBUTE_USER_ID);
	        if (userId == null) {
	            final User user = BaseThreadUtil.currentHost(request);
	            if (null != user) {
	                userId = user.getId();
	            }
	        }
	        return userId;
	    }
}
