package com.xiaonei.reg.guide.action.netbarflow;

import javax.servlet.http.HttpServletRequest;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserBasic;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.reg.guide.action.a090729.MyTimeCost;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

public class NetbarUtil {
	public static void setInitAttribute(HttpServletRequest request){
		GuideLogger.printLog(" - start ");
		MyTimeCost cost = MyTimeCost.begin();
		//保存基本的 年龄和名字
		User host = BaseThreadUtil.currentHost(request);
		try {
			UserBasic ub = SnsAdapterFactory.getUserBasicAdapter().get(host.getId());
			request.setAttribute("user_name", host.getName());
			request.setAttribute("user_age", ub.getBirthYear());
			
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" set name or age error "+e.toString(),GuideLogger.ERROR);
			
		}
		
		//看是否是17岁以上，关系到引导导航条
		try {
			Boolean is_less_17 = false;
			cost.endStep();
			request.setAttribute("is_less_17", is_less_17);
			
			Boolean is_app_guide = false;
			request.setAttribute("is_app_guide", is_app_guide);
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" set 2 error "+e.toString(),GuideLogger.ERROR);
		}
		cost.endStep();
		cost.endFinally();
		
		GuideLogger.printLog(" - end ");
	}

}
