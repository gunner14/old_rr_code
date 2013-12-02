package com.renren.xcs;

import java.util.Map;
import java.util.Properties;
import java.util.Map.Entry;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class XcsConfigurations {

    private static Log logger = LogFactory.getLog(XcsConfigurations.class.getName());

    private static Configuration configuration = null;

    public final static String HOST_BINDING_PREFIX = "xcs.register";

    public final static String REGISTER_SIKP = "xcs.register.skip";

    public final static String OLD_HOST_BINDING_PREFIX = "xoa2.hosts";

    /*private final static int SERVER_CONFIGURATION_FLAG = 0;

    private final static int CLIENT_CONFIGURATION_FLAG = 1;

    public static Configuration getServerConfiguration() {
        return getConfiguration(SERVER_CONFIGURATION_FLAG);
    }

    public static Configuration getClientConfiguration() {
        return getConfiguration(CLIENT_CONFIGURATION_FLAG);
    }*/

    public static Configuration getConfiguration() {
        if (configuration == null) {
            synchronized (Configuration.class) {
                if (configuration == null) {
                    try {
                        try {
                            configuration = new SystemPropertyConfiguration();
                        } catch (NoConfigurationException e) {
                            if (isHostsBinded()) {
                                Properties properties = new Properties();
                                properties.put("cluster", "10.11.21.185:2181");
                                properties.put("root", "test");
                                properties.put("server.username", "test");
                                properties.put("server.password", "test");
                                properties.put("client.username", "test");
                                properties.put("client.password", "test");
                                properties.put("server.alarm.phones", "18888888888,13888888888");
                                properties.put("server.alarm.emails",
                                        "server.test@renren-inc.com,server.test2@renren-inc.com");
                                properties.put("client.alarm.phones", "18666666666,13666666666");
                                properties.put("client.alarm.emails",
                                        "server.test@renren-inc.com,server.test2@renren-inc.com");
                                configuration = new PropertiesFileConfiguration(properties);
                                logger.warn("'-Dxoa2.hosts.*=*' was configed. Use default Configuration.");
                            } else {
                                throw e;
                            }
                        }
                    } catch (XcsException e) {
                        throw new IllegalStateException("Invalid server configuraion.", e);
                    }
                }
            }
        }
        return configuration;
    }

    public static boolean isHostsBinded() {
        Map<?, ?> props = System.getProperties();
        for (Entry<?, ?> entry : props.entrySet()) {
            String key = (String) entry.getKey();
            if ((!key.equals(REGISTER_SIKP) && key.startsWith(HOST_BINDING_PREFIX)) || key.startsWith(OLD_HOST_BINDING_PREFIX)) {
                return true;
            }
        }
        return false;
    }
}
