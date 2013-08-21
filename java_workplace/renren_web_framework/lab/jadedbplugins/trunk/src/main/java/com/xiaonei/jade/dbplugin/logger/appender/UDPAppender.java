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
/**
 * 
 */
package com.xiaonei.jade.dbplugin.logger.appender;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;

import org.apache.log4j.AppenderSkeleton;
import org.apache.log4j.LogManager;
import org.apache.log4j.Logger;
import org.apache.log4j.helpers.LogLog;
import org.apache.log4j.net.SocketNode;
import org.apache.log4j.spi.LoggingEvent;

/**
 * UDPAppender <br>
 * 
 * @author tai.wang@opi-corp.com May 25, 2010 - 6:15:29 PM
 */
public class UDPAppender extends AppenderSkeleton {

    Logger log = LogManager.getLogger(this.getClass());

    static private int bufferSize = 8 * 1024;

    private byte data[];

    private String remoteHost = "localhost";

    private int port = 6000;

    private InetAddress address = null;

    private DatagramSocket dataSocket = null;

    private DatagramPacket dataPacket = null;

    public UDPAppender() {
        init();
    }

    private void init() {
        try {
            dataSocket = new DatagramSocket(this.port + 1);
            address = InetAddress.getByName(remoteHost);
        } catch (Exception e) {
            log.error("", e);
        }

        data = new byte[bufferSize];

        if (this.layout == null) {
            LogLog.debug("The layout is not loaded... we set it.");
            String pattern = "%d{yyyy-MM-dd.HH:mm:ss}%m%n";
            this.setLayout(new org.apache.log4j.PatternLayout(pattern));
        }
    }

    @Override
    protected void append(LoggingEvent event) {
        try {
            String msg = this.getLayout().format(event);
            data = msg.getBytes();
            dataPacket = new DatagramPacket(data, data.length, address, port);
            dataSocket.send(dataPacket);
        } catch (SocketException se) {
            log.error("", se);
        } catch (IOException ie) {
            log.error("", ie);
        }
    }

    /**
     * Derived appenders should override this method if option structure
     * requires it.
     */
    @Override
    public void activateOptions() {
        init();
    }

    @Override
    public void close() {
        if (closed) {
            return;
        }

        if (!dataSocket.isClosed()) {
            dataSocket.close();
        }
        closed = true;
    }

    @Override
    public boolean requiresLayout() {
        return true;
    }

    /**
     * The <b>RemoteHost</b> option takes a string value which should be
     * the host name of the server where a {@link SocketNode} is running.
     * */
    public void setRemoteHost(String host) {
        String val = host.trim();
        remoteHost = val;
    }

    /**
     * Returns value of the <b>RemoteHost</b> option.
     */
    public String getRemoteHost() {
        return remoteHost;
    }

    /**
     * The <b>Port</b> option takes a positive integer representing the
     * port where the server is waiting for connections.
     */
    public void setPort(int port) {
        this.port = port;
    }

    /**
     * Returns value of the <b>Port</b> option.
     */
    public int getPort() {
        return port;
    }

}
