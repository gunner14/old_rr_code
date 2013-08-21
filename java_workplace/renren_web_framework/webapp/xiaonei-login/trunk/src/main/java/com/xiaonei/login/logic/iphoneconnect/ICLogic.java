package com.xiaonei.login.logic.iphoneconnect;

import com.xiaonei.login.constants.LoginStatusCode;

import net.sf.json.JSONObject;

public interface ICLogic {

	public JSONObject getJSONObject(LoginStatusCode failnopost, String email);

	public String native2ascii(String src);

}
