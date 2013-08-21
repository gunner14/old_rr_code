package com.xiaonei.reg.guide.flows.xfive.utils;

import javax.servlet.http.HttpServletRequest;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideTimeCost;

public class GuideXFiveUtil {
	public static void setInitAttributes(HttpServletRequest request){
		GuideLogger.printLog("setInitAttributes() -- start ");
		//保存基本的 年龄和名字
		GuideTimeCost cost = GuideTimeCost.begin();
		User host = BaseThreadUtil.currentHost(request);
		request.setAttribute("user_name", host.getName());
		
		cost.endFinally();
		GuideLogger.printLog("setInitAttributes() -- end ");
	}
}
