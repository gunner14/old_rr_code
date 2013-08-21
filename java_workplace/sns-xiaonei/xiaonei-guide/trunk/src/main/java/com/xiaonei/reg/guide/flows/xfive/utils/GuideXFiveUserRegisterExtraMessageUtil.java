package com.xiaonei.reg.guide.flows.xfive.utils;

import java.util.Map;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.register.logic.IRegExtraMsgLogic;
import com.xiaonei.reg.register.logic.additional.RegExtraMsgLogic;
import com.xiaonei.reg.register.model.RegExtraMsg;

public class GuideXFiveUserRegisterExtraMessageUtil {
	private static Map<String, RegExtraMsg> getExtraMap(User host){
		Map<String, RegExtraMsg> map = null;
		if(host == null){
			GuideLogger.printLog(" host null!",GuideLogger.WARN);
		}
		else{
			try {
				map = RegExtraMsgLogic.getInstance().loadExtraMsgMap(host);
			} catch (Exception e) {
				GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
				e.printStackTrace();
			}
			
		}
		return map;
		
	}
	
	public static int getInvitedAppid(User host){
		int ret = 0;
		Map<String, RegExtraMsg> map = getExtraMap(host);
		if(host == null){
			GuideLogger.printLog(" host null!",GuideLogger.WARN);
		}
		else if(map == null){
			GuideLogger.printLog(" host:"+host.getId()+" map null");
		}
		else{
			RegExtraMsg appidobj = map.get(IRegExtraMsgLogic.key_reg_app_id);
			if(appidobj == null){
				GuideLogger.printLog(" host:"+host.getId()+" obj null");
			}
			else{
				String appidstr = appidobj.getInput_value();
				if(appidstr == null){
					GuideLogger.printLog(" host:"+host.getId()+" string null",GuideLogger.WARN);
				}
				else{
					try {
						ret = Integer.parseInt(appidstr);
					} catch (Exception e) {
						GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
						e.printStackTrace();
					}
				}
			}
		}
		return ret;
	}
	
	public static int getConnectedAppid(User host){
		int ret = 0;
		Map<String, RegExtraMsg> map = getExtraMap(host);
		if(host == null){
			GuideLogger.printLog(" host null!",GuideLogger.WARN);
		}
		else if(map == null){
			GuideLogger.printLog(" host:"+host.getId()+" map null");
		}
		else{
			RegExtraMsg appidobj = map.get(IRegExtraMsgLogic.key_reg_app_connect);
			if(appidobj == null){
				GuideLogger.printLog(" host:"+host.getId()+" obj null");
			}
			else{
				String appidstr = appidobj.getInput_value();
				if(appidstr == null){
					GuideLogger.printLog(" host:"+host.getId()+" string null",GuideLogger.WARN);
				}
				else{
					try {
						ret = Integer.parseInt(appidstr);
					} catch (Exception e) {
						GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
						e.printStackTrace();
					}
				}
			}
		}
		return ret;
	}
	
	public static String getRecommendname(User host){
		String ret = "";
		Map<String, RegExtraMsg> map = getExtraMap(host);
		if(host == null){
			GuideLogger.printLog(" host null!",GuideLogger.WARN);
		}
		else if(map == null){
			GuideLogger.printLog(" host:"+host.getId()+" map null");
		}
		else{
			RegExtraMsg nameobj = map.get(IRegExtraMsgLogic.key_reg_recommond);
			if(nameobj == null){
				GuideLogger.printLog(" host:"+host.getId()+" obj null");
			}
			else{
				String appidstr = nameobj.getInput_value();
				if(appidstr == null){
					GuideLogger.printLog(" host:"+host.getId()+" string null",GuideLogger.WARN);
				}
			}
		}
		return ret;
	}
}
