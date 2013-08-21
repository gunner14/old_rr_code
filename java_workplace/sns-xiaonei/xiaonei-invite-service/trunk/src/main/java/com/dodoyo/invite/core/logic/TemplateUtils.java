package com.dodoyo.invite.core.logic;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.StringWriter;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Properties;
import java.util.Map.Entry;

import org.apache.velocity.Template;
import org.apache.velocity.VelocityContext;
import org.apache.velocity.app.VelocityEngine;
import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.Element;
import org.dom4j.io.SAXReader;
import org.springframework.beans.factory.annotation.Autowired;

import com.dodoyo.invite.core.Constants;
import com.dodoyo.invite.core.entity.InviteParams;
import com.dodoyo.invite.core.entity.TemplateContent;
import com.xiaonei.platform.core.opt.OpiConstants;

public class TemplateUtils {
    
    @Autowired
    private InviteParamsUtils inviteParamsUtils;
    
    private static TemplateUtils tu = new TemplateUtils();

    public TemplateUtils() {
        init();
    }

    private List<TemplateContent> tcList = null;
    private List<TemplateContent> tcList2 = null;
    private Map<String, Template> vmMap = new HashMap<String, Template>();

    private void init() {
        LogUtils.logDebug("TemplateUtils begin to init ....");
        initTemplateList();
        initTemplateList2();
        initVms();
        LogUtils.logDebug("TemplateUtils init over ....");
    }

    private void initVms() {
        FileInputStream fis = null;
        try {
            SAXReader reader = new SAXReader();
            VelocityEngine ve = new VelocityEngine();
            Properties p = new Properties();
            fis = new FileInputStream(new File(Constants.REPLOAD_VELOCITY_PROPERTIES));
            p.load(fis);
            ve.init(p);
            Document doc = reader.read(new FileInputStream(new File(Constants.RELOAD_VELOCITY_TEMPALATE_XML)));
            Element rootElement = doc.getRootElement();
            String id = null;
            String value = null;
            Element cateElement = null;
            List<?> cateElementList = rootElement.elements("category");
            for (int i = 0; i < cateElementList.size(); i++) {
                cateElement = (Element) cateElementList.get(i);
                id = cateElement.attributeValue(Constants.VELOCITY_CONFIG_FILE_ID);
                value = cateElement.attributeValue(Constants.VELOCITY_CONFIG_FILE_VALUE);
                vmMap.put(id, ve.getTemplate(value));
                LogUtils.logDebug("fill vmMap, id = " + id + ", value = " + value);
            }
        }
        catch (Exception e) {
            LogUtils.logExceptionStack(e);
            LogUtils.logErr("initVms failure ....");
        }
        finally
        {
            if (fis != null)
            {
                try {
                    fis.close();
                } catch (IOException e) {
                    e.printStackTrace(System.err);
                }
            }
        }
    }

    private void initTemplateList() {
        SAXReader reader = new SAXReader();
        Document document = null;
        try {
            document = reader.read(new File(Constants.EMAIL_CONFIG_PATH));
        }
        catch (DocumentException e) {
            e.printStackTrace();
            return;
        }
        List<TemplateContent> contentList = new ArrayList<TemplateContent>();
        Element rootElement = document.getRootElement();
        List<?> contentElementList = rootElement.elements("content");
        if (contentElementList == null || contentElementList.size() == 0)
            return;

        Element contentElement = null;
        TemplateContent tc = null;
        for (int i = 0; i < contentElementList.size(); i++) {
            contentElement = (Element) contentElementList.get(i);
            tc = new TemplateContent();
            tc.setAppId(contentElement.elementText("appid"));
            tc.setBody(contentElement.elementText("body"));
            tc.setContentId(contentElement.attributeValue("id"));
            tc.setDisc(contentElement.elementText("disc"));
            tc.setLink(contentElement.elementText("link"));
            tc.setRt(contentElement.elementText("rt"));
            tc.setSourceId(contentElement.elementText("source_id"));
            tc.setTitle(contentElement.elementText("title"));
            tc.setVmTemplate(contentElement.elementText("template"));
            tc.setSender(contentElement.elementText("sender"));
            contentList.add(tc);
        }
        tcList = contentList;
    }
    
