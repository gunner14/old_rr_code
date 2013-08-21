/**
 * 
 */
package com.renren.sns.guide.service;

import java.io.IOException;
import java.util.HashSet;
import java.util.Properties;
import java.util.Set;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

public class HomeNuomiUserConfig {

    private final static String CONFIG_FILE = "nuomi_active_user_ids.properties";

    private static volatile Set<Integer> NUOMI_USER_ID = new HashSet<Integer>();

    private ScheduledExecutorService executor = Executors.newSingleThreadScheduledExecutor();

    private static volatile HomeNuomiUserConfig _instance = null;

    private HomeNuomiUserConfig() {
        initReloader();
    }

    public static HomeNuomiUserConfig getInstance() {
        if (_instance == null) {
            synchronized (HomeNuomiUserConfig.class) {
                if (_instance == null) {
                    _instance = new HomeNuomiUserConfig();
                }
            }
        }
        return _instance;
    }

    public boolean isNuomi(int userId) {
        return NUOMI_USER_ID.contains(userId);
    }

    public void initReloader() {
        executor.scheduleWithFixedDelay(new Runnable() {

            @Override
            public void run() {
                reloadConfig();
            }
        }, 0, 10, TimeUnit.MINUTES);
    }

    public static void reloadConfig() {
        Set<Integer> idSet = new HashSet<Integer>();
        try {
            final Properties p = new Properties();
            p.load(HomeNuomiUserConfig.class.getClassLoader().getResourceAsStream(CONFIG_FILE));

            for (Object k : p.keySet()) {
                try {
                    Integer key = Integer.valueOf((String) k);
                    idSet.add(key);
                } catch (NumberFormatException e) {
                    e.printStackTrace();
                }
            }
            NUOMI_USER_ID = idSet;
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

}
