package org.apache.mahout.fpm.pfpgrowth.extra;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.junit.Test;

public class BruteForceAssociationRuleProducerTest {

    @Test
    public void testCalcSupport() {
        Map<Set<String>, Long> fpLookup = new HashMap<Set<String>, Long>();
        fpLookup.put(new HashSet<String>(Arrays.asList("a", "b", "c")), 5L);
        fpLookup.put(new HashSet<String>(Arrays.asList("a", "c")), 10L);
        fpLookup.put(new HashSet<String>(Arrays.asList("b", "c")), 20L);

        assertEquals(20L, BruteForceAssociationRuleProducer.calcSupport(fpLookup, new HashSet<String>(Arrays
                .asList("c"))));
    }

    @Test
    public void testCombinationNumber() {
        List<String> fp = Arrays.asList("a", "b", "c");
        
        /* test case for consequence length == 1 */
        int consequenceLen = 1;
        int[] subset = new int[consequenceLen];
        for (int i = 0; i < subset.length; i++) {
            subset[i] = i;
        }
        int loopTimes = 0;
        while (true) {
            Set<String> premise = BruteForceAssociationRuleProducer.getPremise(fp, subset);
            if (premise == null) {
                break;
            }

            Set<String> consequence = BruteForceAssociationRuleProducer.getConsequence(fp, subset);

            switch (loopTimes) {
                case 0:
                    assertTrue(premise.equals(new HashSet<String>(Arrays.asList("b", "c"))));
                    assertTrue(consequence.equals(new HashSet<String>(Arrays.asList("a"))));
                    break;
                case 1:
                    assertTrue(premise.equals(new HashSet<String>(Arrays.asList("a", "c"))));
                    assertTrue(consequence.equals(new HashSet<String>(Arrays.asList("b"))));
                    break;
                case 2:
                    assertTrue(premise.equals(new HashSet<String>(Arrays.asList("a", "b"))));
                    assertTrue(consequence.equals(new HashSet<String>(Arrays.asList("c"))));
                    break;
                default:
                    fail("loop times is greater than 3.");
            }

            BruteForceAssociationRuleProducer.nextSubset(subset, fp.size());
            loopTimes++;
        }

        /* test case for consequence length == 2 */
        consequenceLen = 2;
        subset = new int[consequenceLen];
        for (int i = 0; i < subset.length; i++) {
            subset[i] = i;
        }
        loopTimes = 0;
        while (true) {
            Set<String> premise = BruteForceAssociationRuleProducer.getPremise(fp, subset);
            if (premise == null) {
                break;
            }

            Set<String> consequence = BruteForceAssociationRuleProducer.getConsequence(fp, subset);

            switch (loopTimes) {
                case 0:
                    assertTrue(premise.equals(new HashSet<String>(Arrays.asList("c"))));
                    assertTrue(consequence.equals(new HashSet<String>(Arrays.asList("a", "b"))));
                    break;
                case 1:
                    assertTrue(premise.equals(new HashSet<String>(Arrays.asList("b"))));
                    assertTrue(consequence.equals(new HashSet<String>(Arrays.asList("a", "c"))));
                    break;
                case 2:
                    assertTrue(premise.equals(new HashSet<String>(Arrays.asList("a"))));
                    assertTrue(consequence.equals(new HashSet<String>(Arrays.asList("b", "c"))));
                    break;
                default:
                    fail("loop times is greater than 3.");
            }

            BruteForceAssociationRuleProducer.nextSubset(subset, fp.size());
            loopTimes++;
        }

    }
}
