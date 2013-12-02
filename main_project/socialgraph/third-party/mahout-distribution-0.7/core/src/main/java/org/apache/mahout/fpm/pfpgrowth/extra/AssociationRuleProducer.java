package org.apache.mahout.fpm.pfpgrowth.extra;

import java.util.List;

import org.apache.mahout.fpm.pfpgrowth.convertors.string.TopKStringPatterns;


public interface AssociationRuleProducer {

    /**
     * Generate association rules from a group of patterns
     * @param patterns
     * @param minConfendence the minimum confedence * 100
     * @return
     */
    public List<AssociationRule> getAssociationRules(TopKStringPatterns patterns, int minConfendence);
}
