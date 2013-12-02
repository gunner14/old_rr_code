package com.renren.ad.sns.services.uranus.cf;

import static org.junit.Assert.*;

import java.util.List;

import org.junit.BeforeClass;
import org.junit.Test;

import com.renren.ad.sns.services.uranus.Candidate;


public class CFCandidatePickerTest {

    private static CFCandidatePicker picker;
    
    @BeforeClass
    public static void init(){
        picker = new CFCandidatePicker("./test-data");
        picker.init();
    }
    
    
    @Test
    public void testPick() {
        // test a common user
        long uid = 5560;
        List<Candidate> candidates = picker.pick(uid, -1);
        assertNotNull(candidates);
        assertEquals(100, candidates.size());
        for (Candidate c : candidates){
            assertTrue(c.getWeight() <= CFConstants.NEW_MAX_VALUE && c.getWeight() >= CFConstants.NEW_MIN_VALUE);
        }
        
        // test an unknown user
        uid = -1;
        candidates = picker.pick(uid, -1);
        assertNotNull(candidates);
        assertEquals(0, candidates.size());
    }

}
