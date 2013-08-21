package com.dodoyo.invite.httpservice.logic;

import com.dodoyo.invite.core.logic.LogUtils;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Properties;
import java.util.Set;

public class ServiceStatusLogic {

    private static Map<String, String> SERVICE_STATUS_MAP = new HashMap<String, String>();
    ;

    private static Properties pp = new Properties();

    private static ServiceStatusLogic statusLogic = new ServiceStatusLogic();

    private ServiceStatusLogic() {
        initServiceStatus();
    }

    public static ServiceStatusLogic getInstance() {
        return statusLogic;
    }

    public Map<String, String> getServiceStatus() {
        return SERVICE_STATUS_MAP;
    }

    public void initServiceStatus() {
        try {
            pp.load(new FileInputStream(com.dodoyo.invite.core.Constants.SERVICE_INTERFACE_CONF_PATH));
            Set<Entry<Object, Object>> kvs = pp.entrySet();
            for (Entry e : kvs) {
                SERVICE_STATUS_MAP.put((String) e.getKey(), (String) e.getValue());
                LogUtils.logInfo("set service status, key = " + e.getKey() + ", value = " + e.getValue());
            }
        }
        catch (FileNotFoundException e) {
            e.printStackTrace();
        }
        catch (IOException e) {
            e.printStackTrace();
        }
    }
}