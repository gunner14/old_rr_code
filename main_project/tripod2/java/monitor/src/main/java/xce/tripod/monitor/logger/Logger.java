package xce.tripod.monitor.logger;

import java.text.SimpleDateFormat;
import java.util.Date;

public class Logger {
	public static void log(String content) {
		System.out.println("[" + new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date()) + "] " + content);
	}
}
