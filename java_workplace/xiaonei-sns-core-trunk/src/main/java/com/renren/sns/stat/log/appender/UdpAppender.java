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
package com.renren.sns.stat.log.appender;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

import org.apache.log4j.AppenderSkeleton;
import org.apache.log4j.Layout;
import org.apache.log4j.helpers.LogLog;
import org.apache.log4j.spi.LoggingEvent;

/**
 * @author Wang Tai (cn.wang.tai@gmail.com)
 * @date Nov 8, 2011 4:02:58 PM
 * 
 */
public class UdpAppender extends AppenderSkeleton {

    static public final int DEFAULT_PORT = 4560;// 默认端口号

    /** 通过域名得到IP **/
    static InetAddress getAddressByName(final String host) {
        try {
            return InetAddress.getByName(host);
        } catch (final Exception e) {
            return null;
        }
    }

    private String remote_host_;

    private int port_ = DEFAULT_PORT;

    private InetAddress address_;// IP

    private DatagramSocket socket_ = null;

    public UdpAppender() {
    }

    public UdpAppender(final Layout layout, final InetAddress address, final int port) {
        this.layout = layout;
        this.address_ = address;
        this.remote_host_ = address.getHostName();
        this.port_ = port;

        try {
            this.socket_ = new DatagramSocket();
        } catch (final Exception e) {
            e.printStackTrace();
        }
    }

    public UdpAppender(final Layout layout, final String host, final int port) {
        this.layout = layout;
        this.port_ = port;
        this.remote_host_ = host;
        this.address_ = getAddressByName(host);

        try {
            this.socket_ = new DatagramSocket();
        } catch (final Exception e) {
            e.printStackTrace();
        }
    }

    @Override
    public void append(final LoggingEvent event) {
        if (event == null) {
            return;
        }

        if (this.address_ == null) {
            return;
        }

        if (this.socket_ == null) {
            return;
        }

        if (this.layout == null) {
            LogLog.debug("The layout is not loaded... we set it.");
            final String pattern = "%d{yyyy-MM-dd.HH:mm:ss}%m%n";
            this.setLayout(new org.apache.log4j.PatternLayout(pattern));
        }
        final String packet = this.layout.format(event);
        if ((packet != null) && !packet.isEmpty()) {
            try {
                final byte[] bytes = packet.getBytes();
                int length = bytes.length;

                // 超过1024字节很可能发不过去
                if (length > 1024) {
                    length = 1024;
                }
                // 构造一个数据包对象
                final DatagramPacket data = new DatagramPacket(bytes, length, this.address_,
                        this.port_);
                this.socket_.send(data);
            } catch (final Exception e) {}
        }
    }

    @Override
    synchronized public void close() {
        if (this.closed) {
            return;
        }
        if (this.socket_ != null) {
            this.socket_.close();
            this.socket_ = null;
        }
        this.closed = true;
    }

    public int getPort() {
        return this.port_;
    }

    public String getRemoteHost() {
        return this.remote_host_;
    }

    @Override
    public boolean requiresLayout() {
        return true;
    }

    public void setPort(final int port) {
        this.port_ = port;
    }

    public void setRemoteHost(final String host) {
        final String host1 = host == null ? "" : host.trim();
        this.address_ = getAddressByName(host1);
        this.remote_host_ = host1;
    }

}
