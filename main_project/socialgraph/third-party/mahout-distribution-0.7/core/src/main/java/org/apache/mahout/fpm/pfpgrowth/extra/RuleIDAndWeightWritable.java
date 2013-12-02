package org.apache.mahout.fpm.pfpgrowth.extra;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

import org.apache.hadoop.io.Writable;

public class RuleIDAndWeightWritable implements Writable {

    private long ruleID;
    private int weight;

    public RuleIDAndWeightWritable() {
    }

    public RuleIDAndWeightWritable(long ruleID, int weight) {
        super();
        this.ruleID = ruleID;
        this.weight = weight;
    }

    public void setRuleID(long ruleID) {
        this.ruleID = ruleID;
    }

    public void setWeight(int weight) {
        this.weight = weight;
    }

    public long getRuleID() {
        return ruleID;
    }

    public int getWeight() {
        return weight;
    }

    @Override
    public String toString() {
        return String.format("(%d,%d)", ruleID, weight);
    }

    @Override
    public void readFields(DataInput in) throws IOException {
        ruleID = in.readLong();
        weight = in.readInt();
    }

    @Override
    public void write(DataOutput out) throws IOException {
        out.writeLong(ruleID);
        out.writeInt(weight);
    }

}
