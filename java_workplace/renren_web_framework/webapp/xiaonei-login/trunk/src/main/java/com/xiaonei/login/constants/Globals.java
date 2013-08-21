package com.xiaonei.login.constants;

import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.xce.passport.Login;

public abstract class Globals {

    public static final String RENREN_DOMAIN = "renren.com";

    public static final String KAIXIN_DOMAIN = "kaixin.com";

    public static final String IMOP_DOMAIN = "imop.com";

    public static final String C56_DOMAIN = "56.com";

    public static final String PASSPORT_OLD_DOMAIN = "passport."
            + OpiConstants.domain.toString();

    public static final String PASSPORT_DOMAIN = RENREN_DOMAIN;

    public static final String PASSPORT_3RD_DOMAIN = PASSPORT_DOMAIN;
    
    public static final String advertiseStartTime = "advertiseStartTime";
	
	public static final String advertiseEndTime   = "advertiseEndTime";
	
	public static final String TIME_OUT_VALUE     = "timeOutValue";

    public static final List<Login.Type> defaultLogin = new ArrayList<Login.Type>();

    public static final HashMap<String, Boolean> WARNING_USERNAME = new HashMap<String, Boolean>();

    public static String localIp = "0.0.0.0";

    static {
        defaultLogin.add(Login.Type.Account);
        defaultLogin.add(Login.Type.Phone);
        defaultLogin.add(Login.Type.BindedId);
        //		defaultLogin.add(Login.Type.Id);

        try {
            localIp = InetAddress.getLocalHost().getHostAddress();
        } catch (final UnknownHostException e) {
            e.printStackTrace();
        }
    }
}
