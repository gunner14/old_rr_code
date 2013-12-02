package xce.tripod;

import java.util.concurrent.BlockingDeque;
import java.util.concurrent.LinkedBlockingDeque;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class StatisticsLogTask implements Runnable {

    private static String[] categories = new String[] { "xce", "tripod", "tripod_statistics" };

    private BlockingDeque<String> messages = new LinkedBlockingDeque<String>();

    private static Log logger = LogFactory.getLog(StatisticsLogTask.class);

    public void addMessage(String message) {
        try {
            messages.put(message);
        } catch (InterruptedException e) {
            logger.error(e);
        }
        if (messages.size() > 1000) {
            logger.error(new TripodException("statistics log message queue full!"));
            messages.clear();
        }
    }

    public void run() {
        while (true) {
            String message = null;
            try {
                message = messages.take();
            } catch (InterruptedException e) {
                logger.error(e);
                continue;
            }
            RemoteLogger.getInstance().log(message, categories);
        }
    }

}
