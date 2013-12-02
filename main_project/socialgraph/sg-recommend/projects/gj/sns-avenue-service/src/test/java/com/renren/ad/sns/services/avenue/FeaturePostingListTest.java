package com.renren.ad.sns.services.avenue;

import java.util.List;
import java.util.Random;

import org.junit.BeforeClass;
import org.junit.Test;

/**
 *
 * @author xiong.hua po.xu refactored
 */
public class FeaturePostingListTest {

    @BeforeClass
    public static void setUpBeforeClass() throws Exception {}

    @Test
    public void testMerge() {
        FeaturePostingList l1 = new FeaturePostingList();
        FeaturePostingList l2 = new FeaturePostingList();
        Random rand = new Random(System.currentTimeMillis());
        for( int i =0 ;i < 10 ;i ++ ) {
            l1.add(new DocPosting(i,i,rand.nextDouble()));
        }
        for( int i =1 ;i < 20 ;i +=2 ) {
            l2.add(new DocPosting(i*20,i,rand.nextDouble()));
        }
        l1.add(l2.peek());
        List<DocPosting> fpl = FeaturePostingList.merge(l1, l2);
        System.out.println(fpl);
    }

}

