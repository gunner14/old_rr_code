package org.apache.mahout.fpm.pfpgrowth.extra;

import java.io.IOException;

import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

public class GeneRuleIndexMapper extends Mapper<LongWritable, Text, LongWritable, RuleIDAndWeightWritable> {

    static final LongWritable outKey = new LongWritable();

    public void map(LongWritable key, Text val, Context context) throws IOException, InterruptedException {
        String[] items = val.toString().split("\t");
        long ruleID = Long.parseLong(items[0]);
        
        AssociationRule rule = AssociationRule.parseRule(items[1]);
        for (int preID : rule.getPremise()) {
            outKey.set(preID);
            RuleIDAndWeightWritable outVal = new RuleIDAndWeightWritable(ruleID, rule.getWeight(preID));
            context.write(outKey, outVal);
            ////
            //System.out.println(String.format("<%d, %s>", outKey.get(), outVal.toString()));
            ////
        }
    }
}


