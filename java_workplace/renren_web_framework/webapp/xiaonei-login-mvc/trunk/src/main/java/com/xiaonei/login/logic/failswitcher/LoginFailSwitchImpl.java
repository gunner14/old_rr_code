package com.xiaonei.login.logic.failswitcher;

import java.util.HashSet;
import java.util.Set;

import org.apache.commons.lang.StringUtils;

import net.paoding.rose.web.Invocation;

import com.xiaonei.login.constants.LoginLoggerDef;
import com.xiaonei.login.constants.LoginParamDef;
import com.xiaonei.login.constants.LoginSourceType;
import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.logic.LoginLogicFactory;
import com.xiaonei.login.logic.failswitcher.nodes.ILoginFailSwitchNode;
import com.xiaonei.login.logic.logger.PpLogger;

public class LoginFailSwitchImpl implements ILoginFailSwitch {

	private Set<ILoginFailSwitchNode> switchNodes = new HashSet<ILoginFailSwitchNode>(
			0);

	private PpLogger remoteLogger = LoginLoggerDef.login;

	@Override
	public String failSwitch(String domain, Invocation inv,
			LoginStatusCode failCode) {
		LoginLogicFactory.getDomainOpr().setDomainEnv(inv.getRequest(), domain);
		ILoginFailSwitchNode defaultNode = null;
		inv.addModel(LoginParamDef.ATTRIBUTE_LOGIN_STATUS_CODE, failCode);
		//用户名/密码验证错误二次失败，那就验证码吧
//		if((LoginStatusCode.FAILUSER == failCode || LoginStatusCode.FAILVALIDATELOGIN == failCode) && "true".equals(inv.getRequest().getParameter(
//                LoginParamDef.PARAM_ISPLOGIN))){
//		    inv.getRequest().setAttribute("needValidate", true);
//		}
		String ip=inv.getRequest().getAttribute("CLIENT_IP")==null?"":String.valueOf(inv.getRequest().getAttribute("CLIENT_IP"));
		if (remoteLogger.isEnable()) {
			int type= -1;
			 String https="";
        	try{
        		https=inv.getParameter("https");
                if(StringUtils.isEmpty(https)){
                	https="";
                }
        		type=inv.getRequest().getAttribute(LoginParamDef.ATTRIBUTE_LOGIN_SOURCE)==null?-1:((LoginSourceType)inv.getRequest().getAttribute(LoginParamDef.ATTRIBUTE_LOGIN_SOURCE)).getValue();
        	}catch(Exception e){
        		e.printStackTrace();
        	}
			String referer = inv.getRequest().getHeader("Referer");
			String userName = inv.getRequest().getParameter(
					LoginParamDef.PARAM_EMAIL);
			remoteLogger.info(String.format(
					LoginLoggerDef.loginLogPentterNewSsl, failCode
							.name(), userName, referer,userName,ip,type, https));
		}
		for (ILoginFailSwitchNode node : this.switchNodes) {
			if (LoginStatusCode.FAILDEFAULT == node.getTag()) {
				defaultNode = node;
			}
			if (failCode == node.getTag()) {
				
				return node.fail(domain, inv, failCode);
			}
		}
		if (null != defaultNode) {
			return defaultNode.fail(domain, inv, failCode);
		}
		return ILoginFailSwitchNode.failUri;
	}

	@Override
	public void setUpSwitchNode(ILoginFailSwitchNode node) {
		if (null == node) {
			return;
		}
		this.switchNodes.add(node);
	}

	@Override
	public void removeSwitchNode(ILoginFailSwitchNode node) {
		this.switchNodes.remove(node);
	}

	@Override
	public boolean isEmpty() {
		return this.switchNodes.size() == 0;
	}
}
