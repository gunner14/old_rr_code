package xce.tripod;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.locks.ReentrantLock;



public class InvokeClient {

    private InvokeClient() {
        new Thread(task).start();
    }

    private static InvokeClient instance = null;

    static InvokeClient getInstance() {
        if (instance == null) {
            synchronized (InvokeClient.class) {
                if (instance == null) {
                    instance = new InvokeClient();
                }
            }
        }
        return instance;
    }

    /** 异步log任务 */
    private StatisticsLogTask task = new StatisticsLogTask();

    private ConcurrentHashMap<String, OperationInvokeStatistics> map = new ConcurrentHashMap<String, OperationInvokeStatistics>();

    private ReentrantLock lock = new ReentrantLock();

    private OperationInvokeStatistics getOpInvokeStatistics(String op, String namespaceId,
            String businessId) {
        String key = namespaceId + businessId + op;
        if (map.get(key) == null) {
            try {
                lock.lock();
                if (map.get(key) == null) {
                    map.put(key, new OperationInvokeStatistics(namespaceId, businessId, op, task));
                }
            } finally {
                lock.unlock();
            }
        }
        return map.get(key);
    }

    void log(String op, String namespaceId, String businessId, long count) {
        getOpInvokeStatistics(op, namespaceId, businessId).incCount(count);
    }

    void log(String op, String namespaceId, String businessId) {
        log(op, namespaceId, businessId, 1L);
    }
}
