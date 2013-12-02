package org.apache.mahout.fpm.pfpgrowth.extra;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;

import org.apache.hadoop.io.Writable;
import org.apache.mahout.cf.taste.common.TopK;

import com.google.common.primitives.Ints;

public class TopKAssociationRules implements Writable {

    private List<AssociationRule> rules;

    public TopKAssociationRules() {
        rules = new ArrayList<AssociationRule>();
    }

    public TopKAssociationRules(AssociationRule rule) {
        this();
        this.rules.add(rule);
    }

    public TopKAssociationRules(List<AssociationRule> rules) {
        this();
        this.rules.addAll(rules);
    }

    public int getIntSize() {
        int sum = 0;
        for (AssociationRule rule : rules) {
            sum += rule.getIntSize();
        }
        return sum;
    }

    public int size() {
        return rules.size();
    }

    @Override
    public String toString(){
        return rules.toString();
    }
    
    @Override
    public boolean equals(Object other) {
        if (!(other instanceof TopKAssociationRules)) {
            return false;
        }

        TopKAssociationRules that = (TopKAssociationRules) other;
        if (this.rules.size() != that.rules.size()) {
            return false;
        }
        for (int i = 0; i < this.rules.size(); i++) {
            if (!this.rules.get(i).equals(that.rules.get(i))) {
                return false;
            }
        }
        return true;
    }

    @Override
    public int hashCode() {
        int result = 17;
        for (AssociationRule rule : rules) {
            result = result * 31 + rule.hashCode();
        }
        return result;
    }

    /**
     * Merge two TopKAssociationRules according to the following rule: sort all rules by support,
     * confidence, select top-limit rules
     * 
     * @param raw
     * @param limit
     * @return
     */
    public TopKAssociationRules merge(TopKAssociationRules another, int limit) {
        TopK<AssociationRule> topKAr = new TopK<AssociationRule>(limit, BY_SUPPORT_CONFIDENCE);
        for (AssociationRule rule : this.rules) {
            topKAr.offer(rule);
        }
        for (AssociationRule rule : another.rules) {
            topKAr.offer(rule);
        }

        return new TopKAssociationRules(topKAr.retrieve());
    }

    public static final Comparator<AssociationRule> BY_SUPPORT_CONFIDENCE = new Comparator<AssociationRule>() {

        @Override
        public int compare(AssociationRule o1, AssociationRule o2) {
            return o1.getSupport() == o2.getSupport() ? Ints.compare(o1.getConfidence(), o2.getConfidence()) : Ints
                    .compare(o1.getSupport(), o2.getSupport());
        }

    };

    @Override
    public void readFields(DataInput in) throws IOException {
        List<AssociationRule> tmpRules = new ArrayList<AssociationRule>();
        int size = in.readInt();
        for (int i = 0; i < size; i++) {
            AssociationRule rule = new AssociationRule();
            rule.readFields(in);
            tmpRules.add(rule);
        }
        rules = tmpRules;
    }

    @Override
    public void write(DataOutput out) throws IOException {
        out.writeInt(rules.size());
        for (AssociationRule rule : rules) {
            rule.write(out);
        }
    }

}
