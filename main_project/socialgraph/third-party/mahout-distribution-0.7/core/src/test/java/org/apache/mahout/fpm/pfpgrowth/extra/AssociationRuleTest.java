package org.apache.mahout.fpm.pfpgrowth.extra;

import static org.junit.Assert.assertTrue;

import java.util.ArrayList;
import java.util.List;

import org.junit.Test;

public class AssociationRuleTest {

    @Test
    public void testAssociationRule() {
        AssociationRule rule = new AssociationRule(new int[] { 2, 1 }, new int[] { 4, 1, 3 }, 10, 20);
        AssociationRule rule2 = new AssociationRule(new int[] { 1, 2 }, new int[] { 3, 4, 1 }, 10, 20);
        AssociationRule rule3 = new AssociationRule();
        rule3.setPremise(new int[] { 1, 2 });
        rule3.setConsequence(new int[] { 3, 1, 4 });
        rule3.setSupport(10);
        rule3.setConfidence(20);

        assertTrue(rule.equals(rule2));
        assertTrue(rule.equals(rule3));
        assertTrue(rule2.equals(rule3));
    }

    @Test
    public void testParseRuleList() {
        List<AssociationRule> rules = new ArrayList<AssociationRule>();
        rules.add(new AssociationRule(new int[] { 2, 1 }, new int[] { 4, 1, 3 }, 10, 20));
        rules.add(new AssociationRule(new int[] { 2, 1 }, new int[] { 4, 1, 3 }, 10, 20));
        assertTrue(rules.equals(AssociationRule.parseRuleList(rules.toString())));

        String raw = "[(56217109,200573077->223262266 30 93 1), (56217109,223262266->200573077 30 81 2)]";
        List<AssociationRule> parsedRules = AssociationRule.parseRuleList(raw);
        List<AssociationRule> expectRules = new ArrayList<AssociationRule>();
        expectRules.add(new AssociationRule(new int[] { 56217109, 200573077 }, new int[] { 223262266 }, 30, 93, 1));
        expectRules.add(new AssociationRule(new int[] { 56217109, 223262266 }, new int[] { 200573077 }, 30, 81, 2));
        assertTrue(listEquals(expectRules, parsedRules));
    }

    private boolean listEquals(List<AssociationRule> expectRules, List<AssociationRule> parsedRules) {
        if (expectRules != null && parsedRules != null) {
            if (expectRules.size() != parsedRules.size()) {
                return false;
            }

            for (int i = 0; i < expectRules.size(); i++) {
                if (!expectRules.get(i).equals(parsedRules.get(i))) {
                    return false;
                }
            }

            return true;
        }
        return false;
    }

    @Test
    public void testParseRule() {
        AssociationRule rule = new AssociationRule(new int[] { 2, 1 }, new int[] { 4, 1, 3 }, 10, 20);
        assertTrue(rule.equals(AssociationRule.parseRule(rule.toString())));
    }

}
