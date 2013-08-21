package com.xiaonei.login.logic.iphoneconnect;

import net.sf.json.JSONObject;

import org.springframework.stereotype.Service;

import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.logic.status.LoginStatusOpr;

@Service
public class ICLogicImpl implements ICLogic, LoginStatusOpr {

	@Override
	public String getErrorMessage(LoginStatusCode status) {
		if (status == LoginStatusCode.LOGIN_OK) {
			return "登录成功";
		} else if (status == LoginStatusCode.FAILUNKNOWN) {
			return "未知错误";
		} else if (status == LoginStatusCode.FAILINPUT) {
			return "用户输入不合法";
		} else if (status == LoginStatusCode.FAILBANIP) {
			return "用户当前登录IP被封禁";
		} else if (status == LoginStatusCode.FAILUSER) {
			return "用户输入的帐号或者密码不对";
		} else if (status == LoginStatusCode.FAILNOPOST) {
			return "非post访问提交页面";
		} else if (status == LoginStatusCode.FAILUSERBAN) {
			return "帐号被封禁";
		} else if (status == LoginStatusCode.FAILUSERACTIVE) {
			return "帐号未激活";
		} else if (status == LoginStatusCode.FAILUSERSUICIDE) {
			return "帐号已经注销";
		} else if (status == LoginStatusCode.FAILUSERNOTEXIST) {
			return "用户不存在";
		} else if (status == LoginStatusCode.FAILNOSAFEUSER) {
			return "用户存在安全问题";
		} else if (status == LoginStatusCode.FAILVALIDATELOGIN) {
			return "用户输入验证码错误";
		} else if (status == LoginStatusCode.FAILNOTIFYGRAYUSER) {
			return "用户为灰名单用户";
		} else if (status == LoginStatusCode.FAILNOTIFYGRAYIP) {
			return "用户在异常Ip登录";
		}
		return null;
	}

	@Override
	public JSONObject getJSONObject(LoginStatusCode statusCode, String email) {
		JSONObject json = new JSONObject();
		json.put("statusCode", statusCode);
		json.put("desc", native2ascii(getErrorMessage(statusCode)));
		json.put("account", email);
		return json;
	}

	@Override
	public String native2ascii(String src) {
		if (src == null)
			return src;
		String des = "";
		int length = src.length();
		for (int i = 0; i < length; i++) {
			char cvalue = (char) src.charAt(i);
			int ivalue = (int) src.charAt(i);
			if (ivalue >= 19968 && ivalue <= 171941) {
				des += "\\u" + Integer.toString(cvalue, 16);
			} else {
				des += cvalue;
			}
		}
		return des;
	}

}
