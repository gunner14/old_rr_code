package xce.tripod;


public class OperationInvokeStatistics {

    /** 每次调用量积攒的时间间隔，每隔logInterval打一次log，时间单位:ms */
    private static final long TIMED_LOG_INTERVAL = 10 * 60 * 1000;

    /** 调用量积攒的最大值，达到了就打一次log */
    private static final long MAX_COUNT = 1000000;

    private long lastInvokeTime;

    private long count;

    private String namespaceId;

    private String businessId;

    private String operation;

    private StatisticsLogTask task;

    OperationInvokeStatistics(String namespaceId, String businessId, String op,
            StatisticsLogTask task) {
        count = 0;
        lastInvokeTime = 0L;
        this.namespaceId = namespaceId;
        this.businessId = businessId;
        this.operation = op;
        this.task = task;
    }

    synchronized void incCount(long n) {
        count += n;
        long now = System.currentTimeMillis();
        if ((count >= MAX_COUNT) || (now - lastInvokeTime >= TIMED_LOG_INTERVAL)) {
            lastInvokeTime = now;
            task.addMessage(dump(count));
            count = 0;
        }
    }

    private String dump(long count) {
        return Constants.VERSION + " " + namespaceId + " " + businessId + " " + operation + " "
                + count;
    }

}
