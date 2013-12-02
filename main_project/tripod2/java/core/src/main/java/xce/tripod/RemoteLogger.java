package xce.tripod;

import java.text.SimpleDateFormat;
import java.util.Date;

import xce.util.tools.IPAddress;

import com.xiaonei.xce.log.XceLoggerAdapter;

public class RemoteLogger {

    /** client local ipv4 address */
    private String localIpv4Address;

    private RemoteLogger() {
        localIpv4Address = IPAddress.getLocalAddress();
        if ("" == localIpv4Address) {
            localIpv4Address = "unknown_ipv4_address";
        }
    }

    private static RemoteLogger instance = null;

    static RemoteLogger getInstance() {
        if (instance == null) {
            synchronized (RemoteLogger.class) {
                if (instance == null) {
                    instance = new RemoteLogger();
                }
            }
        }
        return instance;
    }

    private String format(String message) {
        SimpleDateFormat formatter = new SimpleDateFormat("HH:mm:ss");
        return formatter.format(new Date()) + " " + localIpv4Address + " " + message;
    }

    void log(String message, String[] categories) {
        try {
            XceLoggerAdapter.getLogger().addLog(categories, format(message));
        } catch (Exception e) {
            System.err.println(e.toString());
        }
    }

}
