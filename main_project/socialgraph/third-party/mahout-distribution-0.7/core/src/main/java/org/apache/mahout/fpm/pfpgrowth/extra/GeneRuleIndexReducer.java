package org.apache.mahout.fpm.pfpgrowth.extra;

import java.io.IOException;
import java.util.Comparator;
import java.util.List;

import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.mahout.cf.taste.common.TopK;

import com.google.common.primitives.Ints;

public class GeneRuleIndexReducer extends
        Reducer<LongWritable, RuleIDAndWeightWritable, LongWritable, ArrayWritable<LongWritable>> {

    /* cap the rule size */
    private int maxRulesPerUser = 100;

    public enum Counters {
        NUM_RULE_INDEX, 
        NUM_INT_ITEM
    };

    private static final Comparator<RuleIDAndWeightWritable> BY_WEIGHT = new Comparator<RuleIDAndWeightWritable>() {

        @Override
        public int compare(RuleIDAndWeightWritable one, RuleIDAndWeightWritable two) {
            return Ints.compare(one.getWeight(), two.getWeight());
        }
    };

    @Override
    public void reduce(LongWritable key, Iterable<RuleIDAndWeightWritable> vals, Context context) throws IOException,
            InterruptedException {
        TopK<RuleIDAndWeightWritable> topKRules = new TopK<RuleIDAndWeightWritable>(maxRulesPerUser, BY_WEIGHT);
        for (RuleIDAndWeightWritable idAndWeight : vals) {
            topKRules.offer(new RuleIDAndWeightWritable(idAndWeight.getRuleID(), idAndWeight.getWeight()));
            //FIXME why the following way is incorrect?
            //topKRules.offer(idAndWeight);
        }

        LongWritable[] values = new LongWritable[topKRules.size()];
        List<RuleIDAndWeightWritable> topKRuleList = topKRules.retrieve();
        for (int i = 0; i < values.length; i++) {
            values[i] = new LongWritable(topKRuleList.get(i).getRuleID());
        }
        ArrayWritable<LongWritable> outVal = new ArrayWritable<LongWritable>(LongWritable.class, values);
        ////
        //System.out.println(topKRuleList);
        //System.out.println(String.format("<%d, %s>", key.get(), outVal.toString()));
        ////
        context.write(key, outVal);
        
        /* for stat purpose only */
        context.getCounter(Counters.NUM_RULE_INDEX).increment(1);
        int numIntItems = outVal.get().length * 2;
        context.getCounter(Counters.NUM_INT_ITEM).increment(numIntItems);
    }
}
