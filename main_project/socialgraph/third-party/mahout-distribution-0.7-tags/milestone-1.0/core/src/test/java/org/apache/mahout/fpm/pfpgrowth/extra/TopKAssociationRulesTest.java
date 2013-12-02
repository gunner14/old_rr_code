package org.apache.mahout.fpm.pfpgrowth.extra;

import static org.junit.Assert.assertTrue;

import java.util.ArrayList;
import java.util.List;

import org.junit.Test;

public class TopKAssociationRulesTest {

    @Test
    public void testMerge() {

        List<AssociationRule> rules = new ArrayList<AssociationRule>();
        rules.add(new AssociationRule(new int[] {}, new int[] {}, 10, 80));
        rules.add(new AssociationRule(new int[] {}, new int[] {}, 10, 85));
        rules.add(new AssociationRule(new int[] {}, new int[] {}, 7, 90));

        List<AssociationRule> rules2 = new ArrayList<AssociationRule>();
        rules2.add(new AssociationRule(new int[] {}, new int[] {}, 8, 100));
        rules2.add(new AssociationRule(new int[] {}, new int[] {}, 12, 85));
        rules2.add(new AssociationRule(new int[] {}, new int[] {}, 8, 90));

        TopKAssociationRules topKRules = new TopKAssociationRules(rules);
        TopKAssociationRules topKRules2 = new TopKAssociationRules(rules2);

        // boundary test
        TopKAssociationRules mergeRules = topKRules.merge(topKRules2, 1);
        assertTrue(mergeRules.equals(new TopKAssociationRules(new AssociationRule(new int[] {}, new int[] {}, 12, 85))));

        // common test
        mergeRules = topKRules.merge(topKRules2, 3);
        rules = new ArrayList<AssociationRule>();
        rules.add(new AssociationRule(new int[] {}, new int[] {}, 12, 85));
        rules.add(new AssociationRule(new int[] {}, new int[] {}, 10, 85));
        rules.add(new AssociationRule(new int[] {}, new int[] {}, 10, 80));
        TopKAssociationRules expected = new TopKAssociationRules(rules);
        assertTrue(mergeRules.equals(expected));

        // boundary test
        mergeRules = topKRules.merge(topKRules2, 6);
        rules = new ArrayList<AssociationRule>();
        rules.add(new AssociationRule(new int[] {}, new int[] {}, 12, 85));
        rules.add(new AssociationRule(new int[] {}, new int[] {}, 10, 85));
        rules.add(new AssociationRule(new int[] {}, new int[] {}, 10, 80));
        rules.add(new AssociationRule(new int[] {}, new int[] {}, 8, 100));
        rules.add(new AssociationRule(new int[] {}, new int[] {}, 8, 90));
        rules.add(new AssociationRule(new int[] {}, new int[] {}, 7, 90));
        expected = new TopKAssociationRules(rules);
        assertTrue(mergeRules.equals(expected));

        // common test
        mergeRules = topKRules.merge(topKRules2, 1000);
        assertTrue(mergeRules.equals(expected));

    }

}
