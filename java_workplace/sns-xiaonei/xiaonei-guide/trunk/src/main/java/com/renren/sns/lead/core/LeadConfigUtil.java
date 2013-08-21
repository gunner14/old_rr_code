package com.renren.sns.lead.core;

import java.util.Enumeration;
import java.util.HashMap;
import java.util.Map;
import java.util.ResourceBundle;

import org.apache.log4j.Logger;

import com.dodoyo.opt.DodoyoResource;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.util.GuideLogger;



public class LeadConfigUtil {
	private static Logger inshion_log = Logger.getLogger("com.inshion.MyLogger");
    private static Map<String,String> map = new HashMap<String,String>();
	public static void reloadConfigMap(){
		DodoyoResource.close("leadconfig");
		DodoyoResource.init("leadconfig");
		ResourceBundle resourceBundle = DodoyoResource.getResourceBundle("leadconfig");
		map = new HashMap<String, String>();
		Enumeration<?> es = resourceBundle.getKeys();
		while (es.hasMoreElements()) {
			String key = (String) es.nextElement();
			String value = null;
			try {
				value = resourceBundle.getString(key);
			} catch (Exception e) {
				e.printStackTrace();
			}
			
			if (value == null) {
				//
			}
			else{
				map.put(key, value);
				inshion_log.info("MyLogger::leadconfig.properties reloaded:"+"<"+key+","+value+">");
			}
		}
	}
	static{
		reloadConfigMap();
	}
	
	public static void main(String[] args) {
		//reloadConfigMap();
		//String a = LeadConfigUtil.getPageUri(new User(), "_aat", "fillinfo");
		//System.out.println("--->"+a);
		map.get(null);
		String coreflag = "_AAS";
		String flowId = map.get("flag."+coreflag+".flow"); //flag._AAS.flow or null
		String stepperId = map.get("flow."+flowId+".stepper");
		String ret = map.get(stepperId);
		System.out.println(flowId+":"+ret);
	}

	public static String getStepperName(User host, String fromsource) {
		String coreflag = LeadCoreLogic.getInstance().getLeadCoreFlag(host, fromsource);
		
		String flowidkey = "flag."+coreflag+".flow";
		String flowId = map.get(flowidkey); //get flag._AAS.flow or null
		String stepperidkey = "flow."+flowId+".stepper";
		String stepperId = map.get(stepperidkey); //get Kouxiangtang
		
		String ret = stepperId;
		if(ret == null) ret = "";
		GuideLogger.printLog(" host:"+host.getId()+" flowidkey:"+flowidkey+" stepperidkey:"+stepperidkey+" ret:"+ret);
		return ret;
	}

	public static String getPageUri(User host, String fromsource, String stepstring) {
		String coreflag = LeadCoreLogic.getInstance().getLeadCoreFlag(host, fromsource);
		
		String flowidkey = "flag."+coreflag+".flow";
		String flowId = map.get(flowidkey); //get flag._AAS.flow or null
		
		String pageurikey = "flow."+flowId+".mapper."+stepstring+".pageuri";
		String pageuri = map.get(pageurikey);
		
		String ret = pageuri;
		if(ret == null) ret = "";
		GuideLogger.printLog(" host:"+host.getId()+" flowidkey:"+flowidkey+" pageurikey:"+pageurikey+" ret:"+ret);
		return ret;
	}
	
	public static String getPageInit(User host, String fromsource, String stepstring) {
		String coreflag = LeadCoreLogic.getInstance().getLeadCoreFlag(host, fromsource);
		
		String flowidkey = "flag."+coreflag+".flow";
		String flowId = map.get(flowidkey); //get flag._AAS.flow or null
		
		String pageinitkey = "flow."+flowId+".mapper."+stepstring+".pageinit";
		String pageuri = map.get(pageinitkey);
		
		String ret = pageuri;
		if(ret == null) ret = "";
		GuideLogger.printLog(" host:"+host.getId()+" flowidkey:"+flowidkey+" pageinitkey:"+pageinitkey+" ret:"+ret);
		return ret;
	}

}
