package com.dodoyo.invite.core.logic;

import com.dodoyo.invite.core.Constants;
import com.xiaonei.platform.core.opt.OpiConstants;

import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.Element;
import org.dom4j.io.SAXReader;

import java.io.File;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class LinkConfUtils {
    //pay attention to these init order, more important

    private static Map<String, String> appLinkSsMap = new HashMap<String, String>();
    private static Map<String, String> appLinkSsMap2 = new HashMap<String, String>();

    private static Map<String, String> linkSsMap = new HashMap<String, String>();
    private static Map<String, String> linkSsMap2 = new HashMap<String, String>();

    private static Document document = null;
    private static Document document2 = null;

    private static final String RENREN = "renren.com";
    private static final String KAIXIN = "kaixin.com";
    
    private static LinkConfUtils lcu = new LinkConfUtils();

    public LinkConfUtils() {
        init();
        init2();
    }

    public static LinkConfUtils getInstance() {
        return lcu;
    }

    private void init() {
        SAXReader reader = new SAXReader();
        try {
            document = reader.read(new File(Constants.LINK_CONFIG_PATH));
        }
        catch (DocumentException e) {
            LogUtils.logExceptionStack(e);
            e.printStackTrace();
            return;
        }
        Element rootElement = document.getRootElement();
        List<?> ssList = rootElement.element("common").elements("ss");
        Element ssElement = null;
        for (int i = 0; i < ssList.size(); i++) {
            ssElement = (Element) ssList.get(i);
            LogUtils.logDebug(ssElement.attributeValue("name"));
            LogUtils.logDebug(ssElement.attributeValue("value"));
            linkSsMap.put(ssElement.attributeValue("name"), ssElement.attributeValue("value"));
        }

        ssList = rootElement.element("app").elements("ss");
        ssElement = null;
        for (int i = 0; i < ssList.size(); i++) {
            ssElement = (Element) ssList.get(i);
            appLinkSsMap.put(ssElement.attributeValue("name"), ssElement.attributeValue("value"));
        }
        LogUtils.logDebug("link conf init over ....document == null ? " + (document == null));
    }
    
    private void init2() {
        SAXReader reader = new SAXReader();
        try {
            document2 = reader.read(new File(Constants.LINK_CONFIG_PATH2));
        }
        catch (DocumentException e) {
            LogUtils.logExceptionStack(e);
            e.printStackTrace();
            return;
        }
        Element rootElement = document2.getRootElement();
        List<?> ssList = rootElement.element("common").elements("ss");
        Element ssElement = null;
        for (int i = 0; i < ssList.size(); i++) {
            ssElement = (Element) ssList.get(i);
            LogUtils.logDebug(ssElement.attributeValue("name"));
            LogUtils.logDebug(ssElement.attributeValue("value"));
            linkSsMap2.put(ssElement.attributeValue("name"), ssElement.attributeValue("value"));
        }

        ssList = rootElement.element("app").elements("ss");
        ssElement = null;
        for (int i = 0; i < ssList.size(); i++) {
            ssElement = (Element) ssList.get(i);
            appLinkSsMap2.put(ssElement.attributeValue("name"), ssElement.attributeValue("value"));
        }
        LogUtils.logDebug("2 link conf init over ....document2 == null ? " + (document2 == null));
    }

    public String getInviteBaseLink() {
    	if (OpiConstants.domain.toString().equalsIgnoreCase(KAIXIN))
    		return getInviteBaseLink2();
        if (document == null) {
            LogUtils.logErr("document is null, return null for getInviteBaseLink()");
            return null;
        }
        Element rootElement = document.getRootElement();
        return rootElement.elementText("base_link");
    }
    
    private String getInviteBaseLink2() {
        if (document2 == null) {
            LogUtils.logErr("document2 is null, return null for getInviteBaseLink()");
            return null;
        }
        Element rootElement = document2.getRootElement();
        return rootElement.elementText("base_link");
    }

    public String getRt() {
    	if (OpiConstants.domain.toString().equalsIgnoreCase(KAIXIN))
    		return getRt2();
        if (document == null) {
            LogUtils.logErr("document is null, return null for getRt()");
            return null;
        }
        Element rootElement = document.getRootElement();
        return rootElement.element("common").elementText("rt");
    }

    private String getRt2() {
        if (document2 == null) {
            LogUtils.logErr("document2 is null, return null for getRt()");
            return null;
        }
        Element rootElement = document2.getRootElement();
        return rootElement.element("common").elementText("rt");
    }
    
    public String getAppRt() {
    	if (OpiConstants.domain.toString().equalsIgnoreCase(KAIXIN))
    		return getAppRt2();
        if (document == null) {
            LogUtils.logErr("document is null, return null for getAppRt()");
            return null;
        }
        Element rootElement = document.getRootElement();
        return rootElement.element("app").elementText("rt");
    }
    
    private String getAppRt2() {
        if (document2 == null) {
            LogUtils.logErr("document2 is null, return null for getAppRt()");
            return null;
        }
        Element rootElement = document2.getRootElement();
        return rootElement.element("app").elementText("rt");
    }

    public String getLinkSs(String key) {
    	if (OpiConstants.domain.toString().equalsIgnoreCase(RENREN))
    		return linkSsMap.get(key);
    	return linkSsMap2.get(key);
    }

    public String getAppLinkSs(String key) {
    	if (OpiConstants.domain.toString().equalsIgnoreCase(RENREN))
    		return appLinkSsMap.get(key);
    	return appLinkSsMap2.get(key);
    }

    public Map<String, String> getLinkSsMap() {
    	if (OpiConstants.domain.toString().equalsIgnoreCase(RENREN))
    		return linkSsMap;
    	return linkSsMap2;
    }

    public Map<String, String> getAppLinkSsMap() {
    	if (OpiConstants.domain.toString().equalsIgnoreCase(RENREN))
    		return appLinkSsMap;
    	return appLinkSsMap2;
    }

    public static void main(String[] args) {
        String str = LinkConfUtils.getInstance().getInviteBaseLink();
        LogUtils.logDebug(str);
    }

}