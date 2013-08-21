package example;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

interface TimeCost {

    long cost();
}

class NginxTimeCost implements TimeCost {

    @Override
    public long cost() {
        int t = new Random(System.currentTimeMillis()).nextInt(10);
        return (long) t;
    }
}

class TomcatTimeCost {

    public int costBySec() {
        // do...sth
        return new Random(System.currentTimeMillis()).nextInt(100); //10 secs
    }
}

class TomcatAdapterTC implements TimeCost {

    private TomcatTimeCost tomcatTimeCost;

    public TomcatAdapterTC(TomcatTimeCost ttc) {
        this.tomcatTimeCost = ttc;
    }

    @Override
    public long cost() {
        return tomcatTimeCost.costBySec() * 1000;
    }
}

public class AdapterOne {

    /**
     * @param args
     */
    public static void main(String[] args) {
        List<TimeCost> list = new ArrayList<TimeCost>();
        list.add(new NginxTimeCost());
        list.add(new NginxTimeCost());
        list.add(new NginxTimeCost());
        list.add(new TomcatAdapterTC(new TomcatTimeCost()));

        long total = 0;
        for (TimeCost tc : list) {
            total += tc.cost();
        }
        System.out.println("Total TimeCost: " + total + " ms");
    }

}
