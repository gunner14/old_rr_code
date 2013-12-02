package org.apache.mahout.fpm.pfpgrowth.extra;

import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import java.util.ArrayList;
import java.util.List;

import org.junit.After;
import org.junit.Ignore;
import org.junit.Test;

import xce.tripod.TripodException;

import com.google.protobuf.InvalidProtocolBufferException;

public class CacheUtilsTest {

    private int userID = 123;

    @Ignore
    @Test
    public void testSetAssociationRule() throws TripodException, InvalidProtocolBufferException {
        AssociationRule rule = new AssociationRule(new int[] { 1, 2, 3 }, new int[] { 4, 5, 6 }, 5, 75, 100L);
        assertTrue(CacheUtils.setAssociationRule(userID + "", rule));
        AssociationRule rule2 = CacheUtils.getAssociationRule(userID + "");
        assertTrue(rule.equals(rule2));
    }

    @Test
    public void testSetAssociationRuleList() throws TripodException, InvalidProtocolBufferException {
        List<AssociationRule> ruleList = new ArrayList<AssociationRule>();
        ruleList.add(new AssociationRule(new int[] { 1, 2, 3 }, new int[] { 4 }, 20, 90, 100L));
        ruleList.add(new AssociationRule(new int[] { 1, 2, 3 }, new int[] { 4, 5 }, 10, 90, 101L));
        ruleList.add(new AssociationRule(new int[] { 1, 2, 3 }, new int[] { 4, 5, 6 }, 5, 75, 102L));

        CacheUtils.resetStat();
        assertTrue(CacheUtils.setAssociationRuleList(userID, ruleList));
        List<AssociationRule> ruleList2 = CacheUtils.getAssociationRuleList(userID);
        assertTrue(listEquals(ruleList, ruleList2));
        System.out.println(CacheUtils.getSummaryStat());
    }

    private static boolean listEquals(List<AssociationRule> list1, List<AssociationRule> list2) {
        if (list1.size() != list2.size()) {
            return false;
        }
        for (int i = 0; i < list1.size(); i++) {
            if (!list1.get(i).equals(list2.get(i))) {
                return false;
            }
        }

        return true;
    }

    @After
    public void clear() {
        if (!CacheUtils.remove(userID + CacheUtils.ASSOCIATION_RULE_SUFFIX)) {
            fail("can not remove key:" + userID + CacheUtils.ASSOCIATION_RULE_SUFFIX);
        }
    }
}
