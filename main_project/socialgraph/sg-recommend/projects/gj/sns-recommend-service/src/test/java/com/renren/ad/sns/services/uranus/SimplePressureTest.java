package com.renren.ad.sns.services.uranus;

import java.util.concurrent.CountDownLatch;

import org.apache.thrift.TException;

public class SimplePressureTest {

    private static final long uid = 245783726L;

    private static final long gid = 1206041452497975643L;

    static class MockClient extends Thread {

        private final UranusServiceProxy proxy = new UranusServiceProxy("10.7.18.50", 9636, 10000);

        //        private final UranusServiceProxy proxy = new UranusServiceProxy("10.3.24.96", 9635, 10000);

        private int numRequest = 100;

        private float avgRequestTime = 0;

        private CountDownLatch latch;

        public MockClient(CountDownLatch latch, int numRequest) {
            this.latch = latch;
            this.numRequest = numRequest;
        }

        @Override
        public void run() {
            long begin = System.currentTimeMillis();
            for (int i = 0; i < numRequest; i++) {
                try {
                    proxy.recommend(uid, gid, 20);
                } catch (TException e) {
                    e.printStackTrace();
                }
            }
            avgRequestTime = (System.currentTimeMillis() - begin) / (1.0f * numRequest);
            System.out.println(Thread.currentThread().getName() + ":" + avgRequestTime);
            latch.countDown();

            proxy.cleanUp();
        }

        public int getNumRequest() {
            return numRequest;
        }

        public float getAvgRequestTime() {
            return avgRequestTime;
        }
    }

    public static float runTest(int numThreads, int requestsPerThread) {
        CountDownLatch latch = new CountDownLatch(numThreads);
        MockClient[] threads = new MockClient[numThreads];
        for (int i = 0; i < threads.length; i++) {
            threads[i] = new MockClient(latch, requestsPerThread);
            threads[i].start();
        }

        // wait all threads to finish
        try {
            latch.await();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        float totalRequestTime = 0;
        for (int i = 0; i < threads.length; i++) {
            totalRequestTime += threads[i].getAvgRequestTime();
        }
        return (1.0f * totalRequestTime) / numThreads;
    }

    public static void main(String[] args) {
        /*
         * Test result:
         * 1 thread(1000 requests per thread): 33.731003/28.713333
         * 10 thread(100 requests per thread): 84.40867/67.801
         * 100 thread(10 requests per thread): 735.93506/624.8239
         */
        Object testSuite = new Object[] { new Object[] { "1 thread(1000 requests): ", 1, 100 },
        /*new Object[] { "10 thread(100 requests): ", 10, 100 },*/
        /*new Object[] { "100 thread(10 requests): ", 80, 10 }*/};

        final int times = 1;
        for (Object ts : (Object[]) testSuite) {
            Object[] test = (Object[]) ts;
            float totalTime = 0;
            for (int i = 0; i < times; i++) {
                totalTime += SimplePressureTest.runTest((Integer) test[1], (Integer) test[2]);
            }
            System.out.println((String) test[0] + totalTime / times);
        }

        try {
            Thread.sleep(100);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
