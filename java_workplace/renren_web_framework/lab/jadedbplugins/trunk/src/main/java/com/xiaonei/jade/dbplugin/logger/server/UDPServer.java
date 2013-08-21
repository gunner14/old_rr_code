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
package com.xiaonei.jade.dbplugin.logger.server;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.util.logging.FileHandler;
import java.util.logging.Formatter;
import java.util.logging.Handler;
import java.util.logging.Level;
import java.util.logging.LogRecord;
import java.util.logging.Logger;

public class UDPServer {

    Logger log = Logger.getLogger(this.getClass().getName());

    private int port = 10005;

    private DatagramSocket ser;

    private DatagramPacket pac;

    private byte rb[];

    private String rev;

    public UDPServer(int port, String file) {
        this.port = port;
        setLog(file);
        init();
    }

    public void init() {

        try {
            ser = new DatagramSocket(port);
            rb = new byte[1024];
            pac = new DatagramPacket(rb, rb.length);
            rev = "";
            int i = 0;
            while (true) {
                ser.receive(pac);
                i = pac.getLength();
                // 接收数据
                if (i > 0) {
                    new Thread() {

                        @Override
                        public void run() {
                            // 指定接收到数据的长度,可使接收数据正常显示,开始时很容易忽略这一点
                            rev = new String(rb, 0, pac.getLength());
                            log.info(rev);
                        };
                    }.start();
                    i = 0;
                }
            }
        } catch (Exception e) {
            log.log(Level.WARNING, "", e);
        } finally {
            if (null != ser) {
                ser.close();
            }
        }
    }

    private void setLog(String file) {
        FileHandler handler;
        try {
            handler = new FileHandler(file, false);
        } catch (SecurityException e1) {
            e1.printStackTrace();
            return;
        } catch (IOException e1) {
            e1.printStackTrace();
            return;
        }
        try {
            handler.setEncoding("UTF-8");
        } catch (SecurityException e) {
            log.log(Level.WARNING, "", e);
        } catch (UnsupportedEncodingException e) {
            log.log(Level.WARNING, "", e);
        }
        handler.setLevel(Level.INFO);
        final Formatter newFormatter = new Formatter() {

            @Override
            public String format(final LogRecord record) {
                return record + "\n";
            }
        };
        handler.setFormatter(newFormatter);
        Handler[] hs = new Handler[log.getHandlers().length];
        for (int i = 0; i < log.getHandlers().length; i++) {
            hs[i] = log.getHandlers()[i];
        }
        for (Handler h : hs) {
            log.removeHandler(h);
        }
        log.addHandler(handler);
    }

    public static void main(String[] args) {
        if (args.length < 2) {
            System.out.println("[port] [file]");
            return;
        }

        new UDPServer(Integer.valueOf(args[0]), String.valueOf(args[1]));
    }
}
