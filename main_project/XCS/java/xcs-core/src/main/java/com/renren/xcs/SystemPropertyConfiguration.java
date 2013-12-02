package com.renren.xcs;

import com.renren.xcs.InvalidConfigurationException;
import com.renren.xcs.PropertiesFileConfiguration;

/**
 * 使用系统参数指定配置文件位置的Configuration实现。此类够着函数没有任何参数，可以直接创建实例。创建实例过程中会从
 * 系统参数中取出配置文件的位置。client配置文件位置使用xcs.client.config指定，server配置文件位置使用
 * xcs.server.config指定。
 * 
 * @author Xun Dai <xun.dai@renren-inc.com>
 * 
 */
public class SystemPropertyConfiguration extends PropertiesFileConfiguration {

    public final static String XCS_CONFIG_KEY = "xcs.config";

    private static String parseConfiurationPath() throws NoConfigurationException {
        String serverConfig = System.getProperty(XCS_CONFIG_KEY);
        String faq = "More >> http://wiki.d.xiaonei.com/display/xoa/FAQ";
        if (serverConfig == null) {
            throw new NoConfigurationException("No config file specified in system properties named '"
                    + XCS_CONFIG_KEY + "'. " + faq);
        }
        return serverConfig;
    }

    /**
     * 夠著函數
     * 
     * @param type 文件的类型。Type.server或者Type.client
     * @throws InvalidConfigurationException 系統参数中没有指定或者指定错误文件，将会跑出次异常
     */
    public SystemPropertyConfiguration() throws InvalidConfigurationException,NoConfigurationException {
        super(parseConfiurationPath());
    }

}
