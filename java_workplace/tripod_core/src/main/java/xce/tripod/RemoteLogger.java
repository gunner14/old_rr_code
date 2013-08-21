package xce.tripod;

import java.text.SimpleDateFormat;
import java.util.Date;

import xce.util.tools.IPAddress;

import com.xiaonei.xce.log.XceLoggerAdapter;

/**
 * RemoteLogger <br/>
 * 
 * @author michael
 * @email liyong@renren-inc.com
 * 
 */
public class RemoteLogger {

    /** */
    private static String[] categories = new String[] { "xce", "tripod",
	    "tripod_use" };

    /** 两次log操作的时间间隔,时间单位:ms */
    private static final long invokeInterval = 60 * 1000;

    /** client local ipv4 address */
    private String localIpv4Address;

    private long lastInvokeTime;

    RemoteLogger() {
	localIpv4Address = IPAddress.getLocalAddress();
	if ("" == localIpv4Address) {
	    localIpv4Address = "unknown_ipv4_address";
	}
	lastInvokeTime = 0L;
    }

    static RemoteLogger instance = null;

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
	return formatter.format(new Date()) + "\t" + localIpv4Address + "\t"
		+ message;
    }

    /**
     * log方法
     * 
     * @param message
     */
    void log(String message) {
	long now = System.currentTimeMillis();
	if (now - lastInvokeTime >= invokeInterval) {
	    lastInvokeTime = now;
	    try {
		XceLoggerAdapter.getLogger()
			.addLog(categories, format(message));
	    } catch (Exception e) {
		e.printStackTrace();
	    }

	}
    }
}
