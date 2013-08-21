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

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class UdpServer {

    private static final int DEFAULT_PORT = 10005;

    public static void main(final String[] args) {
        if (args.length < 2) {
            System.out.println("[port] ");
            return;
        }

        new UdpServer(Integer.valueOf(args[0]));
    }

    Log logger = LogFactory.getLog(this.getClass());

    private int port = DEFAULT_PORT;

    private DatagramSocket datagramSocket;

    private DatagramPacket datagramPacket;

    private byte receiveBuffer[];

    private String receiver;

    public UdpServer(final int port) {
        this.port = port;
        this.init();
    }

    public void init() {

        try {
            this.datagramSocket = new DatagramSocket(this.port);
            this.receiveBuffer = new byte[1024];
            this.datagramPacket = new DatagramPacket(this.receiveBuffer, this.receiveBuffer.length);
            this.receiver = "";
            int i = 0;
            while (true) {
                this.datagramSocket.receive(this.datagramPacket);
                i = this.datagramPacket.getLength();
                // 接收数据
                if (i > 0) {
                    new Thread() {

                        @Override
                        public void run() {
                            // 指定接收到数据的长度,可使接收数据正常显示,开始时很容易忽略这一点
                            UdpServer.this.receiver = new String(UdpServer.this.receiveBuffer, 0,
                                    UdpServer.this.datagramPacket.getLength());
                            UdpServer.this.logger.info(UdpServer.this.receiver);
                        };
                    }.start();
                    i = 0;
                }
            }
        } catch (final Exception e) {
            this.logger.warn("", e);
        } finally {
            if (null != this.datagramSocket) {
                this.datagramSocket.close();
            }
        }
    }

}
