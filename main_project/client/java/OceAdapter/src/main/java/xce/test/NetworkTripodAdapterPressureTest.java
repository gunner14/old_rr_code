package xce.test;

import java.io.BufferedReader;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.concurrent.TimeUnit;

import mop.hi.oce.adapter.impl.NetworkTripodAdapter;

class TestTask implements Runnable {

    public TestTask(int i) {
        threadId = i;
        startIndex = NetworkTripodAdapterPressureTest.THREAD_RUN * threadId;
        endIndex = startIndex + NetworkTripodAdapterPressureTest.THREAD_RUN - 1;

        System.out.println("**************thread" + threadId + "constructed startIndex  "
                + startIndex + "  endIndex  " + endIndex + "****************");
    }

    public static final int FREQ_MAX = 10000;

    private int threadId;

    private int startIndex;

    private int endIndex;

    @Override
    public void run() {
        System.out.println("*********thread  " + threadId + "  run *****");

        int index;
        int rate;
        long now;

        index = startIndex;
        rate = 0;
        now = System.currentTimeMillis();
        while (index < endIndex) {
            int uid = NetworkTripodAdapterPressureTest.arr.get(index);
            NetworkTripodAdapter.getInstance().reloadCollegeInfoList(uid);
            NetworkTripodAdapter.getInstance().reloadElementarySchoolInfo(uid);
            NetworkTripodAdapter.getInstance().reloadHighSchoolInfoList(uid);
            NetworkTripodAdapter.getInstance().reloadJuniorHighSchoolInfo(uid);
            NetworkTripodAdapter.getInstance().reloadNetworkList(uid);
            NetworkTripodAdapter.getInstance().reloadRegionInfo(uid);
            NetworkTripodAdapter.getInstance().reloadUniversityInfoList(uid);
            NetworkTripodAdapter.getInstance().reloadWorkplaceInfoList(uid);
            ++index;
            ++rate;
            long interval = (System.currentTimeMillis() - now);
            if (rate >= FREQ_MAX || interval > 1000) {
                System.out.println("Thread  " + threadId + " interval  " + interval);
                if (interval != 0) {
                    System.out.println("Thread " + threadId + " Rate" + rate * 1000 / interval);
                }
                if (interval < 1000) {
                    try {
                        TimeUnit.MILLISECONDS.sleep(1000 - interval);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
                rate = 0;
                now = System.currentTimeMillis();
            }
        }

    }
}

public class NetworkTripodAdapterPressureTest {

    public static int testSize;

    public static ArrayList<Integer> arr = new ArrayList<Integer>();

    public static final int THREADS = 10;

    public static int THREAD_RUN;

    public static void main(String[] args) {

        try {
            BufferedReader infile = new BufferedReader(new FileReader("./run_data.txt"));
            String ss = "";
            while ((ss = infile.readLine()) != null) {
                arr.add(Integer.parseInt(ss));
            }
            infile.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
        testSize = arr.size();

        THREAD_RUN = testSize / THREADS;

        System.out.println("*******************Program Begin********************************");
        System.out.println("**************data size : " + testSize + "***********************");

        Thread[] t = new Thread[THREADS];
        for (int i = 0; i < THREADS; ++i) {
            t[i] = new Thread(new TestTask(i));
        }
        for (int j = 0; j < THREADS; ++j) {
            t[j].start();
        }

        for (int k = 0; k < THREADS; ++k) {
            try {
                t[k].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        System.exit(0);
    }

}
