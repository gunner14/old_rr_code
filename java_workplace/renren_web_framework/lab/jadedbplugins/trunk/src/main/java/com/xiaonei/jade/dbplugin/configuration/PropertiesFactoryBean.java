package com.xiaonei.jade.dbplugin.configuration;

import java.io.IOException;
import java.io.InputStream;
import java.util.Properties;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.BeansException;
import org.springframework.beans.factory.FactoryBean;
import org.springframework.beans.factory.InitializingBean;
import org.springframework.context.ApplicationContext;
import org.springframework.context.ApplicationContextAware;
import org.springframework.core.io.Resource;
import org.springframework.web.context.WebApplicationContext;

/**
 * PropertiesFactoryBean <br>
 * 配置信息
 * 
 * @author tai.wang@opi-corp.com Jul 19, 2010 - 6:33:25 PM
 */
public class PropertiesFactoryBean implements FactoryBean, InitializingBean,
        ApplicationContextAware {

    public static final String jadeMonitorConfigLocation = "jadeMonitorConfigLocation";

    protected Log logger = LogFactory.getLog(this.getClass());

    private Properties properties = new Properties();

    private ApplicationContext applicationContext;

    @Override
    public void setApplicationContext(ApplicationContext applicationContext) throws BeansException {
        this.applicationContext = applicationContext;
    }

    @Override
    public void afterPropertiesSet() throws Exception {
        setProperties();
    }

    private void setProperties() throws IOException {
        Resource[] resources;
        String jadeMonitorConfigLocation;
        if (applicationContext instanceof WebApplicationContext) {
            WebApplicationContext webctx = (WebApplicationContext) applicationContext;
            jadeMonitorConfigLocation = webctx.getServletContext().getInitParameter(
                    "jadeMonitorConfigLocation");
        } else {
            jadeMonitorConfigLocation = System.getProperty("jadeMonitorConfigLocation");
        }
        //
        if (jadeMonitorConfigLocation == null) {
            logger.info("disable jade monitor plugin: not configuration");
            return;
        }

        jadeMonitorConfigLocation = jadeMonitorConfigLocation.trim();
        resources = applicationContext.getResources(jadeMonitorConfigLocation);

        if (logger.isInfoEnabled()) {
            logger.info("jadedbplugin says: find \'" + resources.length + "\' profiles");
        }
        for (Resource res : resources) {
            if (logger.isInfoEnabled()) {
                logger.info("jadedbplugin's profile: " + res.getFilename());
            }
            InputStream in = res.getInputStream();
            try {
                properties.load(in);
            } catch (IOException e) {

                logger.warn("jadedbplugin says: Fail to read \"" + res.getFilename() + "\"");
            } catch (Exception e) {
                logger.warn("jadedbplugin says: connot find the file, that named \""
                        + res.getFilename() + "\"");
            }
        }
    }

    @Override
    public Object getObject() throws Exception {
        return properties;
    }

    @Override
    public Class<Properties> getObjectType() {
        return Properties.class;
    }

    /*
     * for reload
     */
    @Override
    public boolean isSingleton() {
        return false;
    }

}
