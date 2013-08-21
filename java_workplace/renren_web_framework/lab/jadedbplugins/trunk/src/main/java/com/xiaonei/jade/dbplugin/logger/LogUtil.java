/*
 * Copyright 2009-2010 the original author or authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License i distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.xiaonei.jade.dbplugin.logger;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.log4j.AsyncAppender;
import org.apache.log4j.Level;
import org.apache.log4j.Logger;
import org.apache.log4j.net.SocketAppender;

import com.xiaonei.jade.dbplugin.logger.appender.UDPAppender;

/**
 * 取得Logger工具类
 * 
 * @author michael(liyong@opi-corp.com)
 * 
 */
public class LogUtil {

    protected static Log log = LogFactory.getLog(LogUtil.class);

    private static final boolean isTcp = true;

    /** 远程logger,lazy load */
    private static Logger jadeDBMonitorLogger = null;

    private static String remoteIp = "10.3.16.234";

    private static int remotePort = 12000;

    private static Level level = Level.INFO;

    private static int deplay = 5000;

    public static Logger getJadeDBMonitorLogger() {
        try {
            if (jadeDBMonitorLogger == null) {
                synchronized (LogUtil.class) {
                    if (jadeDBMonitorLogger == null) {

                        AsyncAppender asyncAppender = new AsyncAppender();

                        if (isTcp) {

                            SocketAppender socketAppender = new SocketAppender(remoteIp, remotePort);
                            socketAppender.setReconnectionDelay(deplay);
                            socketAppender.setLocationInfo(true);
                            asyncAppender.addAppender(socketAppender);
                        } else {

                            UDPAppender udpAppender = new UDPAppender();
                            udpAppender.setRemoteHost(remoteIp);
                            udpAppender.setPort(remotePort);
                            asyncAppender.addAppender(udpAppender);
                        }

                        jadeDBMonitorLogger = Logger.getLogger("JadeDBMonitorLogger");

                        jadeDBMonitorLogger.removeAllAppenders();
                        jadeDBMonitorLogger.setAdditivity(false);
                        jadeDBMonitorLogger.setLevel(level);
                        jadeDBMonitorLogger.addAppender(asyncAppender);
                    }
                }
            }
        } catch (Exception e) {
            jadeDBMonitorLogger = null;
            log.error("", e);
        }

        return jadeDBMonitorLogger;
    }

    public static void main(String[] args) {
        // try {
        // LogUtil.getJadeDBMonitorLogger().info("33ello World");
        // } catch (Exception e) {
        // e.printStackTrace();
        // }
    }

}
