package com.renren.sns.wiki.stat.processor;

import java.util.HashSet;

import com.renren.sns.wiki.stat.Frequency;

/**
 * 统计UV，海量ID的情况下可以使用BitSet，暂时预估的UV在百万级，进过测试HashSet就足够用，
 * 300万uv的情况下所占的内存大小为100M
 * 
 * @author yi.li@renren-inc.com since 2012-4-23
 * 
 */
public class UVProcessor extends AbstractProcessor {

    private HashSet<Integer> uvSet;

    public UVProcessor() {
        uvSet = new HashSet<Integer>();
    }

    @Override
    public boolean support(String logType) {

        return true;
    }

    @Override
    public void parseLog(String[] params) {
        String uidStr = params[1];
        try {
            Integer uid = Integer.parseInt(uidStr);
            uvSet.add(uid);
        } catch (NumberFormatException e) {
            logger.error("UV log not number: " + uidStr);
        }
    }

    @Override
    public void finish() {
        if (uvSet != null) {
            int uv = uvSet.size();
            logger.info("UV count: " + uv);

            // TODO　Insert into DB
            if (this.frequency == Frequency.DAILY) {
                
            }
        }
    }

    public static void main(String args[]) {
        HashSet<Integer> sets = new HashSet<Integer>();
        for (int i = 0; i < 3000000; i++) {
            sets.add(i);
        }
        try {
            Thread.sleep(40000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

}
