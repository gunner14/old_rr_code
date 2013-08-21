package com.xiaonei.platform.core.domainIdentified;

import java.io.File;
import java.io.FileFilter;
import java.io.IOException;
import java.util.Arrays;
import java.util.Iterator;
import java.util.Properties;

import javax.servlet.Filter;
import javax.servlet.FilterChain;
import javax.servlet.FilterConfig;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.dom4j.Attribute;
import org.dom4j.Document;
import org.dom4j.Element;
import org.dom4j.io.SAXReader;

import com.xiaonei.platform.core.opt.OpiConstants;

public class DomainIdentifiedFilter implements Filter {

    private Log logger = LogFactory.getLog(DomainIdentifiedFilter.class);

    FilterConfig config;

    @Override
    public void init(FilterConfig config) throws ServletException {
        this.config = config;
        File file = new File(config.getServletContext().getRealPath("/WEB-INF"));
        if (file.exists()) {
            loadConfigs(config, file);
        }
    }

    private void loadConfigs(FilterConfig config, File file) throws ServletException {
        config.getServletContext().log("loading domainIdentified xml...");
        logger.info("loading domainIdentified xml...");
        File[] files = file.listFiles(new FileFilter() {

            @Override
            public boolean accept(File pathname) {
                return (pathname.isFile() && pathname.getName().startsWith("domainIdentified.") && pathname
                        .getName().endsWith(".xml"));
            }
        });
        config.getServletContext().log("found domainIdentified xml: " + Arrays.toString(files));
        logger.info("found domainIdentified xml: " + Arrays.toString(files));
        for (File xml : files) {
            String domain = xml.getName().substring("domainIdentified.".length(),
                    xml.getName().length() - ".xml".length());
            Properties properties;
            try {
                properties = loadXml(xml);
            } catch (Exception e) {
                if (config != null) {
                    config.getServletContext().log("", e);
                }
                logger.error("", e);
                throw new ServletException(e);
            }
            OpiConstants.addDomainIdentifiedProperties(domain, properties);
            config.getServletContext().log(
                    "add domainIdentified Properties: " + domain + " size=" + properties.size());
            logger
                    .info("add domainIdentified Properties: " + domain + " size="
                            + properties.size());
        }

    }

    /*-
    <domain-identified>
    <msg key="a"><value>at</value></msg>
    <msg key="b"><value>bt</value></msg>
    </domain-identified>
    */
    @SuppressWarnings("unchecked")
    private Properties loadXml(File xml) throws Exception {
        Properties p = new Properties();
        SAXReader reader = new SAXReader();
        Document doc = reader.read(xml);
        Element root = doc.getRootElement();
        for (Iterator<Element> i = root.elementIterator(); i.hasNext();) {
            Element elem = i.next();
            if (elem.getName().equals("msg")) {
                Attribute keyAttribute = elem.attribute("key");
                if (keyAttribute == null) {
                    throw new IllegalArgumentException(xml.getAbsolutePath()
                            + ": where is the key attribute of msg element? ");
                }
                String key = keyAttribute.getText();
                Element valueElem = elem.element("value");
                if (valueElem == null) {
                    throw new IllegalArgumentException(xml.getAbsolutePath()
                            + ": where is the value element of '" + key + "' ?");
                }
                p.put(key, valueElem.getTextTrim());
            }
        }
        return p;
    }

    @Override
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain)
            throws IOException, ServletException {
        String domain = OpiConstants.getCurrentDomain();
        if (domain == null) {
            domain = OpiConstants.findDomain(request);
            OpiConstants.setCurrentDomain(domain);
            request.setAttribute("domainIdentified", OpiConstants
                    .getCurrentDomainIdentifiedProperties());
        }
        if (logger.isDebugEnabled()) {
            logger.debug("current domain: " + domain);
        }
        try {
            chain.doFilter(request, response);
        } finally {
            OpiConstants.clearCurrentDomain();
        }
    }

    @Override
    public void destroy() {

    }

    public static void main(String[] args) throws Exception {
        DomainIdentifiedFilter f = new DomainIdentifiedFilter();
        f.loadConfigs(null, new File(args[0]));

    }
}
