package com.renren.ad.sns.services.uranus.cf;

import static org.junit.Assert.*;

import java.util.List;
import java.util.Map;

import org.junit.Test;

import com.renren.ad.sns.services.uranus.Candidate;
/**
 * @author xiang.zhang
 * last modified: 2012/06/05
 */
public class CFRecommendationLoaderTest {

    @Test
    public void testLoadRecommendations() {
        String resultFile = "./test-data/recommendations-test.dat";
        Map<Long, List<Candidate>> uId2Candidates = CFRecommendationLoader
                .loadRecommendations(resultFile);
        assertTrue(uId2Candidates != null);
        assertEquals(3, uId2Candidates.size());
        long[] uids = {5560, 14180, 22460};
        for(long uid : uids){
            assertTrue(uId2Candidates.containsKey(uid));
        }
    }

}
