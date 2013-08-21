package com.xiaonei.reg.guide.action.a090729;

import java.util.Enumeration;
import java.util.HashMap;
import java.util.Map;
import java.util.ResourceBundle;

import javax.servlet.http.HttpServletRequest;

import org.apache.commons.lang.StringUtils;

import com.dodoyo.opt.DodoyoResource;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserBasic;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.service.PlatformServiceHome;
import com.xiaonei.reg.common.RunnableTaskExecutor;
import com.xiaonei.reg.common.constants.IRegFromSource;
import com.xiaonei.reg.guide.logic.search.GuideFriendNewsLogic;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.register.logic.additional.BirthdayLogic;
import com.xiaonei.reg.register.logic.pretreatment.RegFromSourceLogic;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

public class A090729Util {

	public static void reloadProperties(){
		GuideLogger.printLog("reloadProperties() - start");
//		Properties props = new Properties();
		pageMap = new HashMap<String, String>();
		urlMap = new HashMap<String, String>();
		appMap = new HashMap<String, String>();
		DodoyoResource.close("pageids");
		DodoyoResource.init("pageids");
		ResourceBundle resourceBundle = DodoyoResource.getResourceBundle("pageids");

		Enumeration<?> es = resourceBundle.getKeys();
		while (es.hasMoreElements()) {
			String key = (String) es.nextElement();
			String value = null;
			try {
				value = resourceBundle.getString(key);
			} catch (Exception e) {
//				MyLogger.printLog("reloadProperties() value is null - key is:"+key, MyLogger.ERROR);
				System.err.println("A090729Util::reloadProperties() value is null - key is:"+key);
			}
			if (value == null) {

			}
			if (key.indexOf("page.") >= 0) {
				pageMap.put(key, value);
			}
			if (key.indexOf("url.") >= 0) {
				urlMap.put(key, value);
			}
			if (key.indexOf("app.") >= 0) {
			    appMap.put(key, value);
            }

			GuideLogger.printLog("reloadProperties() - " + key + "=" + value);
		}
			
//			try {
//				InputStream ips = new BufferedInputStream(new FileInputStream("pageids.properties"));
//				props.load(ips);
//		}
//		catch (FileNotFoundException e) {
//			e.printStackTrace();
//		} catch (IOException e) {
//			e.printStackTrace();
//		}
	}
	static {
		GuideLogger.printLog("... map reloading");
		reloadProperties();
		GuideLogger.printLog("... map reloaded");
	}
	
	
	static private Map<String,String> pageMap = null;
	static private Map<String,String> urlMap = null;
	static private Map<String,String> appMap = null;
	
	private static Map<String,String> getUrlMap(){
		if(urlMap == null){
			reloadProperties();
		}
		return urlMap;
	}
	
	private static Map<String,String> getPageMap(){
		if(pageMap == null){
			reloadProperties();
		}
		return pageMap;
	}
	
	private static Map<String,String> getAppMap(){
        if(appMap == null){
            reloadProperties();
        }
        return appMap;
    }
	
	
	public static String getAppUrl(String key){
	    GuideLogger.printLog("key:"+key);
	    String ret = null;
	    if(getAppMap()!=null){
	        String urlkey = getAppMap().get(key);
	        if(!MyStringUtils.isNullOrEmpty(urlkey)) {
                GuideLogger.printLog("getAppUrl() key:"+key+" urlkey:"+urlkey+" value:"+getUrlMap().get(urlkey));
                ret = getUrlMap().get(urlkey);
            }
            else{
                GuideLogger.printLog("getAppUrl() null");
            }
	    }
	    return ret;
	}
	
	public static String getUrl(String key){
		GuideLogger.printLog("getUrl() - start - key is  :"+key);
		String urlkey = "";
		if(getPageMap()!=null){
			urlkey = getPageMap().get(key);
			if(getUrlMap() != null) {
				GuideLogger.printLog("getUrl() - end - key is  :"+key+"|value:"+getUrlMap().get(urlkey));
				return getUrlMap().get(urlkey);
			}
			else{
				GuideLogger.printLog("getUrlMap() null");
			}
		}
		else{
			GuideLogger.printLog("getPageMap() null");
		}
		
		return null;
	}
	/**
	 * 看是否通过 pageid 能读出url来！
	 * @param pageid
	 * @return
	 */
	public static boolean hasPageKey(String pageid){
		GuideLogger.printLog("hasPageKey() pageid is :"+pageid);
		if(StringUtils.isEmpty(pageid)){
			GuideLogger.printLog("hasPageKey() pageid is empty:"+pageid);
			return false;
		}
		if(getPageMap() == null){
			GuideLogger.printLog("hasPageKey() getPageMap() is null:"+pageid);
			return false;
		}
		if(getPageMap().containsKey("page."+pageid)){
			GuideLogger.printLog("hasPageKey() pageid has! "+pageid);
			return true;
		}
		GuideLogger.printLog("hasPageKey() false -- pageid is :"+pageid);
		return false;
		
	}
	private static boolean isConfiguredChannal(String fromSource){
		GuideLogger.printLog("isConfiguredChannal() - fromSource:" + fromSource);
		if(fromSource == null) return false;
		return A090729Util.hasPageKey(fromSource.trim());
	}
	
	private static boolean hasFromSource(String fromSource){
		GuideLogger.printLog("hasFromSource() - fromSource:" + fromSource);
		if(fromSource == null) return false;
		if("".equals(fromSource.trim())) return false;
		return true;
	}
	
