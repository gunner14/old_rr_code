package org.apache.mahout.fpm.pfpgrowth.extra;

import java.io.IOException;

import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

public class AggregatingRulesMapper extends Mapper<LongWritable, Text, IntWritable, TopKAssociationRules> {

    static final IntWritable outKey = new IntWritable();

    @Override
    public void map(LongWritable key, Text val, Context context) throws IOException, InterruptedException {
        String ruleStr = val.toString();
        AssociationRule rule = AssociationRule.parseRule(ruleStr);
        for (int userID : rule.getPremise()) {
            outKey.set(userID);
            TopKAssociationRules outVal = new TopKAssociationRules(rule);
            context.write(outKey, outVal);
        }
    }
}
