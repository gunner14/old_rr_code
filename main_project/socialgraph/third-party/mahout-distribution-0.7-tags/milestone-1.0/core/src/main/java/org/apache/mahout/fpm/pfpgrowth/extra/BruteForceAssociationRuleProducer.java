package org.apache.mahout.fpm.pfpgrowth.extra;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.mahout.common.Pair;
import org.apache.mahout.fpm.pfpgrowth.convertors.string.TopKStringPatterns;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * A simple association rule producer.
 * 
 * @author xiang.zhang last updated: 2012/11/09
 */
public class BruteForceAssociationRuleProducer implements AssociationRuleProducer {

    private static final Logger LOG = LoggerFactory.getLogger(BruteForceAssociationRuleProducer.class);

    public static final int NO_MORE_RULE_MARK = Integer.MIN_VALUE;

    /* premise will contain this item */
    private String premiseKey;

    /* to avoid generate too many rules, use the following parameters to control the length of premise.
     * the valid length of premise is between:
     * [ max(FP.length*minPremiseLenRatio, FP.length-maxConsequenceLen), FP.length -1]
     */
    private float minPremiseLenRatio = 0.75f;
    private int maxConsequenceLen = 3;

    /* minimun length of FP */
    private int minFPLen = 2;

    /* maximum number of rules per item */
    private int maxRulesPerUser = 1000;

    public BruteForceAssociationRuleProducer(String premiseKey) {
        super();
        this.premiseKey = premiseKey;
    }

    public BruteForceAssociationRuleProducer(String premiseKey, int maxRulesPerUser) {
        this(premiseKey);
        this.maxRulesPerUser = maxRulesPerUser;
    }

    public BruteForceAssociationRuleProducer(String premiseKey, float minPremiseLenRatio) {
        this(premiseKey);
        this.minPremiseLenRatio = minPremiseLenRatio;
    }

    public BruteForceAssociationRuleProducer(String premiseKey, float minPremiseLenRatio, int minFPLen) {
        this(premiseKey, minPremiseLenRatio);
        this.minFPLen = minFPLen;
    }

    /**
     * Use this method to decrease possible candidate rules further
     * 
     * @param fpLen the length of frequent pattern
     */
    private void tuneParameter(int fpLen) {
        if (fpLen < 100) {
            return;
        } else if (fpLen >= 100 && fpLen < 1000) {
            this.maxConsequenceLen = 2;
        } else {
            this.maxConsequenceLen = 1;
        }
    }

    @Override
    public List<AssociationRule> getAssociationRules(TopKStringPatterns patterns, int minConfendence) {
        List<AssociationRule> rules = new ArrayList<AssociationRule>();
        // number of rules we have found
        int numRules = 0;
        // if we have found 'maxRulesPerItem' rules, set the mark to stop
        boolean stopMark = false;

        // generate FP lookup table
        Map<Set<String>, Long> fpLookup = new HashMap<Set<String>, Long>();
        for (Pair<List<String>, Long> pattern : patterns.getPatterns()) {
            fpLookup.put(new HashSet<String>(pattern.getFirst()), pattern.getSecond());
        }

        // iterate all frequent itemsets
        for (Pair<List<String>, Long> fp : patterns.getPatterns()) {
            final int fpLen = fp.getFirst().size();
            Set<String> premise = null;
            Set<String> consequence = null;
            if (fpLen <= minFPLen) {
                continue;
            }

            tuneParameter(fpLen);
            int maxConsequenceLen = Math.max(1, Math.min(this.maxConsequenceLen,
                    (int) (fpLen * (1 - minPremiseLenRatio))));

            /*
             *  iterate over all consequence length, from longer to shorter,
             *  as longer consequence is more useful.
             */
            for (int consequenceLen = maxConsequenceLen; consequenceLen >= 1; consequenceLen--) {
                int[] subset = new int[consequenceLen];
                for (int i = 0; i < subset.length; i++) {
                    subset[i] = i;
                }

                while (true) {
                    premise = getPremise(fp.getFirst(), subset);
                    if (premise == null) {
                        break;
                    }

                    if (valid(premise)) {
                        consequence = getConsequence(fp.getFirst(), subset);

                        long support = -1;
                        if (fpLookup.containsKey(premise)) {
                            support = fpLookup.get(premise);
                        } else {
                            /**
                             * fpLookup only conatains closed frequent itemset, each itemset
                             * contains the 'premiseKey' item. as the support counting of a sub
                             * frequent itemset is no less than its superset, so if a sub frequent
                             * itemset can not be found in fpLookup, its support counting is equal
                             * to the maximum support counting of all its super frequent itemset.
                             */
                            support = calcSupport(fpLookup, premise);
                        }

                        // check the minimum confidence
                        int confidence = (int) (100.0f * fp.getSecond().intValue() / support);
                        if (confidence >= minConfendence) {
                            rules.add(new AssociationRule(premise, consequence, fp.getSecond().intValue(), confidence));

                            // check if we should stop
                            if (++numRules >= maxRulesPerUser) {
                                stopMark = true;
                                break;
                            }
                        }
                    }

                    // generate the next premise/consequence
                    nextSubset(subset, fpLen);

                } // for each combination number

                if (stopMark) {
                    break;
                }
            } // for each consequence length

            if (stopMark) {
                break;
            }
        } // for each frequent itemset

        return rules;
    }

    /**
     * Calculate the support counting of the specified sub frequent itemset
     * 
     * @param fpLookup the closed frequent itemsets
     * @param itemset the sub frequent itemset
     * @return
     */
    public static long calcSupport(Map<Set<String>, Long> fpLookup, Set<String> itemset) {
        long maxSupp = -1;
        for (Map.Entry<Set<String>, Long> entry : fpLookup.entrySet()) {
            if (entry.getKey().containsAll(itemset) && entry.getValue() > maxSupp) {
                maxSupp = entry.getValue();
            }
        }
        return maxSupp;
    }

    /**
     * Check whether the premise is valid. <br/>
     * at present we only check whether the premise contains premiseKey.
     * 
     * @param premise
     * @return
     */
    private boolean valid(Set<String> premise) {
        return premise.contains(premiseKey);
    }

    public static Set<String> getPremise(List<String> fp, int[] subset) {
        // mark that we have no more rules to generate
        if (subset[0] == NO_MORE_RULE_MARK) {
            return null;
        }

        Set<String> premise = new HashSet<String>();
        boolean[] mark = new boolean[fp.size()];
        Arrays.fill(mark, true);
        for (int idx : subset) {
            mark[idx] = false;
        }

        for (int i = 0; i < fp.size(); i++) {
            if (mark[i]) {
                premise.add(fp.get(i));
            }
        }
        return premise;
    }

    public static Set<String> getConsequence(List<String> fp, int[] subset) {
        Set<String> consequence = new HashSet<String>();
        for (int i = 0; i < subset.length; i++) {
            consequence.add(fp.get(subset[i]));
        }
        return consequence;
    }

    /**
     * Generate the next combination number
     * 
     * @param subset
     * @param length the length of FP
     */
    public static void nextSubset(int[] subset, int length) {
        for (int i = subset.length - 1, maxVal = length - 1; i >= 0; i--, maxVal--) {
            if (subset[i] < maxVal) {
                subset[i]++;
                for (int j = i + 1; j <= subset.length - 1; j++) {
                    subset[j] = subset[j - 1] + 1;
                }
                return;
            }
        }

        // if no more rules, set the mark
        subset[0] = NO_MORE_RULE_MARK;
    }
}
