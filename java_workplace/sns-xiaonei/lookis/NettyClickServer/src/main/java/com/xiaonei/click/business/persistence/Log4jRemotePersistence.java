/**
 * 
 */
package com.xiaonei.click.business.persistence;

import java.io.Serializable;

import org.apache.log4j.Appender;
import org.apache.log4j.net.SocketAppender;

/**
 * @author lookis (lookisliu@gmail.com)
 * 
 */
public class Log4jRemotePersistence implements Persistence {

    private static volatile Log4jRemotePersistence _instance = null;

    private Appender appender = null;

    private org.apache.log4j.Logger logger = null;

    private Log4jRemotePersistence() {
        appender = new SocketAppender("10.3.19.59", 8080);
        logger = org.apache.log4j.LogManager.getLogger("click");
        logger.removeAllAppenders();
        logger.addAppender(appender);
    }

    public static Log4jRemotePersistence getInstance() {
        if (_instance == null) {
            synchronized (Log4jRemotePersistence.class) {
                if (_instance == null) {
                    _instance = new Log4jRemotePersistence();
                }
            }
        }
        return _instance;
    }

    @Override
    public Object get(Object key) {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public void set(Serializable o) {
        logger.warn(o.toString());
    }

}