	@SuppressWarnings("unused")
	private static boolean isChannnelUser(User host){
		try {
			GuideLogger.printLog(" host:"+host.getId());
			String fromSource = RegFromSourceLogic.getFromSource(host);
			GuideLogger.printLog("isChannnelUser() 1 - host:" + host.getId()+"fromSource:"+fromSource+" IRegFromSource.S_CHANNALDOG:"+IRegFromSource.S_CHANNALDOG);
			if (hasFromSource(fromSource)){
				if(isConfiguredChannal(fromSource)){
					GuideLogger.printLog("isChannnelUser() - host:" + host.getId()+" "+fromSource);
					GuideLogger.printLog(" must not here! host:"+host.getId(),GuideLogger.FATAL);
					return true;
				}
			}
			GuideLogger.printLog("isChannnelUser() 2 - host:" + host.getId()+"fromSource:"+fromSource+" IRegFromSource.S_CHANNALDOG:"+IRegFromSource.S_CHANNALDOG);
			if(StringUtils.equals(fromSource, IRegFromSource.S_CHANNALDOG)){
				GuideLogger.printLog("isChannnelUser() - host:" + host.getId()+" true because of S_CHANNALDOG");
				GuideLogger.printLog(" must not here! host:"+host.getId(),GuideLogger.FATAL);
				return true;
			}
			
			if(StringUtils.equals(fromSource, "INSHION")){
				GuideLogger.printLog("isChannnelUser() - host:" + host.getId()+" true because of INSHION");
				GuideLogger.printLog(" must not here! host:"+host.getId(),GuideLogger.FATAL);
				return true;
			}
		} catch (Exception e) {
			GuideLogger.printLog("isChannnelUser() -- error -- host:"+host.getId()+" " + e.toString(),GuideLogger.ERROR);
		}
		return false;
	}
	
	@SuppressWarnings("unused")
	private static void sendNewEvent(final User host){
		GuideLogger.printLog("sendNewEvent() - start - "+"host:"+host.getId());
		//开心的别发新鲜事
		if(!"renren.com".equals(OpiConstants.domain.toString()))return;

		MyTimeCost cost = MyTimeCost.begin();
		Runnable taskSaveFans = new Runnable() {
			public void run() {
				MyTimeCost cost = MyTimeCost.begin();
				try {
					GuideLogger.printLog("Telling News Feed!");
					GuideFriendNewsLogic.getInstance().tellAll(host);
					//MyLogger.printLog(" Need Tell News Feed!",1);
				} catch (Exception e) {
					GuideLogger.printLog("sendNewEvent() - "+"host:"+host.getId()+e.toString(),1);
					e.printStackTrace();
				}
				cost.endFinally();
			}
		};
		RunnableTaskExecutor.getInstance().runTask(taskSaveFans);
		cost.endFinally();
	}
	
	public static void setInitAttributes(HttpServletRequest request){
		GuideLogger.printLog("setInitAttributes() -- start ");
		//保存基本的 年龄和名字
		MyTimeCost cost = MyTimeCost.begin();
		User host = BaseThreadUtil.currentHost(request);
		UserBasic ub = SnsAdapterFactory.getUserBasicAdapter().get(host.getId());
		request.setAttribute("user_name", host.getName());
		request.setAttribute("user_age", ub.getBirthYear());
		
		//看是否是ID注册，如果是ID注册 就去掉EMAIL的属性
		String email = host.getAccount();
		if(email == null){
			email = "";
		}
		else{
			if(email.indexOf("@")>=0){
				//这是ID注册的
				
			}
			else{
				email = "";
			}
		}
		request.setAttribute("user_email", email);
		cost.endStep();

		//看是否是17岁以上，关系到引导导航条
		Boolean is_less_17 = false;
		try {
			is_less_17 = BirthdayLogic.getInstance().less17age(request);
		} catch (Exception e) {
			GuideLogger.printLog("setInitAttributes() -- error 1 -- "+host.getId(),1);
		}

		cost.endStep();
		request.setAttribute("is_less_17", is_less_17);
		
		Boolean is_app_guide = false;
		
		//2009-12-21 17:23:27 发现这个没用了
//		try {
//			if(isChannnelUser(host)){
//				is_app_guide = true;
//			}
//			cost.endStep();
//		} catch (Exception e) {
//			GuideLogger.printLog("setInitAttributes() -- error 2",1);
//		}
		cost.endStep();
		GuideLogger.printLog("setInitAttributes() -- is_app_guide="+is_app_guide);
		request.setAttribute("is_app_guide", is_app_guide);
		cost.endFinally();
		GuideLogger.printLog("setInitAttributes() -- end ");
	}
	public static void asyncInstallFarm(final User host) {
		GuideLogger.printLog("start - host:"+host.getId());
		Runnable task = new Runnable(){
			@Override
			public void run() {
				try {
					GuideLogger.printLog("doing - host:"+host.getId()+"-"+23163);
					PlatformServiceHome.getInstance().installApp(host.getId(),23163);
				} catch (Exception e) {
					GuideLogger.printLog("error - host:"+host.getId(),GuideLogger.DEBUG);
				}
			}
		};
		RunnableTaskExecutor.getInstance().runTask(task);
	}
	
	public static void asyncInstallCook(final User host) {
		GuideLogger.printLog("start - host:"+host.getId());
		Runnable task = new Runnable(){
			@Override
			public void run() {
				try {
					GuideLogger.printLog("doing - host:"+host.getId()+"-"+44097);
					PlatformServiceHome.getInstance().installApp(host.getId(),44097);
				} catch (Exception e) {
					GuideLogger.printLog("error - host:"+host.getId(),GuideLogger.DEBUG);
				}
			}
			
		};
		RunnableTaskExecutor.getInstance().runTask(task);
	}
}
