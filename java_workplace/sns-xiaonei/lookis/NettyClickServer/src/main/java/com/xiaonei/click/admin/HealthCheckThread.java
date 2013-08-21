package com.xiaonei.click.admin;

import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;

import com.xiaonei.click.configure.Config;
import com.xiaonei.click.configure.Config.Key;

/**
 * 纯针对Http的健康检查线程
 * 
 * @author lookis (lookisliu@gmail.com)
 * 
 */
public class HealthCheckThread extends Thread implements ReloadAble {

    private static HealthCheckThread _instance = new HealthCheckThread();

    private long checkInterval = 10 * 60 * 1000; //默认10分钟检查一次

    private ReloadAble serverInstance = null;

    private int port;

    private HealthCheckThread() {
        super("HealthCheckThread");
    }

    public static HealthCheckThread getInstance() {
        return _instance;
    }

    //这里沿用ReloadAble,传过来的肯定是bootstrap里的类
    public void init(ReloadAble reloadAble) {
        serverInstance = reloadAble;
        this.checkInterval = Long.parseLong(Config.getInstance().get(Config.Key.CHECK_INTERVAL,
                this).toString());
        this.port = Integer.parseInt(Config.getInstance().get(Config.Key.PORT, this).toString());
    }

    @Override
    public void run() {
        System.out.println("Health Checker monitor the server on port:" + port);
        while (true) {
            try {
                Thread.sleep(checkInterval);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            URL u = null;
            try {
                u = new URL("http", "127.0.0.1", port, "index.html");
            } catch (MalformedURLException e) {
                System.err.println("Health Check Thread Create Fail!!");
                e.printStackTrace();
            }

            if (u != null) {
                try {
                    u.openStream();
                } catch (IOException e) {
                    System.err.println("Server didn't response,restart");
                    serverInstance.reloadAll();
                }
            }
        }
    }

    //涉及变量有CHECK_INTERVAL和PORT，重新加载配置的时候要更新
    @Override
    public void reload(Key k) {

        if (k.equals(Key.CHECK_INTERVAL)) {
            this.checkInterval = Long.parseLong(Config.getInstance().get(Config.Key.CHECK_INTERVAL,
                    this).toString());
        } else if (k.equals(Key.PORT)) {
            this.port = Integer
                    .parseInt(Config.getInstance().get(Config.Key.PORT, this).toString());
        }

    }

    @Override
    public void reloadAll() {
        // TODO Auto-generated method stub

    }
}
