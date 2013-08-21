package com.xiaonei.login.logic.failswitcher.nodes;


import org.apache.commons.lang.StringUtils;

import com.xiaonei.admin.util.net.URLEncoder;
import com.xiaonei.login.constants.LoginParamDef;
import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.logic.captcha.CaptchaAdapterFactory;

import net.paoding.rose.web.Invocation;


/**
 * AbsLoginFailSwitchNode <br>
 *
 * @author tai.wang@opi-corp.com Jul 28, 2010 - 2:45:43 PM
 */
public abstract class AbsLoginFailSwitchNode implements ILoginFailSwitchNode {
	private String failUri = ILoginFailSwitchNode.failUri;
	
	protected final String getFailUri(Invocation inv) {
		if(inv.getRequest().getAttribute("LOGIN_TYPE")==null||!"web".equals(inv.getRequest().getAttribute("LOGIN_TYPE"))){
			return "/views/login.jsp";
		}
		 final String email=inv.getRequest().getParameter(LoginParamDef.PARAM_EMAIL);
		 StringBuilder sb=new StringBuilder(this.failUri);
			boolean isFirst=true;
			String origURL = inv.getRequest().getParameter(LoginParamDef.PARAM_ORIGURL);
			isFirst=append(sb, "origURL",origURL, isFirst, true);
		 if(email==null){
			 return sb.toString();
		 }
		
		Integer count=CaptchaAdapterFactory.getCaptchaAdapterInstance().getLoginCountFromRequest(inv.getRequest());
		isFirst=append(sb, "catchaCount",String.valueOf(count), isFirst, false);
		final LoginStatusCode code=(LoginStatusCode) inv.getModel(LoginParamDef.ATTRIBUTE_LOGIN_STATUS_CODE);
		isFirst=append(sb,"failCode", String.valueOf(code.getCode()), isFirst, false);
		//isFirst=append(sb,"failDescription", code.getDescription(), isFirst, true);
		//isFirst=append(sb,"email", email, isFirst, false);
//		 String emailLoginUrl=null;
//		 if(inv.getRequest().getAttribute(LoginParamDef.ATTRIBUTE_EMAIL_LOGIN_URL)!=null){
//         	emailLoginUrl=(String) inv.getRequest().getAttribute(LoginParamDef.ATTRIBUTE_EMAIL_LOGIN_URL);
//         }
//		 isFirst=append(sb,"emailLoginUrl", emailLoginUrl, isFirst, true);
		String url= sb.toString();
		return url;
	}
	/**
	 * 拼接url
	 * @param sb
	 * @param name
	 * @param str
	 * @param isFirst
	 * @param needEncode
	 * @return
	 */
	protected boolean append(StringBuilder sb, String name, String str, boolean isFirst, boolean needEncode){
		if(!StringUtils.isEmpty(str)){
			String tmp=str;
			if(needEncode){
				tmp=URLEncoder.encodeUtf8(str);
			}
			tmp=name+"="+tmp;
			if(isFirst){
				sb.append("?"+tmp);
				isFirst=false;
			}else{
				sb.append("&"+tmp);
			}
		}
		return isFirst;
	}

}
