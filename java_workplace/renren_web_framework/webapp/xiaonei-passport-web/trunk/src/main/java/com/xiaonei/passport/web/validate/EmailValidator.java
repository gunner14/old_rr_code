package com.xiaonei.passport.web.validate;


import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.stereotype.Service;

import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.web.model.PassportForm;
import com.xiaonei.platform.core.utility.RegularExpression;
import com.xiaonei.websecurity.adapter.SafeFilter;

@Service
public class EmailValidator implements PpParamValidator {

	protected Log log = LogFactory.getLog(this.getClass());

	@Override
	public LoginStatusCode validate(PassportForm pForm) {
		String email = null;
		try{
			if (log.isDebugEnabled()) {
				log.debug("login parameters in emailValidate are: "
						+ pForm.toString());
			}
			if (StringUtils.isEmpty(pForm.getEmail())) {
				log.error("用户名为空");
				return LoginStatusCode.FAILINPUT;
			
			}
			
			email =StringUtils.trimToEmpty(pForm.getEmail());
			email = RegularExpression.processForLogin(email);
			email = email.toLowerCase();
			email=SafeFilter.getInstance().getRichContent(email);
			pForm.setEmail(email);
			return LoginStatusCode.LOGIN_OK;
		}catch(Exception e){
			log.error("email validator error, email is :"+email );
			 return LoginStatusCode.FAILINPUT;
		}
	
		
	}
}
