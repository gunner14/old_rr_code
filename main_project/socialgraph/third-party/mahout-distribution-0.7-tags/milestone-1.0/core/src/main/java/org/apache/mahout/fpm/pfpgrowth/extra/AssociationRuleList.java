package org.apache.mahout.fpm.pfpgrowth.extra;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import org.apache.hadoop.io.Writable;

public class AssociationRuleList implements Writable {

    private List<AssociationRule> ruleList;

    public AssociationRuleList(List<AssociationRule> ruleList) {
        super();
        this.ruleList = ruleList;
    }

    @Override
    public void readFields(DataInput in) throws IOException {
        List<AssociationRule> tmpRuleList = new ArrayList<AssociationRule>();
        int numRules = in.readInt();
        for (int i = 0; i < numRules; i++) {
            AssociationRule rule = new AssociationRule();
            rule.readFields(in);
            tmpRuleList.add(rule);
        }
        this.ruleList = tmpRuleList;
    }

    @Override
    public void write(DataOutput out) throws IOException {
        out.writeInt(ruleList.size());
        for (AssociationRule rule : ruleList) {
            rule.write(out);
        }
    }
}
