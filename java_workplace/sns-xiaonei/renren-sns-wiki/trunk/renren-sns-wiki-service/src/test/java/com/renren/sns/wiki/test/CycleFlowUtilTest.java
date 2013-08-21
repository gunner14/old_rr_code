package com.renren.sns.wiki.test;

import java.util.ArrayList;
import java.util.List;

import org.junit.Test;

import com.renren.sns.wiki.utils.CycleFlowUtil;
import com.renren.sns.wiki.utils.CycleFlowUtil.CycleUnit;

/**
 * @author yi.li@renren-inc.com since Aug 8, 2012
 * 
 */
public class CycleFlowUtilTest {

    public static int a;

    @Test
    public void testCycleFlow() {
        CycleFlowUtil.submitCycleTask(2, 2, new CycleUnit<Integer>() {

            @Override
            public List<Integer> queryCycle(int begin, int limit) {
                a++;
                if (a > 5) {
                    return null;
                }

                List<Integer> result = new ArrayList<Integer>();
                result.add(1);
                return result;
            }

            @Override
            public void processCycle(List<Integer> datas) {
                System.out.println("==>" + a);
            }
        });
        System.out.println("done");
    }
}