    private void initTemplateList2() {
        SAXReader reader = new SAXReader();
        Document document = null;
        try {
            document = reader.read(new File(Constants.EMAIL_CONFIG_PATH2));
        }
        catch (DocumentException e) {
            e.printStackTrace();
            return;
        }
        List<TemplateContent> contentList = new ArrayList<TemplateContent>();
        Element rootElement = document.getRootElement();
        List<?> contentElementList = rootElement.elements("content");
        if (contentElementList == null || contentElementList.size() == 0)
            return;

        Element contentElement = null;
        TemplateContent tc = null;
        for (int i = 0; i < contentElementList.size(); i++) {
            contentElement = (Element) contentElementList.get(i);
            tc = new TemplateContent();
            tc.setAppId(contentElement.elementText("appid"));
            tc.setBody(contentElement.elementText("body"));
            tc.setContentId(contentElement.attributeValue("id"));
            tc.setDisc(contentElement.elementText("disc"));
            tc.setLink(contentElement.elementText("link"));
            tc.setRt(contentElement.elementText("rt"));
            tc.setSourceId(contentElement.elementText("source_id"));
            tc.setTitle(contentElement.elementText("title"));
            tc.setVmTemplate(contentElement.elementText("template"));
            tc.setSender(contentElement.elementText("sender"));
            contentList.add(tc);
        }
        tcList2 = contentList;
    }

    private static TemplateUtils getInstance() {
        return tu;
    }

    //public method
    public InviteParams updateInviteParamsByTemplateId(InviteParams ip) {
        String templateId = ip.getTemplateId();

        TemplateContent tc = getTemplateContentByTemplateId(templateId);

        //confirm the email template
        if (tc == null) {
            if (inviteParamsUtils.isRealAppId(ip.getAppId())) {
                tc = getTemplateContentByTemplateId(ip.getAppId());
                if (tc == null)
                    tc = getTemplateContentByTemplateId(Constants.DEFULT_TEMPLATE_FOR_APP);
            } else
                tc = getTemplateContentByTemplateId(Constants.DEFULT_TEMPLATE_FOR_COMMON);
        }

        ip.setTemplateContent(new TemplateContent(tc));

        return ip;
    }

    /**
     * get templateContent from xml configurations
     *
     * @param templateId
     * @return
     */
    @SuppressWarnings("deprecation")
    public TemplateContent getTemplateContentByTemplateId(String templateId) {
        if (templateId == null)
            return null;

        //check is mutiple templates or not
        List<TemplateContent> specTemplateContentList = this.getTemplateContentsByTemplateId(templateId);
        if (specTemplateContentList == null)
            return null;
        int specSize = specTemplateContentList.size();
        if (specSize > 0) {
            if (specSize == 1)
                return specTemplateContentList.get(0);
            Date d = new Date();
            return specTemplateContentList.get(d.getSeconds() % specSize);
        }

//		List<TemplateContent> templateContentList = getTemplateContentList();
//		if (templateContentList == null || templateContentList.size() == 0)
//			return null;
//		
//		for (TemplateContent tc : templateContentList)
//		{
//			if (templateId.equals(tc.getContentId()))
//				return tc;
//		}

        return null;
    }

    /**
     * get templateContent from xml configurations
     *
     * @param templateId
     * @return
     */
    public List<TemplateContent> getTemplateContentsByTemplateId(String templateId) {
        List<TemplateContent> specTemplateList = new ArrayList<TemplateContent>();

        if (templateId == null)
            return null;

        List<TemplateContent> templateContentList = getTemplateContentList();
        if (templateContentList == null || templateContentList.size() == 0)
            return null;

        for (TemplateContent tc : templateContentList) {
            if (templateId.equals(tc.getContentId()))
                specTemplateList.add(tc);
        }

        return specTemplateList;
    }

    /**
     * 从配置文件中获取template content list
     *
     * @return
     */
    public List<TemplateContent> getTemplateContentList() {
    	if (OpiConstants.domain.toString().equalsIgnoreCase("renren.com"))
    		return tcList;
        return tcList2;
    }

    public Template getVMTemplate(String templateVM) {
        return vmMap.get(templateVM);
    }

    public String getVMContent(Map<String, Object> datas, Template t) {
        StringWriter writer = new StringWriter();
        VelocityContext context = new VelocityContext();
        if (datas != null) {
            /* instead by following codes checked by Findbug
             * Set<String> set = datas.keySet();
            for (String s : set) {
                context.put(s, datas.get(s));
            }*/
            for (Entry<String, Object> e : datas.entrySet()) {
                context.put(e.getKey(), e.getValue());
            }
        }
        try {
            if (t != null)
                t.merge(context, writer);
        }
        catch (Exception e) {
            e.printStackTrace(System.err);
        }
        return writer.toString();
    }

    public static void main(String[] args) {
        List<TemplateContent> list = TemplateUtils.getInstance().getTemplateContentList();
        for (TemplateContent tc : list) {
            System.out.println(tc);
        }

        TemplateContent tc = TemplateUtils.getInstance().getTemplateContentByTemplateId("19974");
		System.out.println(tc);
	}
}