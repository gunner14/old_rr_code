package org.apache.mahout.fpm.pfpgrowth.extra;

import java.io.IOException;

import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.mahout.common.Parameters;
import org.apache.mahout.fpm.pfpgrowth.PFPGrowth;


public class AggregatingRulesCombiner  extends
        Reducer<IntWritable, TopKAssociationRules, IntWritable, TopKAssociationRules> {

    private int maxRulesPerUser = 100;

    @Override
    public void reduce(IntWritable key, Iterable<TopKAssociationRules> vals, Context context) throws IOException,
            InterruptedException {

        TopKAssociationRules mergedRules = new TopKAssociationRules();
        for (TopKAssociationRules rule : vals) {
            mergedRules = mergedRules.merge(rule, maxRulesPerUser);
        }
        context.write(key, mergedRules);
    }

    @Override
    protected void setup(Context context) throws IOException, InterruptedException {
        super.setup(context);
        Parameters params = new Parameters(context.getConfiguration().get(PFPGrowth.PFP_PARAMETERS, ""));
        maxRulesPerUser = Integer.parseInt(params.get(PFPGrowth.MAX_RULES_PER_USER));
    }
}

