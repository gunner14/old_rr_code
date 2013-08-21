/**
 * 
 */
package com.xiaonei.click;

import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;

import javax.servlet.ServletContextEvent;
import javax.servlet.ServletContextListener;

import net.spy.memcached.MemcachedClient;

/**
 * @author lookis (lookisliu@gmail.com)
 * 
 */
public class Initiate implements ServletContextListener {

    MemcachedClient mc = null;

    private static int CORE_POOL_SIZE = 10;

    ScheduledExecutorService es = Executors.newScheduledThreadPool(CORE_POOL_SIZE);

    @Override
    public void contextDestroyed(ServletContextEvent arg0) {

    }

    @Override
    public void contextInitialized(ServletContextEvent arg0) {
    }
}
