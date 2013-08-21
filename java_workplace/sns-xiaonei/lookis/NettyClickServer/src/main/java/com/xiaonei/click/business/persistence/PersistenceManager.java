package com.xiaonei.click.business.persistence;

/**
 * 持久化管理类
 * 
 * @author lookis (lookisliu@gmail.com)
 * 
 */
public class PersistenceManager {

    private static volatile Persistence _instance = null;

    private PersistenceManager() {

    }

    public static Persistence getInstance() {
        if (_instance == null) {
            synchronized (PersistenceManager.class) {
                if (_instance == null) {
                    try {
                        _instance = Log4jRemotePersistence.getInstance();
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            }
        }
        return _instance;
    }

    protected static void setPersistence(Persistence per) {
        _instance = per;
    }

}
