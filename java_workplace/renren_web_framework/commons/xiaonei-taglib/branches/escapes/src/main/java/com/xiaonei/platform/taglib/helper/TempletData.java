package com.xiaonei.platform.taglib.helper;

import java.util.HashMap;
import java.util.Map;

public class TempletData{
	public static String Footer_LinkBar = "Footer_LinkBar";
	public static String Footer_Icp = "Footer_Icp";
	public static String Footer_Net_Police = "Footer_Net_Police";
	
	//下面是sidebar的部分
	public static String Sidebar_Wrapper_sidebar = "Sidebar_Wrapper_sidebar";
	public static String Sidebar_Wrapper_loginpanel = "Sidebar_Wrapper_loginpanel";
	public static String Sidebar_Login = "Sidebar_Login";
	public static String Sidebar_QuickLink = "Sidebar_QuickLink";
	public static String Sidebar_YouCan = "Sidebar_YouCan";

	public static String Sidebar_Search = "Sidebar_Search";
	public static String Sidebar_AdminMenu = "Sidebar_AdminMenu";
	public static String Sidebar_SuperAdminMenu = "Sidebar_SuperAdminMenu";
	
	//下面是navigation的部分
	public static String Navigation_Utility = "Navigation_Utility";
	public static String Navigation_NavLeft = "Navigation_NavLeft";
	public static String Navigation_NavRight = "Navigation_NavRight";//
	public static String Navigation_Replace_ImNotice = "Navigation_Replace_ImNotice";
	
	public static String Navigation_Replace_SuiBianKanKan = "Navigation_Replace_SuiBianKanKan";
	public static String Navigation_Replace_NewEvent = "Navigation_Replace_NewEvent";
	public static String Navigation_Replace_NewMessageCount = "Navigation_Replace_NewMessageCount";

	//下面是head的部分
	public static String Head_Meta = "Head_Meta";
	public static String Head_Css = "Head_Css";
	public static String Head_Javascript = "Head_Javascript";
	
	private Map map = new HashMap();
	
	private static TempletData instance = new TempletData();

	public static TempletData getInstance() {
		if (instance == null) {
			synchronized (TempletData.class) {
				if (instance == null)
					instance = new TempletData();
			}
		}
		return instance;
	}

	private TempletData(){
		super();
	}
	
	public String getMap(String templetKey) {
		return (String) map.get(templetKey);
	}

	public void setMap(String templetKey, String templetData) {
		this.map.put(templetKey, templetData);
	}
}
