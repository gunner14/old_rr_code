/**
 * 
 */
package com.xiaonei.click.bootstrap;

import java.io.File;
import java.io.FileOutputStream;
import java.io.PrintStream;
import java.net.InetSocketAddress;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.click.admin.HealthCheckThread;
import com.xiaonei.click.admin.ReloadAble;
import com.xiaonei.click.configure.Config;
import com.xiaonei.click.configure.Config.Key;
import com.xiaonei.click.server.HttpServer;
import com.xiaonei.click.server.NioClickServer;

/**
 * 
 * 极烂的代码，重构后补注释
 * 
 * @author lookis (lookisliu@gmail.com)
 * 
 */
public class WsepStarter implements ReloadAble {

    private static Log logger = LogFactory.getLog(WsepStarter.class);

    private HttpServer server = null;

    private static InetSocketAddress address = null;

    public static String SERVER_PATH;

    public static void main(String[] args) {
        if (args.length != 1) {
            System.out.println("Usage: " + WsepStarter.class.getName() + " <SERVER_PATH>");
            return;
        } else {
            SERVER_PATH = args[0];
            if (!checkRuntimeDependance(SERVER_PATH)) return;
            Config.init(SERVER_PATH + File.separator + "conf" + File.separator
                    + "wsep-config.properties");
        }

        WsepStarter starter = new WsepStarter();
        //Properties p = null;
        try {
            //    p = starter.loadConfig();
            String stdout, stderr;
            stdout = (String) Config.getInstance().get(Config.Key.STDOUT, starter);
            stderr = (String) Config.getInstance().get(Config.Key.STDERR, starter);
            if (stdout.startsWith(".")) {
                stdout = stdout.replaceFirst("\\.", SERVER_PATH);
            }
            if (stderr.startsWith(".")) {
                stderr = stderr.replaceFirst("\\.", SERVER_PATH);
            }
            File out = new File(stdout);
            File err = new File(stderr);
            try {
                if (!out.exists()) {
                    out.createNewFile();
                }
                if (!err.exists()) {
                    err.createNewFile();
                }
            } catch (Exception e) {
                logger.debug("Init stdout/stderr error,continue with out redirect std");
            }

            //setup server
            System.setOut(new PrintStream(new FileOutputStream(stdout, true)));
            System.setErr(new PrintStream(new FileOutputStream(stderr, true)));
        } catch (Exception e) {
            e.printStackTrace();
            return;
        }

        //start server

        int port = Integer.parseInt(Config.getInstance().get(Config.Key.PORT, starter).toString());
        address = new InetSocketAddress(port);
        starter.server = new NioClickServer();
        starter.server.bind(address);
        System.out.println("server started at " + address.getPort());
        //start health check thread
        HealthCheckThread.getInstance().init(starter);
        HealthCheckThread.getInstance().start();

    }

    private static boolean checkRuntimeDependance(String SERVER_PATH) {
        File configPath = new File(SERVER_PATH + File.separator + "conf" + File.separator
                + "wsep-config.properties");
        if (!configPath.exists()) {
            System.out.println("Can't fetch config.");
            System.out.println("Check file:" + configPath.getAbsolutePath());
            return false;
        }
        return true;
    }

    @Override
    public void reloadAll() {
        System.out.println("Restart server...");
        if (server != null) {
            System.out.println("Release resource");
            server.reloadAll();
        }

        server = new NioClickServer();
        server.bind(address);
        System.out.println("New server started at " + address.getPort());
    }

    @Override
    public void reload(Key k) {
        if (k.equals(Config.Key.PORT)) {
            address = new InetSocketAddress(address.getAddress(), Integer.parseInt(Config
                    .getInstance().get(Config.Key.PORT, null).toString()));
            reloadAll();
        }
    }
}
